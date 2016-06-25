cimport ccpv
from cpython.object cimport Py_LT, Py_LE, Py_EQ, Py_GT, Py_GE, Py_NE
from error import InvalidCPV

cdef class cpv(object):
    cdef ccpv.CPV *_cpv

    cdef readonly char *P
    cdef readonly char *PN
    cdef readonly char *PV
    cdef readonly unsigned int PR
    cdef readonly char *PVR
    cdef readonly char *PF
    cdef readonly char *CATEGORY

    def __init__(self, const char *cpv_string, versioned=True):
        self._cpv = ccpv.cpv_alloc(cpv_string, versioned)
        if self._cpv is NULL:
            raise InvalidCPV(ccpv.ebuild_strerror(ccpv.ebuild_errno))
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

    def __hash__(self):
        return hash(self.CATEGORY + self.PF)

    def __richcmp__(cpv self, cpv other, int op):
        cdef ccpv.cmp_code ret = ccpv.cpv_cmp(self._cpv, other._cpv)
        if   op == Py_LT:
            return ret == ccpv.OLDER
        elif op == Py_LE:
            return ret == ccpv.OLDER or ret == ccpv.EQUAL
        elif op == Py_EQ:
            return ret == ccpv.EQUAL
        elif op == Py_NE:
            return ret == ccpv.NOT_EQUAL
        elif op == Py_GT:
            return ret == ccpv.NEWER
        elif op == Py_GE:
            return ret == ccpv.NEWER or ret == ccpv.EQUAL
        else:
            assert False
