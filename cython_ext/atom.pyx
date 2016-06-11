cimport catom 
from error import InvalidAtom

cdef class atom:
    cdef catom.ATOM *_atom
    cdef catom.atom_op atom_op
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

    def __cinit__(self, const char *atom_string):
        self._atom = catom.atom_alloc(atom_string)
        if self._atom is NULL:
            raise InvalidAtom("parse error, invalid input atom string")
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

    def __cmp__(self, atom other):
        cdef int ret = catom.atom_cmp(self._atom, other._atom)
        if   (ret < 0): return -1
        elif (ret > 0): return 1
        return 0
