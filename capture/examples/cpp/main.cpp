#include <auralis/capture/config.hpp>
#include <auralis/capture/device_manager.hpp>
#include <iostream>

int main() {
    std::cout << "[C++] Auralis Capture Example\n";
    
    auralis::capture::AudioConfig cfg;
    cfg.validate();
    std::cout << "[C++] Config: SR=" << cfg.sample_rate 
              << " | Ch=" << cfg.channels 
              << " | Buf=" << cfg.buffer_size << "\n";
    
    auto devs = auralis::capture::DeviceManager::scan_devices();
    std::cout << "[C++] Devices found: " << devs.size() << "\n";
    if (!devs.empty()) {
        std::cout << "[C++] First: " << devs[0].name << " (" << devs[0].description << ")\n";
    }
    
    std::cout << "[C++] ✅ Integration successful.\n";
    return 0;
}
