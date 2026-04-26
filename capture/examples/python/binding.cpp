#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <auralis/capture/config.hpp>
#include <auralis/capture/device_manager.hpp>
#include <auralis/capture/stream.hpp>
#include <string>
#include <vector>
#include <functional>

namespace py = pybind11;

PYBIND11_MODULE(auralis_capture, m) {
    m.doc() = "Python bindings for Auralis Capture";

    // Bind AudioConfig
    py::class_<auralis::capture::AudioConfig>(m, "AudioConfig")
        .def(py::init<>())
        .def("validate", &auralis::capture::AudioConfig::validate)
        .def_readwrite("sample_rate", &auralis::capture::AudioConfig::sample_rate)
        .def_readwrite("channels", &auralis::capture::AudioConfig::channels)
        .def_readwrite("buffer_size", &auralis::capture::AudioConfig::buffer_size);

    // Bind DeviceManager static method
    m.def("get_devices", []() {
        auto devices = auralis::capture::DeviceManager::scan_devices();
        py::list result;
        for (const auto& dev : devices) {
            result.append(py::make_tuple(dev.name, dev.description));
        }
        return result;
    }, "Returns a list of available audio devices.");

    // Bind Stream
    py::class_<auralis::capture::Stream>(m, "Stream")
        .def(py::init<const auralis::capture::AudioConfig&>(), 
             py::arg("config"))
        .def("start", &auralis::capture::Stream::start)
        .def("stop", &auralis::capture::Stream::stop)
        .def("is_running", &auralis::capture::Stream::is_running)
        .def("set_callback", [](auralis::capture::Stream& self, py::object cb) {
            // Wrapper sicuro per chiamare funzioni Python dal thread C++
            self.set_callback([cb](const float* data, size_t frames, bool vad_active) {
                // Acquisisci il lock di Python prima di chiamare il callback
                py::gil_scoped_acquire acquire;
                try {
                    // Converte il puntatore float in una lista Python
                    py::list py_data;
                    for (size_t i = 0; i < frames; ++i) {
                        py_data.append(data[i]);
                    }
                    // Chiama la funzione Python: callback(lista_dati, numero_frame, vad_attivo)
                    cb(py_data, frames, vad_active);
                } catch (const py::error_already_set& e) {
                    // Gestisce eventuali errori Python senza crashare il thread C++
                    PyErr_Print();
                }
            });
        }, py::arg("callback"), "Sets the Python callback function to receive audio data.")
        .def("close", [](auralis::capture::Stream& self) {
            // Assicuriamoci che lo stream sia fermato prima di chiudere
            if (self.is_running()) self.stop();
        });
}
