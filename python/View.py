#include <Python.h>
#include <ctype.h>

static PyObject *view_format(PyObject *s, PyObject *args) {
    PyObject *bytes;
    if (!PyArg_ParseTuple(args, "O", &bytes)) return NULL;
    if (!PyBytes_Check(bytes)) Py_RETURN_NONE;

    unsigned char *data = (unsigned char *)PyBytes_AS_STRING(bytes);
    Py_ssize_t size = PyBytes_GET_SIZE(bytes);

    PyObject *out = PyUnicode_New(0, 127);

    for (Py_ssize_t i = 0; i < size; i += 16) {
        PyUnicode_AppendAndDel(&out, PyUnicode_FromFormat("%08lx  ", i));

        for (int j = 0; j < 16; j++) {
            if (i + j < size)
                PyUnicode_AppendAndDel(&out, PyUnicode_FromFormat("%02x ", data[i + j]));
            else
                PyUnicode_AppendAndDel(&out, PyUnicode_FromString("   "));
        }

        PyUnicode_AppendAndDel(&out, PyUnicode_FromString(" "));

        for (int j = 0; j < 16 && i + j < size; j++) {
            unsigned char c = data[i + j];
            PyUnicode_AppendAndDel(&out, PyUnicode_FromFormat("%c", isprint(c) ? c : '.'));
        }

        PyUnicode_AppendAndDel(&out, PyUnicode_FromString("\n"));
    }

    return out;
}

static PyMethodDef methods[] = {
    {"format", view_format, METH_VARARGS, NULL},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef mod = {
    PyModuleDef_HEAD_INIT,
    "View",
    NULL,
    -1,
    methods
};

PyMODINIT_FUNC PyInit_View() {
    return PyModule_Create(&mod);
}
