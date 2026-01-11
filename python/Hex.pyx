#include <Python.h>
#include <stdio.h>

static PyObject *hex_read(PyObject *s, PyObject *args) {
    const char *path;
    if (!PyArg_ParseTuple(args, "s", &path)) return NULL;

    FILE *f = fopen(path, "rb");
    if (!f) Py_RETURN_NONE;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    PyObject *bytes = PyBytes_FromStringAndSize(NULL, size);
    fread(PyBytes_AS_STRING(bytes), 1, size, f);
    fclose(f);

    return bytes;
}

static PyMethodDef methods[] = {
    {"read", hex_read, METH_VARARGS, NULL},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef mod = {
    PyModuleDef_HEAD_INIT,
    "Hex",
    NULL,
    -1,
    methods
};

PyMODINIT_FUNC PyInit_Hex() {
    return PyModule_Create(&mod);
}
