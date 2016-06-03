#include "common.h"

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

int getsuffix(const char *suff)
{
    size_t len = sizeof(version_suffixes_str) / sizeof(*version_suffixes_str);
    int i;
    for (i = 0; i < len; ++i)
        if (!strncmp(suff, version_suffixes_str[i], 
                     strlen(version_suffixes_str[i])))
            return i;
}
