# cython: c_string_type=str, c_string_encoding=ascii
cimport elib
from pkgcore.ebuild.errors import InvalidVersion

def version_cmp(v1, v2):
    cdef elib.cmp_code ret = elib.version_cmp(v1, v2)
    if ret == elib.ERROR:
        raise InvalidVersion(v1, v2)
    return ret

def version_cmp_ext(v1, r1, v2, r2):
    if v1 == v2:
        return (r1 > r2) - (r1 < r2)
    if v1 is None:
        v1 = ''
    if v2 is None:
        v2 = ''
    ret = version_cmp(v1, v2)
    if ret == 0:
        return (r1 > r2) - (r1 < r2)
    return ret

def version_match(v1, v2, operation):
    cdef elib.atom_op op = elib.atom_op_from_str(operation)
    cdef int ret = elib.version_match(v1, v2, op)
    if ret == -1:
        raise InvalidVersion(v1, v2)
    return ret > 0
