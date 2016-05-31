#include "common.h"

const char * const version_suffixes_str[] = {"alpha", "beta", "pre", "rc", "p", ""};

void cpv_print(const CPV *cpv)
{
    if (!cpv) {
        printf("NULL\n");
        return;
    }
    printf("P: %s\n", cpv->P);
    printf("PN: %s\n", cpv->PN);
    printf("PV: %s\n", cpv->PV);
    printf("PR: %s\n", cpv->PR);
    printf("PVR: %s\n", cpv->PVR);
    printf("PF: %s\n", cpv->PF);
    printf("CATEGORY: %s\n", cpv->CATEGORY);
    printf("letter: %c\n", cpv->letter);
    printf("suffixes:\n");
    int i;
    for (i = 0; cpv->suffixes[i].suffix != SUF_NORM; ++i) {
        printf("%s", version_suffixes_str[cpv->suffixes[i].suffix]);
        printf("%lu\n", cpv->suffixes[i].val);
    }
}

int getsuffix(const char *suff)
{
    size_t len = sizeof(version_suffixes_str) / sizeof(*version_suffixes_str);
    int i;
    for (i = 0; i < len; ++i)
        if (!strncmp(suff, version_suffixes_str[i], 
                    strlen(version_suffixes_str[i])))
            return i;
}

bool isversion(const char *vers)
{
    char *ptr = (char*)vers;
    int tmp;
    if (!isdigit(*ptr))
        return 0;
    while (isdigit(*++ptr))
        ;

    while (ptr[0] == '.' && isdigit(ptr[1]))
        while (isdigit(*++ptr))
            ;

    if (isalpha(*ptr)) // optional version letter
        if (!isalpha(ptr[1]) && islower(ptr[0]))
            ++ptr;
        else
            return 0;

    while (ptr[0] == '_' && (tmp = strlen(version_suffixes_str[getsuffix(&ptr[1])]))) {
        ptr += tmp + 1;
        while (isdigit(*ptr)) //optional suffix integer
            ++ptr;
    }
    if (ptr[0] == '-' && ptr[1] == 'r' && isdigit(ptr[2])) {
        ptr += 2;
        while (isdigit(*ptr))
            ++ptr;
    }

    return !*ptr;
}

static CPV *cpv_alloc_versioned(const char *cpv_string)
{
    CPV *ret;
    char *ptr, *tmp_ptr;
    size_t m_len, cpv_len, cpv_string_len;
    int id, sid, sid_len;

    // CPV + P + PF + PVR + (CATEGORY,PN,PV,PR) + (optional -r0)
    cpv_len = sizeof(CPV);
    cpv_string_len = strlen(cpv_string) + 1;
    m_len = cpv_len + cpv_string_len * 4 + 3;

    ret = malloc(m_len);
    if (ret == NULL)
        err("Out of memory error\n");
    memset(ret, 0, m_len);

    ptr = (char*)ret;
    ret->P = ptr + cpv_len;
    ret->PF = ret->P + cpv_string_len;
    ret->PVR = ret->PF + cpv_string_len; 
    ret->CATEGORY = ret->PVR + cpv_string_len;
    strcpy(ret->CATEGORY, cpv_string);

    ptr = ret->CATEGORY;
    while (*ptr && *++ptr != '/')
        if (!VALID_CHAR(*ptr))
            goto cpv_error;

    // category
    if (*ptr) {
        if (INVALID_FIRST_CHAR(*(ret->CATEGORY)))
            goto cpv_error;
        *ptr = '\0';
        ret->PN = ptr + 1;
    } else {
        ret->PN = ret->CATEGORY;
        ret->CATEGORY = NULL; // category may be empty
    }
    if (INVALID_FIRST_CHAR(*(ret->PN)))
        goto cpv_error;
    strcpy(ret->PF, ret->PN);

    // revision
    ptr = ret->PN + strlen(ret->PN) - 1;
    tmp_ptr = ptr;
    ret->PV = ptr + 1;
    while (ptr > ret->PN) {
        if (!VALID_CHAR(*ptr))
            goto cpv_error;
        else if (!isdigit(*ptr)) {
            if (ptr[-1] == '-' && ptr[0] == 'r' && isdigit(ptr[1])) {
                ret->PR = ptr;
                ptr[-1] = '\0';
                ptr -= 2;
            } else {
                strcpy(&tmp_ptr[2], "r0");
                ret->PR = &tmp_ptr[2];
            }
            break;
        }
        --ptr;
    }
    strcpy(ret->P, ret->PN);
 
    // version
    while (ptr > ret->PN) {
        if (ptr[-1] == '-' && isdigit(ptr[0])) {
            ptr[-1] = '\0';
            ret->PV = ptr;
            break;
        }
        ptr--;
    }
    if (!isversion(ret->PV))
        goto cpv_error;

    // pkgname shouldn't end with a hyphen followed by a valid version
    if ((ptr = strrchr(ret->PN, '-')) && isversion(&ptr[1]))
        goto cpv_error;

    ptr = ret->PN;
    while (*++ptr)
        if (!VALID_CHAR(*ptr))
            goto cpv_error;

    // optional version letter
    if ((ptr = strchr(ret->PV, '_')) && isalpha(ptr[-1]))
        ret->letter = ptr[-1];
    else if (isalpha(ret->PR[-2]))
        ret->letter = ret->PR[-2];

    // suffixes
    id = 0;
    ret->suffixes = realloc(ret->suffixes, sizeof(suffix_ver) * (id + 1));
    if (ret->suffixes == NULL)
        err("Out of memory error\n");
    ret->suffixes[id].suffix = SUF_NORM;
    ret->suffixes[id].val = 0;

    while (ptr && *ptr) {
        if (!(tmp_ptr = strchr(&ptr[1], '_')))
            tmp_ptr = ptr + strlen(ptr);

        sid = getsuffix(&ptr[1]);
        ret->suffixes[id].suffix = sid;
        sid_len = strlen(version_suffixes_str[sid]);
        ptr += sid_len;

        char num[tmp_ptr - ptr];
        strncpy(num, &ptr[1], tmp_ptr - ptr - 1);
        num[tmp_ptr - ptr] = '\0';
        ret->suffixes[id].val = atoll(num);

        id++;
        ret->suffixes = realloc(ret->suffixes, sizeof(suffix_ver) * (id + 1));
        if (ret->suffixes == NULL)
            err("Out of memory error\n");
        ret->suffixes[id].suffix = SUF_NORM;
        ret->suffixes[id].val = 0;

        ptr = tmp_ptr;
    }

    strcpy(ret->PVR, ret->PV);
    if (strcmp(ret->PR, "r0")) {
        strcat(ret->PVR, "-");
        strcat(ret->PVR, ret->PR);
    }
    return ret;
cpv_error:
    cpv_free(ret);
    return NULL;
}

