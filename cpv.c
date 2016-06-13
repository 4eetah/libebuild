#include "common.h"

void cpv_print(const CPV *cpv)
{
    if (!cpv) {
        printf("NULL\n");
        return;
    }
    printf("P: %s\n", cpv->P);
    printf("PN: %s\n", cpv->PN);
    printf("PV: %s\n", cpv->PV);
    printf("PR: %d\n", cpv->PR_int);
    printf("PVR: %s\n", cpv->PVR);
    printf("PF: %s\n", cpv->PF);
    printf("CATEGORY: %s\n", cpv->CATEGORY);
    printf("letter: %c\n", cpv->letter);
    printf("suffixes: ");
    int i;
    for (i = 0; cpv->suffixes[i].suffix != SUF_NORM; ++i) {
        printf("%s", version_suffixes_str[cpv->suffixes[i].suffix]);
        printf("%lu ", cpv->suffixes[i].val);
    }
    printf("\n");
}

static CPV *cpv_alloc_versioned(const char *cpv_string)
{
    CPV *ret;
    char *ptr, *tmp_ptr, *end_ptr;
    size_t m_len, cpv_len, cpv_string_len;
    int id, sid, sid_len;

    // CPV + P + PF + PVR + (CATEGORY,PN,PV,PR)
    cpv_len = sizeof(CPV);
    cpv_string_len = strlen(cpv_string) + 1;
    m_len = cpv_len + cpv_string_len * 4;

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

    // category
    ptr = ret->CATEGORY;
    end_ptr = ptr + cpv_string_len - 2;
    if (INVALID_FIRST_CHAR(*ptr))
        goto cpv_error;
    while (*++ptr != '/')
        if (!VALID_CHAR(*ptr))
            goto cpv_error;

    *ptr = '\0';
    ret->PN = ptr + 1;
    if (INVALID_FIRST_CHAR(*(ret->PN)))
        goto cpv_error;
    strcpy(ret->PF, ret->PN);

    tmp_ptr = NULL;
    ptr = end_ptr;
    ret->PV = end_ptr + 1;
    ret->PR_int = 0;
 
    // version
    while (ptr > ret->PN) {
        if (ptr[0] == '-' && isdigit(ptr[1])) {
            end_ptr = ptr;
            ret->PV = &ptr[1];
            break;
        }
        ptr--;
    }
    if (!isvalid_version(ret->PV))
        goto cpv_error;
    else {
        strcpy(ret->PVR, ret->PV);
        // revision
        if (ptr = strchr(ret->PV, '-')) {
            ptr[0] = '\0';
            ret->PR_int = atoi(&ptr[2]);
            tmp_ptr = ptr - 1;
        }
        strcpy(ret->P, ret->PN);
        *end_ptr = '\0';
    }

    ptr = ret->PN;
    while (*++ptr)
        if (!VALID_CHAR(*ptr))
            goto cpv_error;
        // pkgname shouldn't end with a hyphen followed by a valid version
        else if (ptr[0] == '-' && isdigit(ptr[1]) && isvalid_version(&ptr[1]))
            goto cpv_error;

    // optional version letter
    if (ptr = strchr(ret->PV, '_')) {
        if (isalpha(ptr[-1]))
            ret->letter = ptr[-1];
    } else if (tmp_ptr) {
        if (isalpha(*tmp_ptr))
            ret->letter = *tmp_ptr;
    } else if ((tmp_ptr = &ret->PV[strlen(ret->PV)-1]) && (isalpha(*tmp_ptr)))
        ret->letter = *tmp_ptr;

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

    // CPV + PF + (CATEGORY,PN)
    cpv_len = sizeof(CPV);
    cpv_string_len = strlen(cpv_string) + 1;
    m_len = cpv_len + cpv_string_len * 2;

    ret = malloc(m_len);
    if (ret == NULL)
        err("Out of memory error\n");
    memset(ret, 0, m_len);

    ptr = (char*)ret;
    ret->PF = ptr + cpv_len;
    ret->CATEGORY = ret->PF + cpv_string_len;
    strcpy(ret->CATEGORY, cpv_string);

    ptr = ret->CATEGORY;
    // set empty version
    ret->P = ptr + cpv_string_len - 1;
    ret->PV = ret->P;
    ret->PR_int = 0;
    ret->PVR = ret->P;

    // category
    if (INVALID_FIRST_CHAR(*ptr))
        goto cpv_error;
    while (*++ptr != '/')
        if (!VALID_CHAR(*ptr))
            goto cpv_error;

    *ptr = '\0';
    ret->PN = ptr + 1;
    if (INVALID_FIRST_CHAR(*(ret->PN)))
        goto cpv_error;
    strcpy(ret->PF, ret->PN);

    ptr = ret->PN;
    while (*++ptr)
        if (!VALID_CHAR(*ptr))
            goto cpv_error;
        // pkgname shouldn't end with a hyphen followed by a valid version
        else if (ptr[0] == '-' && isdigit(ptr[1]) && isvalid_version(&ptr[1]))
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

CPV *cpv_alloc(const char *cpv_string, int versioned)
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

/*
 * NOTE: unversioned cpv implicitly gets 0 version
 */
cmp_code cpv_cmp(const CPV *c1, const CPV *c2)
{
    if (!c1 || !c2)
        return ERROR;

    if (strcmp(c1->CATEGORY, c2->CATEGORY) ||
        strcmp(c1->PN, c2->PN))
        return NOT_EQUAL;

    return version_cmp(c1->PVR, c2->PVR);
}

cmp_code cpv_cmp_str(const char *s1, const char *s2)
{
    cmp_code ret;
    char *ptr;
    CPV *c1, *c2;
    if (!(c1 = cpv_alloc(s1, 1)))
        c1 = cpv_alloc(s1, 0);
    if (!c1)
        return ERROR;

    if (!(c2 = cpv_alloc(s2, 1)))
        c2 = cpv_alloc(s2, 0);
    if (!c2)
        goto cpv_error;

    ret = cpv_cmp(c1, c2);

    cpv_free(c2);
cpv_error:
    cpv_free(c1);
    return ret;
}
