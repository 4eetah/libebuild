cimport ccpv
from error import InvalidCPV

cdef class cpv:
    cdef ccpv.CPV *_cpv

    cdef readonly char *P
    cdef readonly char *PN
    cdef readonly char *PV
    cdef readonly unsigned int PR
    cdef readonly char *PVR
    cdef readonly char *PF
    cdef readonly char *CATEGORY

    def __cinit__(self, const char *cpv_string, versioned=True):
        self._cpv = ccpv.cpv_alloc(cpv_string, versioned)
        if self._cpv is NULL:
            raise InvalidCPV("parse error, invalid input cpv string")
        self.P = self._cpv.P
        self.PN = self._cpv.PN
        self.PV = self._cpv.PV
        self.PR = self._cpv.PR_int
        self.PVR = self._cpv.PVR
        self.PF = self._cpv.PF
        self.CATEGORY = self._cpv.CATEGORY

    def __dealloc__(self):
        ccpv.cpv_free(self._cpv)

    def __str__(self):
        return self.CATEGORY + "/" + self.PF

    def __cmp__(self, cpv other):
        cdef int ret = ccpv.cpv_cmp(self._cpv, other._cpv)
        if   (ret < 0): return -1
        elif (ret > 0): return 1
        return 0
