#pragma once
#include <auralis/capture/config.hpp>
#include <string>

namespace auralis::capture {

class ConfigManager {
public:
    static AudioConfig load(const std::string& path);
    static void save(const std::string& path, const AudioConfig& cfg);
    static std::string default_config_path();
};

} // namespace auralis::capture
