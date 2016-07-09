# cython: c_string_type=str, c_string_encoding=ascii
cimport elib
from cpython.object cimport Py_LT, Py_LE, Py_EQ, Py_GT, Py_GE, Py_NE
from pkgcore.ebuild.errors import InvalidCPV

cdef class cpv(object):
    cdef elib.CPV *_cpv

    cdef readonly str category
    cdef readonly str package
    cdef readonly str cpvstr
    cdef readonly str key
    cdef readonly str version
    cdef readonly str fullver
    cdef readonly object revision

    def __init__(self, *args, versioned=True):
        for x in args:
            if not isinstance(x, basestring):
                raise TypeError("all args must be strings, got %r" % (args,))
        cdef Py_ssize_t l = len(args)
        if l == 1:
            cpv_arg = args[0]
        elif l == 3:
            cpv_arg = "%s/%s-%s" % args
        else:
            raise TypeError("CPV takes 1 arg (cpvstr), or 3 (cat, pkg, ver):"
                " got %r" % (args,))

        self._cpv = elib.cpv_alloc(cpv_arg, versioned)
        if self._cpv is NULL:
            raise InvalidCPV(elib.ebuild_strerror(elib.ebuild_errno))
        self.category = self._cpv.CATEGORY
        self.package = self._cpv.PN
        self.key = self.category + "/" + self.package
        if versioned:
            self.cpvstr = self.key + "-" + self._cpv.PVR
            self.version = self._cpv.PV
            self.fullver = self._cpv.PVR
            self.revision = self._cpv.PR_int if self._cpv.PR_int else None
        else:
            self.cpvstr = self.key
            self.version = None 
            self.fullver = None
            self.revision = None

    def __dealloc__(self):
        elib.cpv_free(self._cpv)

    def __str__(self):
        return self.cpvstr 

    def __repr__(self):
        return '<%s %s @#%x>' % (
            self.__class__.__name__, self.cpvstr, id(self))

    def __hash__(self):
        return hash(self.cpvstr)
    
    def __richcmp__(cpv self, cpv other, int op):
        cdef elib.cmp_code ret = elib.cpv_cmp(self._cpv, other._cpv)
        if   op == Py_LT:
            return ret == elib.OLDER
        elif op == Py_LE:
            return ret == elib.OLDER or ret == elib.EQUAL
        elif op == Py_EQ:
            return ret == elib.EQUAL
        elif op == Py_NE:
            return ret != elib.EQUAL
        elif op == Py_GT:
            return ret == elib.NEWER
        elif op == Py_GE:
            return ret == elib.NEWER or ret == elib.EQUAL
        else:
            assert False
