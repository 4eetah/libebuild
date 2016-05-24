#include <Python.h>

#include <stddef.h>

static PyMethodDef libebuild_AndRestriction_methods[] = {
    {"match", (PyCFunction)libebuild_AndRestriction_match, METH_O},
//    {"force_False", (PyCFunction)libebuild_AndRestriction_forceFalse,
//        METH_VARARGS | METH_KEYWORDS},
//    {"force_True", (PyCFunction)libebuild_AndRestriction_forceTrue,
//        METH_VARARGS | METH_KEYWORDS},
//    {"change_restrictions", (PyCFunction)libebuild_AndRestriction_change_restriction,
//        METH_VARARGS | METH_KEYWORDS},
//    {"add_restrictions", (PyCFunction)libebuild_AndRestriction_add_restriction,
//        METH_VARARGS},
//    {"finalize", (PyCFunction)libebuild_AndRestriction_finalize, METH_NOARGS}
//    {"dnf_solutions", (PyCFunction)libebuild_AndRestriction_dnf_solutions,
//        METH_KEYWORDS}
//    {"iter_dnf_solutions", (PyCFunction)libebuild_AndRestriction_iter_dnf_solutions,
//        METH_KEYWORDS}
//    {"cnf_solutions", (PyCFunction)libebuild_AndRestriction_cnf_solutions,
//        METH_KEYWORDS}
//    {"iter_cnf_solutions", (PyCFunction)libebuild_AndRestriction_iter_cnf_solutions,
//        METH_KEYWORDS}
    {NULL}
}

static PyMemberDef libebuild_AndRestriction_members[] = {
    {"restrictions", T_OBJECT, offsetof(libebuild_AndRestriction, restrictions), READONLY},
    {"type", T_OBJECT, offsetof(libebuild_AndRestriction, restrictions), READONLY},
    {"_hash", T_OBJECT, offsetof(libebuild_AndRestriction, restrictions), READONLY},
    {NULL}
};

//snakeoil_IMMUTABLE_ATTR_BOOL(libebuild_AndRestriction, "negate", negate,

static PyObject *
libebuild_AndRestriction_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
}

static PyObject *
libebuild_AndRestriction_match(libebuild_AndRestriction *self,
    PyObject *value)
{
    return Py_BuildValue("i", 1);
}

static PyTypeObject pkgcore_AndRestriction_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                             /* ob_size*/
    "libebuild.boolean.AndRestriction",            /* tp_name*/
    sizeof(libebuild_AndRestriction),              /* tp_basicsize*/
    0,                                             /* tp_itemsize*/
    (destructor)libebuild_AndRestriction_dealloc,     /* tp_dealloc*/
    0,                                             /* tp_print*/
    0,                                             /* tp_getattr*/
    0,                                             /* tp_setattr*/
    0,                                             /* tp_compare*/
    0,                                             /* tp_repr*/
    0,                                             /* tp_as_number*/
    0,                                             /* tp_as_sequence*/
    0,                                             /* tp_as_mapping*/
    0,                                             /* tp_hash */
    (ternaryfunc)0,                               /* tp_call*/
    (reprfunc)0,                                     /* tp_str*/
    0,                                             /* tp_getattro*/
    0,                                             /* tp_setattro*/
    0,                                             /* tp_as_buffer*/
    Py_TPFLAGS_BASETYPE|Py_TPFLAGS_DEFAULT,        /* tp_flags*/
    libebuild_AndRestriction_documentation,             /* tp_doc */
    (traverseproc)0,                                 /* tp_traverse */
    (inquiry)0,                                   /* tp_clear */
    (richcmpfunc)libebuild_AndRestriction_richcompare,  /* tp_richcompare */
    0,                                             /* tp_weaklistoffset */
    (getiterfunc)0,                               /* tp_iter */
    (iternextfunc)0,                                 /* tp_iternext */
    libebuild_AndRestriction_methods,                 /* tp_methods */
    libebuild_AndRestriction_members,                 /* tp_members */
    libebuild_AndRestriction_attrs,                     /* tp_getset */
    0,                                             /* tp_base */
    0,                                             /* tp_dict */
    0,                                             /* tp_descr_get */
    0,                                             /* tp_descr_set */
    0,                                             /* tp_dictoffset */
    (initproc)0,                                     /* tp_init */
    0,                                             /* tp_alloc */
    libebuild_AndRestriction_new,                     /* tp_new */
};
