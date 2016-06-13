#include "common.h"

int isvalid_eapi_reqs(const ATOM *atom, int eapi)
{
    if (!VALID_EAPI(eapi))
        return 0;

    if ((atom->block_op == ATOM_OP_BLOCK_HARD && eapi < 2)
        || (*atom->SLOT && eapi == 0)
        || (*atom->SLOT == '*' && eapi < 5)
        || (strchr(atom->SLOT, '=') && eapi < 5)
        || (*atom->SUBSLOT && eapi < 5)
        || (strchr(atom->SUBSLOT, '=') && eapi < 5)
        || (*atom->USE_DEPS && eapi < 2))
        return 0;

    int i;
    for (i = 0; atom->USE_DEPS[i]; ++i)
        if (strchr(atom->USE_DEPS[i], '(') && eapi < 4)
            return 0;

    return 1;
}

int isvalid_version(const char *ptr)
{
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

int isvalid_usedep(const char *ptr)
{
    char pfx = 0;

    if (USEDEP_PREF(*ptr)) {
        pfx = *ptr;
        ++ptr;
    }
    if (INVALID_FIRST_USE_CHAR(*ptr))
        return 0;

    ++ptr;
    while (VALID_USE_CHAR(*ptr))
        ++ptr;
    if (ptr[0] == '(' && (ptr[1] == '+' || ptr[1] == '-') && ptr[2] == ')')
        ptr += 3;

    if (USEDEP_SUF(*ptr)) {
        if (pfx != '-')
            ++ptr;
    } else if (pfx == '!')
        return 0;

    return !*ptr; 
}

int isvalid_repo(const char *ptr)
{
    if (INVALID_FIRST_REPO_CHAR(*ptr))
        return 0;
    ++ptr;
    while (VALID_REPO_CHAR(*ptr))
        ++ptr;
    
    return !*ptr;
}

int isvalid_slot(const char *ptr)
{
    if (INVALID_FIRST_CHAR(*ptr))
        if (SLOT_OPERATOR(*ptr))
            return !ptr[1];
        else
            return 0;
    ++ptr;
    while (VALID_CHAR(*ptr))
        ++ptr;
    if (ptr[0] == '/' && !INVALID_FIRST_CHAR(ptr[1]))
        ptr += 2;

    while (VALID_CHAR(*ptr))
        ++ptr;
    if (*ptr == '=')
        ++ptr;

    return !*ptr;
}

version_suffixes getsuffix(const char *suff)
{
    size_t len = sizeof(version_suffixes_str) / sizeof(*version_suffixes_str);
    version_suffixes i;
    for (i = 0; i < len; ++i)
        if (!strncmp(suff, version_suffixes_str[i], 
                     strlen(version_suffixes_str[i])))
            return i;
}

/*
 * pms version comparison logic
 */
cmp_code version_cmp(const char *v1, const char *v2)
{
    if (!isvalid_version(v1) || !isvalid_version(v2))
        return ERROR;

    char *ptr1, *ptr2;
    unsigned long long n1 = strtoll(v1, &ptr1, 10);
    unsigned long long n2 = strtoll(v2, &ptr2, 10);
    if (n1 > n2)
        return 1;
    else if (n1 < n2)
        return -1;

    char c1, c2;
    while ((*ptr1 == '.') && (*ptr2 == '.')) {
        ++ptr1; ++ptr2;
        if (*ptr1 == '0' || *ptr2 == '0') {
            while (isdigit(*ptr1) || isdigit(*ptr2)) {
                c1 = isdigit(*ptr1) ? *ptr1++ : '0';
                c2 = isdigit(*ptr2) ? *ptr2++ : '0';
                if (c1 > c2)
                    return 1;
                else if (c1 < c2)
                    return -1;
            }
        } else {
            n1 = strtoll(ptr1, &ptr1, 10);
            n2 = strtoll(ptr2, &ptr2, 10);
            if (n1 > n2)
                return 1;
            else if (n1 < n2)
                return -1;
        }
    }
    if (*ptr1 == '.')
        return 1;
    else if (*ptr2 == '.')
        return -1;

    if (isalpha(*ptr1) && isalpha(*ptr2)) {
        if (*ptr1 > *ptr2)
            return 1;
        else if (*ptr1 < *ptr2)
            return -1;
        ++ptr1; ++ptr2;
    }
    else if (isalpha(*ptr1))
        return 1;
    else if (isalpha(*ptr2))
        return -1;

    while ((*ptr1 == '_') && (*ptr2 == '_')) {
        ++ptr1; ++ptr2;
        version_suffixes suff1 = getsuffix(ptr1);
        version_suffixes suff2 = getsuffix(ptr2);
        if (suff1 > suff2)
            return 1;
        else if (suff1 < suff2)
            return -1;
        else {
            ptr1 += strlen(version_suffixes_str[suff1]);
            ptr2 += strlen(version_suffixes_str[suff2]);
            n1 = strtoll(ptr1, &ptr1, 10);
            n2 = strtoll(ptr2, &ptr2, 10);
            if (n1 > n2)
                return 1;
            else if (n1 < n2)
                return -1;
        }
    }
    if (*ptr1 == '_')
        return getsuffix(ptr1 + 1) == SUF_P ? 1 : -1;
    else if (*ptr2 == '_')
        return getsuffix(ptr2 + 1) == SUF_P ? -1 : 1;

    n1 = (*ptr1 == '-') ? atoll(ptr1 + 2) : 0;
    n2 = (*ptr2 == '-') ? atoll(ptr2 + 2) : 0;
    if (n1 > n2)
        return 1;
    else if (n1 < n2)
        return -1;
    
    return 0;
}
