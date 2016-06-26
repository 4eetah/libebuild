cdef extern from "version.h":

    const char * const atom_op_str[]

    ctypedef enum cmp_code:
        OLDER=-1,
        EQUAL,
        NEWER,
        NOT_EQUAL,
        ERROR

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

    cmp_code version_cmp(const char *v1, const char *v2)
    int version_match(const char *v1, const char *v2, atom_op op)
    atom_op atom_op_from_str(const char *op);
