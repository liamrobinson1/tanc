#include <Python.h>
#include <iostream>
#include <datetime.h>  // Include the Python datetime API
#include <numpy/arrayobject.h> // and numpy
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>  // If you're using Eigen types

namespace py = pybind11;
using namespace std;

#include "time.hpp"
#include "profile.hpp"

PYBIND11_MODULE(tanc, m) {
    m.doc() = "tanc module";  // Optional module docstring
    m.def("linspace", &datetime_linspace, "Generate n evenly spaced DateTime objects between two specified DateTime points.");
    m.def("arange", &datetime_arange, "Generate DateTime objects between two specified DateTime points with a specified step size.");

    py::class_<DateTime>(m, "DateTime")
        .def(py::init<int, int, int, int, int, int, int>(), 
             py::arg("year"), py::arg("month"), py::arg("day"), 
             py::arg("hour")=0, py::arg("minute")=0, py::arg("second")=0, 
             py::arg("nanosecond")=0
             )
        .def_readwrite("year", &DateTime::year)
        .def_readwrite("month", &DateTime::month)
        .def_readwrite("day", &DateTime::day)
        .def_readwrite("hour", &DateTime::hour)
        .def_readwrite("minute", &DateTime::minute)
        .def_readwrite("second", &DateTime::second)
        .def_readwrite("nanosecond", &DateTime::nanosecond)
        .def_readwrite("px", &DateTime::px)
        .def_readwrite("py", &DateTime::py)
        .def_readwrite("gmst", &DateTime::gmst)
        .def_readwrite("gast", &DateTime::gast)
        .def("__repr__", [](const DateTime &dt) {
            return "<DateTime: " + std::to_string(dt.year) + "-" + std::to_string(dt.month) + "-" + std::to_string(dt.day) + " " + std::to_string(dt.hour) + ":" + std::to_string(dt.minute) + ":" + std::to_string(dt.second) + "." + std::to_string(dt.nanosecond) + ">";
        })
        .def("__str__", [](const DateTime &dt) {
            return std::to_string(dt.year) + "-" + std::to_string(dt.month) + "-" + std::to_string(dt.day) + " " + std::to_string(dt.hour) + ":" + std::to_string(dt.minute) + ":" + std::to_string(dt.second) + "." + std::to_string(dt.nanosecond);
        })
        .def("__sub__", [](DateTime &dt1, DateTime &dt2) {
            return dt1 - dt2;
        })
        .def("__add__", [](DateTime &dt1, TimeDelta &td2) {
            return dt1 + td2;
        })
        .def_readwrite("jd_utc", &DateTime::jd_utc)
        .def_readwrite("jd_ut1", &DateTime::jd_ut1)
        .def_readwrite("jd_tai", &DateTime::jd_tai)
        .def_readwrite("jd_tt", &DateTime::jd_tt)
        .def_readwrite("gast", &DateTime::gast)
        .def("gtod_to_itrf", &DateTime::gtod_to_itrf)
        .def("teme_to_gtod", &DateTime::teme_to_gtod)
        .def("tod_to_teme", &DateTime::tod_to_teme)
        .def("mod_to_tod", &DateTime::mod_to_tod)
        .def("j2000_to_mod", &DateTime::j2000_to_mod)
        .def("itrf_to_j2000", &DateTime::itrf_to_j2000)
        ;
    
    py::class_<TimeDelta>(m, "TimeDelta")
        .def(py::init<int, int, int, int, int, int, int>(), 
             py::arg("years")=0, py::arg("months")=0, py::arg("days")=0, 
             py::arg("hours")=0, py::arg("minutes")=0, py::arg("seconds")=0, 
             py::arg("nanoseconds")=0
             )
        .def_readwrite("years", &TimeDelta::years)
        .def_readwrite("months", &TimeDelta::months)
        .def_readwrite("days", &TimeDelta::days)
        .def_readwrite("hours", &TimeDelta::hours)
        .def_readwrite("minutes", &TimeDelta::minutes)
        .def_readwrite("seconds", &TimeDelta::seconds)
        .def_readwrite("nanoseconds", &TimeDelta::nanoseconds)
        .def("__repr__", [](const TimeDelta &dt) {
            return "<TimeDelta: " + std::to_string(dt.years) + "Y " + std::to_string(dt.months) + "M " + std::to_string(dt.days) + "D " + std::to_string(dt.hours) + "h " + std::to_string(dt.minutes) + "m " + std::to_string(dt.seconds) + "s " + std::to_string(dt.nanoseconds) + "ns>";
        })
        .def("__str__", [](const TimeDelta &dt) {
            return std::to_string(dt.years) + "Y " + std::to_string(dt.months) + "M " + std::to_string(dt.days) + "D " + std::to_string(dt.hours) + "h " + std::to_string(dt.minutes) + "m " + std::to_string(dt.seconds) + "s " + std::to_string(dt.nanoseconds) + "ns";
        })
        ;

}