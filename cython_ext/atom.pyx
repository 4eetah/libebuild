cimport catom 
from error import InvalidAtom

cdef class atom:
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

    def __cinit__(self, const char *atom_string, int eapi=6):
        self._atom = catom.atom_alloc(atom_string)
        if self._atom is NULL:
            raise InvalidAtom("parse error, invalid input atom string")
        if not catom.isvalid_eapi_reqs(self._atom, eapi):
            raise InvalidAtom("eapi constraints validation failed")
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
        use_deps = []
        i = 0
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

    def __richcmp__(atom self, atom other, int op):
        cdef catom.cmp_code ret = catom.atom_cmp(self._atom, other._atom)
        if   op == 0:
            return ret == catom.OLDER
        elif op == 1:
            return ret == catom.OLDER or ret == catom.EQUAL
        elif op == 2:
            return ret == catom.EQUAL
        elif op == 3:
            return ret == catom.NOT_EQUAL
        elif op == 4:
            return ret == catom.NEWER
        elif op == 5:
            return ret == catom.NEWER or ret == catom.EQUAL

    def intersects(atom self, atom other):
        return catom.atom_intersect(self._atom, other._atom) > 0
