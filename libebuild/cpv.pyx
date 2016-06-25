cimport ccpv
from cpython.object cimport Py_LT, Py_LE, Py_EQ, Py_GT, Py_GE, Py_NE
from pkgcore.ebuild.errors import InvalidCPV

cdef class cpv(object):
    cdef ccpv.CPV *_cpv

    cdef readonly char *category
    cdef readonly char *package
    cdef readonly str cpvstr
    cdef readonly str key
    cdef readonly object versioned

    def __init__(self, *args, versioned=True):
        for x in args:
            if not isinstance(x, basestring):
                raise TypeError("all args must be strings, got %r" % (args,))
        cdef Py_ssize_t l = len(args)
        cdef str cpv_arg
        if l == 1:
            cpv_arg = args[0]
        elif l == 3:
            cpv_arg = "%s/%s-%s" % args
        else:
            raise TypeError("CPV takes 1 arg (cpvstr), or 3 (cat, pkg, ver):"
                " got %r" % (args,))

        self.versioned = versioned
        self._cpv = ccpv.cpv_alloc(cpv_arg, self.versioned)
        if self._cpv is NULL:
            raise InvalidCPV(ccpv.ebuild_strerror(ccpv.ebuild_errno))
        self.category = self._cpv.CATEGORY
        self.package = self._cpv.PN
        self.key = self.category + "/" + self.package
        if self.versioned:
            self.cpvstr = self.key + "-" + self._cpv.PVR
        else:
            self.cpvstr = self.key

    property version:
        def __get__(self):
            return self._cpv.PV if self.versioned else None

    property fullver:
        def __get__(self):
            return self._cpv.PVR if self.versioned else None

    property revision:
        def __get__(self):
            return self._cpv.PR_int if self._cpv.PR_int else None

    def __dealloc__(self):
        ccpv.cpv_free(self._cpv)

    def __str__(self):
        return self.cpvstr 

    def __repr__(self):
        return '<%s %s @#%x>' % (
            self.__class__.__name__, self.cpvstr, id(self))

    def __hash__(self):
        return hash(self.cpvstr)

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
