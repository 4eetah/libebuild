#include <Python.h>
#include <structmember.h>
#include <stddef.h>
#include "common.h"

static PyObject *libebuild_InvalidCPV_Exc = NULL;

static PyMemberDef libebuild_cpv_members[] = {
	{"category", T_STRING, offsetof(CPV, CATEGORY), READONLY},
	{"package", T_STRING, offsetof(CPV, PN), READONLY},
	//{"key", T_OBJECT, offsetof(CPV, key), READONLY},
	{"fullver", T_STRING, offsetof(CPV, PF), READONLY},
	{"version", T_STRING, offsetof(CPV, PV), READONLY},
	{"revision", T_STRING, offsetof(CPV, PR), READONLY},
	{NULL}
};

static PyObject* 
libebuild_cpv_init(CPV *self, PyObject *args, PyObject *kwds)
{
    int result = 0;
    bool isversioned = true;
    PyObject *cat, *pkg, *ver, *versioned, *cpvstr, *dummy;
    cat = pkg = ver = cpvstr = dummy = NULL;
    char *keylist[] = {"versioned", NULL};

    Py_ssize_t tsize = PyTuple_Size(args);
    if (tsize != 1 && tsize != 3) {
        PyObject *err_msg = PyString_FromString(
            "cpv accepts either 1 arg (cpvstr), or 3 (category, package, "
            "version); all must be strings: got %r");
        if(err_msg) {
            PyObject *new_args = PyTuple_Pack(1, args);
            if(new_args) {
                PyObject *s = PyString_Format(err_msg, new_args);
                if(s) {
                    PyErr_SetString(PyExc_TypeError, PyString_AsString(s));
                    Py_CLEAR(s);
                }
                Py_CLEAR(new_args);
            }
            Py_CLEAR(err_msg);
        }
        return NULL;
    }
    if (!PyArg_ParseTuple(args, "S|SS", &cat, &pkg, &ver))
        return NULL;

    dummy = PyTuple_New(0);
    if (!PyArg_ParseTupleAndKeywords(dummy, kwds, "|O", keylist, &versioned))
        return NULL;
    if (versioned && !PyObject_IsTrue(versioned))
        isversioned = false;

    if (pkg)
        cpvstr = PyString_FromFormat("%s/%s-%s",
                                    PyString_AsString(cat),
                                    PyString_AsString(pkg),
                                    PyString_AsString(ver));
    else
        cpvstr = cat;

    /* do actual stuff here */
    self = cpv_alloc(PyString_AsString(cpvstr), isversioned);
    cpv_print(self);
    if (!self) {
        PyObject *tmp = PyObject_CallFunctionObjArgs(libebuild_InvalidCPV_Exc, cpvstr, NULL);
        if(NULL != tmp) {
            PyErr_SetObject(libebuild_InvalidCPV_Exc, tmp);
            Py_DECREF(tmp);
        }
    }
}

static void
libebuild_cpv_dealloc(CPV *self)
{
//    cpv_free(self);
}

static PyTypeObject libebuild_cpvType = {
	PyObject_HEAD_INIT(NULL)
	0,								/* ob_size */
	"CPV",
	sizeof(CPV),			  /* tp_basicsize */
	0,								/* tp_itemsize */
	(destructor)libebuild_cpv_dealloc,  /* tp_dealloc */
	0,								/* tp_print */
	0,								/* tp_getattr */
	0,								/* tp_setattr */
	0,	 /* tp_compare */
	0,//(reprfunc)libebuild_cpv_repr,	   /* tp_repr */
	0,								/* tp_as_number */
	0,								/* tp_as_sequence */
	0,								/* tp_as_mapping */
	0,	   /* tp_hash */
	0,								/* tp_call */
	0,//(reprfunc)libebuild_cpv_str,		/* tp_str */
	0,								/* tp_getattro */
	0,								/* tp_setattro */
	0,								/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
	0,								/* tp_doc */
	0,								/* tp_traverse */
	0,								/* tp_clear */
	0,								/* tp_richcompare */
	0,								/* tp_weaklistoffset */
	0,								/* tp_iter */
	0,								/* tp_iternext */
	0,								/* tp_methods */
	libebuild_cpv_members,			  /* tp_members */
	0,		   /* tp_getset */
	0,								/* tp_base */
	0,								/* tp_dict */
	0,								/* tp_descr_get */
	0,								/* tp_descr_set */
	0,								/* tp_dictoffset */
	(initproc)libebuild_cpv_init,	   /* tp_init */
	0,								/* tp_alloc */
	PyType_GenericNew,				/* tp_new */
};

PyDoc_STRVAR(
	libebuild_cpv_documentation,
	"C implementation of ebuild CPV parsing.");

PyMODINIT_FUNC
initcpv(void)
{
	PyObject *m;
	m = Py_InitModule3("cpv", NULL, libebuild_cpv_documentation);
	if (!m)
		return;

    if (PyType_Ready(&libebuild_cpvType) < 0)
        return;

    Py_INCREF(&libebuild_cpvType);
    if (PyModule_AddObject(m, "CPV", (PyObject *)&libebuild_cpvType) == -1)
        return;
}