static CPV *cpv_alloc_unversioned(const char *cpv_string)
{
    CPV *ret;
    char *ptr, *tmp_ptr;
    size_t m_len, cpv_len, cpv_string_len;

    // CPV + (CATEGORY,PN)
    cpv_len = sizeof(CPV);
    cpv_string_len = strlen(cpv_string) + 1;
    m_len = cpv_len + cpv_string_len;

    ret = malloc(m_len);
    if (ret == NULL)
        err("Out of memory error\n");
    memset(ret, 0, m_len);

    ptr = (char*)ret;
    ret->CATEGORY = ptr + cpv_len;
    strcpy(ret->CATEGORY, cpv_string);

    ptr = ret->CATEGORY;
    while (*ptr && *++ptr != '/')
        if (!VALID_CHAR(*ptr))
            goto cpv_error;

    // category
    if (*ptr) {
        if (INVALID_FIRST_CHAR(*(ret->CATEGORY)))
            goto cpv_error;
        *ptr = '\0';
        ret->PN = ptr + 1;
    } else {
        ret->PN = ret->CATEGORY;
        ret->CATEGORY = NULL; // category may be empty
    }
    if (INVALID_FIRST_CHAR(*(ret->PN)))
        goto cpv_error;

    // pkgname shouldn't end with a hyphen followed by a valid version
    if ((ptr = strrchr(ret->PN, '-')) && isversion(&ptr[1]))
        goto cpv_error;

    ptr = ret->PN;
    while (*++ptr)
        if (!VALID_CHAR(*ptr))
            goto cpv_error;

    ret->suffixes = malloc(sizeof(suffix_ver));
    if (ret->suffixes == NULL)
        err("Out of memory error\n");
    ret->suffixes[0].suffix = SUF_NORM;
    ret->suffixes[0].val = 0;

    return ret;
cpv_error:
    cpv_free(ret);
    return NULL;
}

CPV *cpv_alloc(const char *cpv_string, bool versioned)
{
    if (versioned)
        return cpv_alloc_versioned(cpv_string);
    else
        return cpv_alloc_unversioned(cpv_string);
}

void cpv_free(CPV *cpv)
{
    if (!cpv) return;
    free(cpv->suffixes);
    free(cpv);
}
