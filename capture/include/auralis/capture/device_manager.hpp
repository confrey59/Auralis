#pragma once
#include <string>
#include <vector>

namespace auralis::capture {

struct DeviceInfo {
    int index;
    std::string name;      // ALSA device name (e.g. "hw:0,0")
    std::string description; // Human-readable
};

class DeviceManager {
public:
    static std::vector<DeviceInfo> scan_devices();
    static int get_default_input_index(); // Returns index of "default" or first available
};

} // namespace auralis::capture
