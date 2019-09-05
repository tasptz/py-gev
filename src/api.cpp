#include <pybind11/pybind11.h>

#include "camera.h"

namespace py = pybind11;

PYBIND11_MODULE(pygev, m) {
    py::class_<Camera>(m, "Camera")
        .def(py::init<const Camera::CameraInfo>())
        .def_static("list_devices", &Camera::list_devices)
        .def("__enter__", [] (Camera& c) -> Camera& { c.open(); return c;})
        .def("__exit__", [] (Camera& c, py::object exc_type, py::object exc_value, py::object traceback) { c.close();})
        .def("start_grab", &Camera::startGrab)
        .def("stop_grab", &Camera::stopGrab)
        .def("recv", &Camera::recv, py::arg("wait") = false)
        .def_property_readonly("properties", &Camera::properties)
        .def_static("initialize", &Camera::initialize)
        .def_static("uninitialize", &Camera::uninitialize);
}
