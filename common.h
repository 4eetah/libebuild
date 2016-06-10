#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
#define USEDEP_PREF(c) ((c) == '!' || (c) == '-')
#define USEDEP_SUF(c) ((c) == '=' || (c) == '?')

#define SLOT_OPERATOR(c) ((c) == '*' || (c) == '=')

extern const char * const version_suffixes_str[6];
extern const char * const atom_op_str[10];

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
    unsigned int PR_int;
    char *PVR;
    char *PF;
    char *CATEGORY;
    char letter; /* optional version letter */
    suffix_ver *suffixes;
} CPV;

typedef struct {
    char *P;
    char *PN;
    char *PV;
    unsigned int PR_int;
    char *PVR;
    char *PF;
    char *CATEGORY;
    char *SLOT, *SUBSLOT;
    char *REPO;
    char **USE_DEPS; /* NULL as end indicator */
    char letter; /* optional version letter */
    suffix_ver *suffixes;
    atom_op pfx_op, sfx_op;
    atom_op block_op;
} ATOM;

/*
 * libebuild interface 
 ***/
CPV *cpv_alloc(const char *cpv_string, int versioned);
void cpv_free(CPV *cpv);
void cpv_print(const CPV *cpv);

ATOM *atom_alloc(const char *atom_string);
void atom_free(ATOM *atom);
void atom_print(const ATOM *atom);
/***/

int isvalid_version(const char *version);
int isvalid_usedep(const char *usedep);
int isvalid_repo(const char *repo);
int isvalid_slot(const char *slot);
version_suffixes getsuffix(const char *suff);

#define err(msg) do { perror(msg); exit(1); } while (0)

#endif // COMMON_H
