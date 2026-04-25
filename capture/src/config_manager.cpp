#include <auralis/capture/config_manager.hpp>
#include <fstream>
#include <filesystem>
#include <iostream>

namespace auralis::capture {

std::string ConfigManager::default_config_path() {
    return "capture_config.json"; // Modificabile in ~/.config/auralis/capture.json
}

AudioConfig ConfigManager::load(const std::string& path) {
    if (!std::filesystem::exists(path)) {
        std::clog << "[Config] File non trovato. Genero default: " << path << "\n";
        AudioConfig def;
        save(path, def);
        return def;
    }

    std::ifstream ifs(path);
    if (!ifs.is_open()) throw std::runtime_error("Impossibile aprire " + path);

    nlohmann::json j;
    ifs >> j;
    AudioConfig cfg = AudioConfig::from_json(j);
    cfg.validate();
    return cfg;
}

void ConfigManager::save(const std::string& path, const AudioConfig& cfg) {
    std::ofstream ofs(path, std::ios::trunc);
    if (!ofs.is_open()) throw std::runtime_error("Impossibile scrivere " + path);
    ofs << cfg.to_json().dump(4); // 4 spazi di indentazione
}

} // namespace auralis::capture
