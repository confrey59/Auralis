#include <auralis/capture/device_manager.hpp>
#include <alsa/asoundlib.h>
#include <vector>
#include <string>

namespace auralis::capture {

std::vector<DeviceInfo> DeviceManager::scan_devices() {
    std::vector<DeviceInfo> devices;
    void **hints, **n;
    
    if (snd_device_name_hint(-1, "capture", &hints) < 0) return devices;
    
    int idx = 0;
    for (n = hints; *n != nullptr; n++) {
        char* name = snd_device_name_get_hint(*n, "NAME");
        char* desc = snd_device_name_get_hint(*n, "DESC");
        if (name && std::string(name).find("null") == std::string::npos) {
            DeviceInfo dev;
            dev.index = idx++;
            dev.name = name;
            dev.description = desc ? desc : name;
            devices.push_back(dev);
        }
        free(name);
        free(desc);
    }
    snd_device_name_free_hint(hints);
    return devices;
}

int DeviceManager::get_default_input_index() {
    auto devs = scan_devices();
    for (size_t i = 0; i < devs.size(); ++i) {
        if (devs[i].name == "default") return static_cast<int>(i);
    }
    return devs.empty() ? -1 : 0;
}

} // namespace auralis::capture
