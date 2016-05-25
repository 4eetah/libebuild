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

#define INVALID_FIRST_CAT_CHAR(c) (!isalnum(c) \
        || '-' == (c) \
        || '+' == (c) \
        || '.' == (c))

#define INVALID_FIRST_PKG_CHAR(c) (!isalnum(c) \
        || '-' == (c) \
        || '+' == (c))

typedef enum { SUF_ALPHA=0, SUF_BETA, SUF_PRE, SUF_RC, SUF_P, SUF_NORM } version_suffixes;

typedef enum {
    E_SUCCESS = 0,
    E_INVALID_CATEGORY,
    E_INVALID_PKG_NAME,
    E_INVALID_VERSION,
} ebuild_err;

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
int isversion(const char *version);
int hassuffix(const char *suff);
void cpv_print(const CPV *cpv);

#define err(msg) do { perror(msg); exit(1); } while (0)

#define SET_CPV_ERR(cpv, err) \
    do { \
        memset(cpv, 0, sizeof(CPV)); \
        cpv->cpv_err = err; \
    } while (0)

#endif // COMMON_H
