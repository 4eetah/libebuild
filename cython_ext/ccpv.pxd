cdef extern from "cpv.h":

    int ebuild_errno

    ctypedef enum cmp_code:
        OLDER = -1
        EQUAL
        NEWER
        NOT_EQUAL
        ERROR

    ctypedef struct CPV:
        char *P
        char *PN
        char *PV
        unsigned int PR_int
        char *PVR
        char *PF
        char *CATEGORY

    CPV *cpv_alloc(const char *cpv_string, bint versioned)
    void cpv_free(CPV *cpv)
    cmp_code cpv_cmp(const CPV *cpv1, const CPV *cpv2)
    cmp_code cpv_cmp_str(const char *s1, const char *s2)
    const char *ebuild_strerror(int code)
