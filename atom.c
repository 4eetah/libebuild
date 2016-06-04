#include "common.h"

const char * const atom_op_str[] = { "", ">", ">=", "=", "<=", "<", "~", "!", "!!", "*" };
const char * const version_suffixes_str[] = {"alpha", "beta", "pre", "rc", "p", ""};

void atom_print(const ATOM *atom)
{
    if (!atom) {
        printf("NULL\n");
        return;
    }
    printf("P: %s\n", atom->P);
    printf("PN: %s\n", atom->PN);
    printf("PV: %s\n", atom->PV);
    printf("PR: %d\n", atom->PR_int);
    printf("PVR: %s\n", atom->PVR);
    printf("PF: %s\n", atom->PF);
    printf("CATEGORY: %s\n", atom->CATEGORY);
    printf("letter: %c\n", atom->letter);
    printf("suffixes: ");
    int i;
    for (i = 0; atom->suffixes[i].suffix != SUF_NORM; ++i) {
        printf("%s", version_suffixes_str[atom->suffixes[i].suffix]);
        printf("%lu ", atom->suffixes[i].val);
    }
    printf("\n");
    printf("SLOT: %s\n", atom->SLOT);
    printf("SUBSLOT: %s\n", atom->SUBSLOT);
    printf("REPO: %s\n", atom->REPO);
    printf("USE_DEPS: ");
    for (i = 0; atom->USE_DEPS[i]; ++i)
        printf("%s ", atom->USE_DEPS[i]);
    printf("\n");
    printf("block_op: %s\n", atom_op_str[atom->block_op]);
    printf("pfx_op: %s\n", atom_op_str[atom->pfx_op]);
    printf("sfx_op: %s\n", atom_op_str[atom->sfx_op]);
}

