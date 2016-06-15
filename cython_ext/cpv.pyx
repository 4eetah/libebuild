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

    def __repr__(self):
        return '<%s %s @#%x>' % (
            self.__class__.__name__, self.CATEGORY + "/" + self.PF, id(self))

    def __richcmp__(cpv self, cpv other, int op):
        cdef ccpv.cmp_code ret = ccpv.cpv_cmp(self._cpv, other._cpv)
        if   op == 0:
            return ret == ccpv.OLDER
        elif op == 1:
            return ret == ccpv.OLDER or ret == ccpv.EQUAL
        elif op == 2:
            return ret == ccpv.EQUAL
        elif op == 3:
            return ret == ccpv.NOT_EQUAL
        elif op == 4:
            return ret == ccpv.NEWER
        elif op == 5:
            return ret == ccpv.NEWER or ret == ccpv.EQUAL
