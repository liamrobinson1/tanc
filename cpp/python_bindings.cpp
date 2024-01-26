#include <Python.h>
#include <iostream>
#include <datetime.h>  // Include the Python datetime API
#include <numpy/arrayobject.h> // and numpy

using namespace std;

#include "time.hpp"
#include "profile.hpp"

typedef struct {
    PyObject_HEAD
    DateTime* cpp_obj;  // Pointer to the C++ object
} PyDateTimeObject;

static void PyDateTime_dealloc(PyDateTimeObject* self) {
    delete self->cpp_obj;  // Properly delete the C++ object
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* PyDateTime_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyDateTimeObject* self;
    self = (PyDateTimeObject*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cpp_obj = new DateTime(args);
    }
    return (PyObject*)self;
}

// Other methods for the Python object (e.g., getters, setters, etc.)
static PyObject* DateTime_get_year(PyDateTimeObject* self, void* closure) {
    return PyLong_FromLong(self->cpp_obj->year);
}

static PyGetSetDef DateTime_getsetters[] = {
    {"year", (getter)DateTime_get_year, NULL, "year", NULL},
    // Add entries for other attributes...
    {NULL}  /* Sentinel */
};

static PyTypeObject PyDateTimeType = {
    PyVarObject_HEAD_INIT(NULL, 0) // This macro initializes several fields
    .tp_name = "tanc.DateTime",     // The type’s name
    .tp_basicsize = sizeof(PyDateTimeObject),  // Memory size of one instance
    .tp_flags = Py_TPFLAGS_DEFAULT, // Default flags
    .tp_doc = "DateTime objects",   // Documentation string
    .tp_new = PyDateTime_new,       // "new" method for creating instances
    .tp_dealloc = (destructor) PyDateTime_dealloc, // "dealloc" method
    .tp_getset = DateTime_getsetters, // Getter and setter functions
    // Other fields and methods as needed...
};

PyObject* cpp_datetime_to_py_datetime(DateTime dt) {
    // Create a new Python datetime object
    PyObject* pydate = PyDateTime_FromDateAndTime(dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second, dt.nanosecond/1000.0);
    return pydate;
}

DateTime py_datetime_to_cpp_datetime(PyObject* pydate) {
    // Extract the date and time components from the datetime object
    int year = PyDateTime_GET_YEAR(pydate);
    int month = PyDateTime_GET_MONTH(pydate);
    int day = PyDateTime_GET_DAY(pydate);
    int hour = PyDateTime_DATE_GET_HOUR(pydate);
    int minute = PyDateTime_DATE_GET_MINUTE(pydate);
    int second = PyDateTime_DATE_GET_SECOND(pydate);
    int microsecond = PyDateTime_DATE_GET_MICROSECOND(pydate);

    // Create a new DateTime object
    DateTime dt = DateTime(year, month, day, hour, minute, second, microsecond*1000);
    return dt;
}

TimeDelta py_timedelta_to_cpp_timedelta(PyObject* pytd) {
    // Extract the time components from the timedelta object
    int days = PyDateTime_DELTA_GET_DAYS(pytd);
    int seconds = PyDateTime_DELTA_GET_SECONDS(pytd);
    int microseconds = PyDateTime_DELTA_GET_MICROSECONDS(pytd);
    // Create a new TimeDelta object
    TimeDelta td = TimeDelta(0, 0, days, 0, 0, seconds, microseconds*1000);
    return td;
}

PyObject* cpp_timedelta_to_py_timedelta(TimeDelta td) {
    // Create a new Python timedelta object
    PyObject* pytd = PyDelta_FromDSU(td.days, td.seconds, td.nanoseconds/1000.0);
    return pytd;
}

PyObject* cpp_datetimes_to_numpy_array(std::vector<DateTime> date_vec) {
    npy_intp dims[1] = {(npy_intp)date_vec.size()};
    PyObject* pylist = PyArray_SimpleNew(1, dims, NPY_OBJECT);
    int n_datetimes = date_vec.size();
    
    for (int i = 0; i < n_datetimes; i++) {
        DateTime dt = date_vec[i];
        PyObject* pydate = cpp_datetime_to_py_datetime(dt);
        PyArray_SETITEM((PyArrayObject*)pylist, PyArray_GETPTR1((PyArrayObject*)pylist, i), pydate);
    }
    return pylist;
}

