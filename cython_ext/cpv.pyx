cimport ccpv
from error import InvalidCPV

cdef class cpv:
    cdef ccpv.CPV *_cpv

    cdef readonly char *P
    cdef readonly char *PN
    cdef readonly char *PV
    cdef readonly char *PR
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
        self.PR = self._cpv.PR
        self.PVR = self._cpv.PVR
        self.PF = self._cpv.PF
        self.CATEGORY = self._cpv.CATEGORY

    def __dealloc__(self):
        if self._cpv is not NULL:
            ccpv.cpv_free(self._cpv)

    def __str__(self):
        if self.PV == b'':
            return self.CATEGORY + "/" + self.PN
        else:
            return self.CATEGORY + "/" + self.PF
