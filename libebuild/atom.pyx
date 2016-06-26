cimport catom 
from cpython.object cimport Py_LT, Py_LE, Py_EQ, Py_GT, Py_GE, Py_NE
from pkgcore.ebuild.errors import MalformedAtom

cdef class atom(object):
    cdef catom.ATOM *_atom
    cdef bytes atom_str

    cdef readonly char *category
    cdef readonly char *package
    cdef readonly str   cpvstr
    cdef readonly str   key
    cdef readonly bytes op
    cdef readonly object blocks
    cdef readonly object blocks_strongly
    cdef readonly bytes version
    cdef readonly bytes fullver
    cdef readonly object revision
    cdef readonly bytes slot_operator
    cdef readonly bytes slot
    cdef readonly bytes subslot
    cdef readonly bytes repo_id
    cdef readonly tuple use 

    def __init__(self, const char *atom_string, int eapi=6):
        self._atom = catom.atom_alloc_eapi(atom_string, eapi)
        if self._atom is NULL:
            raise MalformedAtom(catom.ebuild_strerror(catom.ebuild_errno))
        self.atom_str = <bytes>atom_string

        self.category = self._atom.CATEGORY
        self.package = self._atom.PN
        self.key = self.category + "/" + self.package
        self.version = self._atom.PV if self._atom.PV[0] else None
        self.fullver = self._atom.PVR if self._atom.PVR[0] else None
        self.revision = self._atom.PR_int if self._atom.PR_int else None
        self.repo_id = self._atom.REPO if self._atom.REPO[0] else None
        if self.fullver:
            self.cpvstr = self.key + "-" + self.fullver
        else:
            self.cpvstr = self.key

        if self._atom.pfx_op != catom.ATOM_OP_NONE:
            if self._atom.sfx_op == catom.ATOM_OP_STAR:
                self.op = catom.atom_op_str[<int>self._atom.pfx_op] + \
                          catom.atom_op_str[<int>self._atom.sfx_op]
            else:
                self.op = catom.atom_op_str[<int>self._atom.pfx_op]
        else:
            self.op = b''

        if self._atom.block_op == catom.ATOM_OP_BLOCK:
            self.blocks = True
            self.blocks_strongly = False
        elif self._atom.block_op == catom.ATOM_OP_BLOCK_HARD:
            self.blocks = True
            self.blocks_strongly = True
        else:
            self.blocks = False
            self.blocks_strongly = False

        if self._atom.SLOT[0]:
            if self._atom.SLOT[0] == '*' or self._atom.SLOT[0] == '=':
                self.slot_operator = self._atom.SLOT
                self.slot = None
                self.subslot = None
            elif self._atom.SLOT[-1] == '=':
                self.slot_operator = self._atom.SLOT[-1]
                self.slot = self._atom.SLOT[:-1]
                self.subslot = None
            elif self._atom.SUBSLOT[0]:
                self.slot = self._atom.SLOT
                if self._atom.SUBSLOT[-1] == '=':
                    self.slot_operator = self._atom.SUBSLOT[-1]
                    self.subslot = self._atom.SUBSLOT[:-1]
                else:
                    self.slot_operator = None
                    self.subslot = self._atom.SUBSLOT
            else:
                self.slot = self._atom.SLOT
                self.subslot = None
                self.slot_operator = None
        else:
            self.slot = None
            self.subslot = None
            self.slot_operator = None

        if self._atom.USE_DEPS[0] == NULL:
            self.use = None
        else:
            use_deps = []
            i = 0
            while self._atom.USE_DEPS[i] != NULL:
                use_deps.append(self._atom.USE_DEPS[i])
                i = i + 1
            self.use = tuple(sorted(use_deps))

    def __dealloc__(self):
        catom.atom_free(self._atom)

    def __str__(self):
        return self.atom_str

    def __repr__(self):
        return '<%s %s @#%x>' % (self.__class__.__name__, self.atom_str, id(self))

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
