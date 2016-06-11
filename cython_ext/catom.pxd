cdef extern from "atom.h":

    const char * const atom_op_str[]

    ctypedef enum atom_op:
        ATOM_OP_NONE = 0
        ATOM_OP_NEWER
        ATOM_OP_NEWER_EQUAL
        ATOM_OP_EQUAL
        ATOM_OP_OLDER_EQUAL
        ATOM_OP_OLDER
        ATOM_OP_PV_EQUAL
        ATOM_OP_BLOCK
        ATOM_OP_BLOCK_HARD
        ATOM_OP_STAR

    ctypedef struct ATOM:
        char *P
        char *PN
        char *PV
        unsigned int PR_int
        char *PVR
        char *PF
        char *CATEGORY
        char *SLOT
        char *SUBSLOT
        char *REPO
        char **USE_DEPS
        atom_op pfx_op, sfx_op
        atom_op block_op

    ATOM *atom_alloc(const char *atom_string)
    void atom_free(ATOM *atom)
    int atom_cmp(const ATOM *atom1, const ATOM *atom2)
