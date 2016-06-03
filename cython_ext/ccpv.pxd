cdef extern from "cpv.h":
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