static PyObject* datetime_linspace(PyObject* self, PyObject* args) {
    PyObject *py_dt1, *py_dt2;
    int n;

    // Parse the input objects as two datetime objects and an integer
    if (!PyArg_ParseTuple(args, "OOi", &py_dt1, &py_dt2, &n)) {
        return NULL;
    }

    // Ensure the objects are datetime objects
    if (!PyDateTime_Check(py_dt1) || !PyDateTime_Check(py_dt2)) {
        PyErr_SetString(PyExc_TypeError, "Expected two datetime objects.");
        return NULL;
    }
    
    DateTime dt1 = py_datetime_to_cpp_datetime(py_dt1);
    DateTime dt2 = py_datetime_to_cpp_datetime(py_dt2);
    std::vector<DateTime> date_vec = datetime_linspace(dt1, dt2, n);
    return cpp_datetimes_to_numpy_array(date_vec);
}

// similarly for arange
static PyObject* datetime_arange(PyObject* self, PyObject* args) {
    PyObject *py_dt1, *py_dt2, *py_td;
    // using a datetime.timedelta object for the step size

    // Parse the input objects as two datetime objects and a timedelta object
    if (!PyArg_ParseTuple(args, "OOO", &py_dt1, &py_dt2, &py_td)) {
        return NULL;
    }

    // Ensure the objects are datetime objects
    if (!PyDateTime_Check(py_dt1) || !PyDateTime_Check(py_dt2)) {
        PyErr_SetString(PyExc_TypeError, "Expected two datetime objects.");
        return NULL;
    }

    // Ensure the timedelta object is a timedelta object
    if (!PyDelta_Check(py_td)) {
        PyErr_SetString(PyExc_TypeError, "Expected a timedelta object.");
        return NULL;
    }

    DateTime dt1 = py_datetime_to_cpp_datetime(py_dt1);
    DateTime dt2 = py_datetime_to_cpp_datetime(py_dt2);
    TimeDelta td = py_timedelta_to_cpp_timedelta(py_td);

    std::vector<DateTime> date_vec = datetime_arange(dt1, dt2, td);
    return cpp_datetimes_to_numpy_array(date_vec);
}

static PyMethodDef datetime_methods[] = {
    {
        "linspace", 
        datetime_linspace, 
        METH_VARARGS, 
        "linspace(datetime_start, datetime_end, n)\n"
        "--\n\n"
        "Generate n evenly spaced DateTime objects between two specified DateTime points.\n\n"
        "Positional Arguments:\n"
        "datetime_start : datetime.datetime\n"
        "    The starting point as a datetime.datetime object.\n"
        "datetime_end : datetime.datetime\n"
        "    The ending point as a datetime.datetime object.\n"
        "n : int\n"
        "    The number of DateTime objects to generate.\n\n"
        "Returns:\n"
        "list of DateTime objects\n"
        "    A list containing n DateTime objects evenly spaced between the two specified DateTime points."
    },
    {
        "arange", 
        datetime_arange, 
        METH_VARARGS, 
        "arange(datetime_start, datetime_end, timedelta)\n"
        "--\n\n"
        "Generate DateTime objects between two specified DateTime points with a specified step size.\n\n"
        "Positional Arguments:\n"
        "datetime_start : datetime.datetime\n"
        "    The starting point as a datetime.datetime object.\n"
        "datetime_end : datetime.datetime\n"
        "    The ending point as a datetime.datetime object.\n"
        "timedelta : datetime.timedelta\n"
        "    The step size as a datetime.timedelta object.\n\n"
        "Returns:\n"
        "list of DateTime objects\n"
        "    A list containing DateTime objects evenly spaced between the two specified DateTime points with the specified step size."
    },
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef tanc_module = {
    PyModuleDef_HEAD_INIT,
    "tanc",
    "tanc module",
    -1,
    datetime_methods
};

PyMODINIT_FUNC PyInit_tanc(void) {
    PyDateTime_IMPORT;
    import_array();

    PyObject* m;
    if (PyType_Ready(&PyDateTimeType) < 0)
        return NULL;

    m = PyModule_Create(&tanc_module);
    if (m == NULL)
        return NULL;

    Py_INCREF(&PyDateTimeType);
    if (PyModule_AddObject(m, "DateTime", (PyObject*)&PyDateTimeType) < 0) {
        Py_DECREF(&PyDateTimeType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}
