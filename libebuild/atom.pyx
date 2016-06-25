cimport catom 
from cpython.object cimport Py_LT, Py_LE, Py_EQ, Py_GT, Py_GE, Py_NE
from error import InvalidAtom

cdef class atom(object):
    cdef catom.ATOM *_atom
    cdef bytes atom_str

    cdef readonly char *P
    cdef readonly char *PN
    cdef readonly char *PV
    cdef readonly unsigned int PR
    cdef readonly char *PVR
    cdef readonly char *PF
    cdef readonly char *CATEGORY
    cdef readonly char *SLOT
    cdef readonly char *SUBSLOT
    cdef readonly char *REPO
    cdef readonly char *pfx_op
    cdef readonly char *sfx_op
    cdef readonly char *block_op
    cdef readonly tuple USE_DEPS

    def __init__(self, const char *atom_string, int eapi=6):
        self._atom = catom.atom_alloc_eapi(atom_string, eapi)
        if self._atom is NULL:
            raise InvalidAtom(catom.ebuild_strerror(catom.ebuild_errno))
        self.atom_str = <bytes>atom_string
        self.P = self._atom.P
        self.PN = self._atom.PN
        self.PV = self._atom.PV
        self.PR = self._atom.PR_int
        self.PVR = self._atom.PVR
        self.PF = self._atom.PF
        self.CATEGORY = self._atom.CATEGORY
        self.SLOT = self._atom.SLOT
        self.SUBSLOT = self._atom.SUBSLOT
        self.REPO = self._atom.REPO
        self.pfx_op = catom.atom_op_str[<int>self._atom.pfx_op]
        self.sfx_op = catom.atom_op_str[<int>self._atom.sfx_op]
        self.block_op = catom.atom_op_str[<int>self._atom.block_op]
        cdef list use_deps = []
        cdef int i = 0
        while self._atom.USE_DEPS[i] is not NULL:
            use_deps.append(self._atom.USE_DEPS[i])
            i = i + 1
        self.USE_DEPS = tuple(use_deps)

    def __dealloc__(self):
        catom.atom_free(self._atom)

    def __str__(self):
        return self.atom_str

    def __repr__(self):
        return '<%s %s @#%x>' % (self.__class__.__name__, self.atom_str, id(self))

    def __hash__(self):
        return hash(self.atom_str)

    def __richcmp__(atom self, atom other, int op):
        cdef catom.cmp_code ret = catom.atom_cmp(self._atom, other._atom)
        if   op == Py_LT:
            return ret == catom.OLDER
        elif op == Py_LE:
            return ret == catom.OLDER or ret == catom.EQUAL
        elif op == Py_EQ:
            return ret == catom.EQUAL
        elif op == Py_NE:
            return ret == catom.NOT_EQUAL
        elif op == Py_GT:
            return ret == catom.NEWER
        elif op == Py_GE:
            return ret == catom.NEWER or ret == catom.EQUAL
        else:
            assert False

    def intersects(atom self, atom other):
        return catom.atom_intersect(self._atom, other._atom) > 0
