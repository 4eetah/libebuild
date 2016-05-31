#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#ifdef DOTS_DISALLOWED
#define VALID_CHAR(c) (isalnum(c) \
        || '-' == (c) \
        || '+' == (c) \
        || '_' == (c))
#else
#define VALID_CHAR(c) (isalnum(c) \
        || '-' == (c) \
        || '+' == (c) \
        || '_' == (c) \
        || '.' == (c))
#endif

#define VALID_USE_CHAR(c) (isalnum(c) \
        || '-' == (c) \
        || '+' == (c) \
        || '_' == (c) \
        || '@' == (c))

#define VALID_REPO_CHAR(c) (isalnum(c) \
        || '-' == (c) \
        || '_' == (c))

#define INVALID_FIRST_CHAR(c) (!VALID_CHAR(c) \
        || '-' == (c) \
        || '+' == (c) \
        || '.' == (c))

#define INVALID_FIRST_REPO_CHAR(c) (!VALID_REPO_CHAR(c) \
        || '-' == (c))

#define INVALID_FIRST_USE_CHAR(c) (!isalnum(c))

typedef enum { SUF_ALPHA=0, SUF_BETA, SUF_PRE, SUF_RC, SUF_P, SUF_NORM } version_suffixes;

typedef enum {
    /*    */ ATOM_OP_NONE = 0,
    /* >  */ ATOM_OP_NEWER,
    /* >= */ ATOM_OP_NEWER_EQUAL,
    /* =  */ ATOM_OP_EQUAL,
    /* <= */ ATOM_OP_OLDER_EQUAL,
    /* <  */ ATOM_OP_OLDER,
    /* ~  */ ATOM_OP_PV_EQUAL,
    /* !  */ ATOM_OP_BLOCK,
    /* !! */ ATOM_OP_BLOCK_HARD,
    /* *  */ ATOM_OP_STAR,
} atom_op;

typedef struct {
    version_suffixes suffix;
    unsigned long val;
} suffix_ver;

typedef struct {
    char *P;
    char *PN;
    char *PV;
    char *PR;
    char *PVR;
    char *PF;
    char *CATEGORY;
    char letter; /* optional version letter */
    suffix_ver *suffixes;
} CPV;

CPV *cpv_alloc(const char *cpv_string, bool versioned);
void cpv_free(CPV *cpv);
const char *cpv_err_msg(const CPV *cpv);
bool isversion(const char *version);
int hassuffix(const char *suff);
void cpv_print(const CPV *cpv);

typedef struct {
    char *P;
    char *PN;
    char *PV;
    char *PR;
    char *PVR;
    char *PF;
    char *CATEGORY;
    char *SLOT, *SUBSLOT, *SLOT_OP;
    char *REPO;
    char **USE_DEPS;
    char letter; /* optional version letter */
    suffix_ver *suffixes;
    atom_op *pfx_op, *sfx_op;
} ATOM;

#define err(msg) do { perror(msg); exit(1); } while (0)

#endif // COMMON_H
