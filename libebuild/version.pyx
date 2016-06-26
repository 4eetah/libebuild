cimport cversion
from pkgcore.ebuild.errors import InvalidVersion

def version_cmp(const char *v1, const char *v2):
    cdef cversion.cmp_code ret = cversion.version_cmp(v1, v2) 
    if ret == cversion.ERROR:
        raise InvalidVersion(v1, v2)
    return ret

def version_cmp_ext(v1, r1, v2, r2):
    if v1 == v2:
        return cmp(r1, r2)
    if v1 is None:
        v1 = ''
    if v2 is None:
        v2 = ''
    ret = version_cmp(str(v1), str(v2))
    if ret == 0:
        return cmp(r1, r2)
    return ret

def version_match(const char *v1, const char *v2, const char *operation):
    cdef cversion.atom_op op = cversion.atom_op_from_str(operation)
    cdef int ret = cversion.version_match(v1, v2, op)
    if ret == -1:
        raise InvalidVersion(v1, v2)
    return ret > 0