ATOM *atom_alloc(const char* atom_string)
{
    ATOM *ret;
    char *ptr, *tmp_ptr, *end_ptr;
    size_t m_len, atom_len, atom_string_len;
    int id, sid, sid_len;

    // ATOM + P + PF + PVR + (CATEGORY,PN,PV,PR,SLOT,SUBSLOT,REPO,USE_DEPS)
    atom_len = sizeof(ATOM);
    atom_string_len = strlen(atom_string) + 1;
    m_len = atom_len + atom_string_len * 4;

    ret = malloc(m_len);
    if (ret == NULL)
        err("Out of memory error\n");
    memset(ret, 0, m_len);

    ptr = (char*)ret;
    ret->P = ptr + atom_len;
    ret->PF = ret->P + atom_string_len;
    ret->PVR = ret->PF + atom_string_len;
    ret->CATEGORY = ret->PVR + atom_string_len;
    
    // set trailing usedeps element
    id = 0;
    ret->USE_DEPS = realloc(ret->USE_DEPS, sizeof(char*) * (id + 1));
    ret->USE_DEPS[id++] = NULL;

    if (atom_string[0] == '!') {
        ++atom_string;
        --atom_string_len;
        if (atom_string[0] == '!') {
            ++atom_string;
            --atom_string_len;
            ret->block_op = ATOM_OP_BLOCK_HARD;
        } else
            ret->block_op = ATOM_OP_BLOCK;
    } else
        ret->block_op = ATOM_OP_NONE;

    switch (atom_string[0]) {
    case '>':
        ++atom_string;
        --atom_string_len;
        if (atom_string[0] == '=') {
            ++atom_string;
            --atom_string_len;
            ret->pfx_op = ATOM_OP_NEWER_EQUAL;
        } else
            ret->pfx_op = ATOM_OP_NEWER;
        break;
    case '=':
        ++atom_string;
        --atom_string_len;
        ret->pfx_op = ATOM_OP_EQUAL;
        break;
    case '<':
        ++atom_string;
        --atom_string_len;
        if (atom_string[0] == '=') {
            ++atom_string;
            --atom_string_len;
            ret->pfx_op = ATOM_OP_OLDER_EQUAL;
        } else
            ret->pfx_op = ATOM_OP_OLDER;
        break;
    case '~':
        ++atom_string;
        --atom_string_len;
        ret->pfx_op = ATOM_OP_PV_EQUAL;
        break;
    default:
        ret->pfx_op = ATOM_OP_NONE;
        break;
    }
    strcpy(ret->CATEGORY, atom_string);
    if ((ret->block_op != ATOM_OP_NONE) && (ret->pfx_op == ATOM_OP_NONE))
        goto atom_error;

    end_ptr = ret->CATEGORY + atom_string_len - 2;

    // usedeps
    if (*end_ptr == ']')
        if (ptr = strchr(ret->CATEGORY, '[')) {
            *ptr++ = '\0';
            while ((tmp_ptr = strchr(ptr, ',')) ||
                  ((tmp_ptr = end_ptr) >= ptr)) {
                *tmp_ptr = '\0';
                if (!isvalid_usedep(ptr))
                    goto atom_error;
                ret->USE_DEPS = realloc(ret->USE_DEPS, sizeof(char*) * (id + 1));
                ret->USE_DEPS[id - 1] = ptr;
                ret->USE_DEPS[id] = NULL;
                ptr = tmp_ptr + 1;
                ++id;
            }
            end_ptr = ret->USE_DEPS[0] - 2;
        } else
            goto atom_error;

    // repo
    if (ptr = strstr(ret->CATEGORY, "::")) {
        ret->REPO = ptr + 2;
        *ptr = '\0';
        end_ptr = ptr - 1;
        if (!isvalid_repo(ret->REPO))
            goto atom_error;
    } else
        ret->REPO = end_ptr + 1;

    // slot
    if (ptr = strrchr(ret->CATEGORY, ':')) {
        ret->SLOT = ptr + 1;
        *ptr = '\0';
        end_ptr = ptr - 1;
        if (!isvalid_slot(ret->SLOT))
            goto atom_error;
        if (ptr = strchr(ret->SLOT, '/')) {
            ret->SUBSLOT = ptr + 1;
            *ptr = '\0';
        } else
            ret->SUBSLOT = end_ptr + 1;
    } else {
        ret->SLOT = end_ptr + 1;
        ret->SUBSLOT = end_ptr + 1;
    }

    // match any base version
    if (*end_ptr == '*') {
        if (ret->pfx_op != ATOM_OP_EQUAL)
            goto atom_error;
        ret->sfx_op = ATOM_OP_STAR;
        *end_ptr = '\0';
        end_ptr--;
    } else
        ret->sfx_op = ATOM_OP_NONE;
    
    // category
    ptr = ret->CATEGORY;
    if (INVALID_FIRST_CHAR(*ptr))
        goto atom_error;
    while (*++ptr != '/')
        if (!VALID_CHAR(*ptr))
            goto atom_error;

    *ptr = '\0';
    ret->PN = ptr + 1;
    if (INVALID_FIRST_CHAR(*(ret->PN)))
        goto atom_error;
    strcpy(ret->PF, ret->PN);

    // version
    ptr = end_ptr;
    while (ptr > ret->PN) {
        if (ptr[0] == '-' && isdigit(ptr[1])) {
            tmp_ptr = ptr;
            ret->PV = &ptr[1];
            break;
        }
        ptr--;
    }

    // set default suffix
    id = 0;
    ret->suffixes = realloc(ret->suffixes, sizeof(suffix_ver) * (id + 1));
    if (ret->suffixes == NULL)
        err("Out of memory error\n");
    ret->suffixes[id].suffix = SUF_NORM;
    ret->suffixes[id].val = 0;

    if (ptr == ret->PN) {
        if ((ret->pfx_op != ATOM_OP_NONE) || (ret->sfx_op != ATOM_OP_NONE))
            goto atom_error;
        // set empty version
        ret->P   = end_ptr + 1;
        ret->PV  = end_ptr + 1;
        ret->PVR = end_ptr + 1;
        ret->PR_int = 0;
    } else if (ret->pfx_op == ATOM_OP_NONE) {
        goto atom_error;
    } else if (!isvalid_version(ret->PV)) {
        goto atom_error;
    } else {
        // got a valid version along with prefix operator
        strcpy(ret->PVR, ret->PV);
        end_ptr = NULL;

        // revision
        if (ptr = strchr(ret->PV, '-')) {
            ptr[0] = '\0';
            ret->PR_int = atoi(&ptr[2]);
            end_ptr = ptr - 1;
        } else
            ret->PR_int = 0;

        strcpy(ret->P, ret->PN);
        *tmp_ptr = '\0';

        // optional version letter
        if (ptr = strchr(ret->PV, '_')) {
            if (isalpha(ptr[-1]))
                ret->letter = ptr[-1];
        } else if (end_ptr) {
            if (isalpha(*end_ptr))
                ret->letter = *end_ptr;
        } else if ((end_ptr = &ret->PV[strlen(ret->PV)-1]) && (isalpha(*end_ptr)))
            ret->letter = *end_ptr;

        // suffixes
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
    }

    ptr = ret->PN;
    while (*++ptr)
        if (!VALID_CHAR(*ptr))
            goto atom_error;
        // pkgname shouldn't end with a hyphen followed by a valid version
        else if (ptr[0] == '-' && isdigit(ptr[1]) && isvalid_version(&ptr[1]))
            goto atom_error;

    return ret;
atom_error:
    atom_free(ret);
    return NULL;
}

void atom_free(ATOM *atom)
{
    if (!atom) return;
    free(atom->USE_DEPS);
    free(atom->suffixes);
    free(atom);
}
