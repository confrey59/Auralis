#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <nlohmann/json.hpp>

namespace auralis::capture {

struct AudioConfig {
    std::string device_name = "default";
    unsigned int sample_rate = 48000;
    unsigned int channels = 1;
    unsigned int buffer_size = 512;
    std::string format = "FLOAT_LE"; // S16_LE, S24_3LE, S32_LE, FLOAT_LE, FLOAT64_LE
    float vad_threshold = 0.01f;
    bool allow_resample = true;

    void validate() const;
    nlohmann::json to_json() const;
    static AudioConfig from_json(const nlohmann::json& j);
};

// Serializer/Deserializer per nlohmann::json
inline void to_json(nlohmann::json& j, const AudioConfig& c) { j = c.to_json(); }
inline void from_json(const nlohmann::json& j, AudioConfig& c) { c = AudioConfig::from_json(j); }

} // namespace auralis::capture
