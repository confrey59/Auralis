#include <pybind11/pybind11.h>
#include <auralis/capture/config.hpp>
#include <auralis/capture/device_manager.hpp>
#include <string>

namespace py = pybind11;

PYBIND11_MODULE(auralis_capture, m) {
    m.doc() = "Python bindings for Auralis Capture";

    py::class_<auralis::capture::AudioConfig>(m, "AudioConfig")
        .def(py::init<>())
        .def("validate", &auralis::capture::AudioConfig::validate)
        .def_readwrite("sample_rate", &auralis::capture::AudioConfig::sample_rate)
        .def_readwrite("channels", &auralis::capture::AudioConfig::channels)
        .def_readwrite("buffer_size", &auralis::capture::AudioConfig::buffer_size);

    m.def("get_devices", []() {
        auto devices = auralis::capture::DeviceManager::scan_devices();
        py::list result;
        for (const auto& dev : devices) {
            result.append(py::make_tuple(dev.name, dev.description));
        }
        return result;
    }, "Returns a list of available audio devices.");
}
