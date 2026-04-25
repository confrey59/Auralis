#include <auralis/capture/config.hpp>
#include <algorithm>

namespace auralis::capture {

void AudioConfig::validate() const {
    if (sample_rate < 8000 || sample_rate > 192000)
        throw std::invalid_argument("sample_rate out of range [8000-192000]");
    if (channels == 0 || channels > 8)
        throw std::invalid_argument("channels out of range [1-8]");
    if (buffer_size == 0 || (buffer_size & (buffer_size - 1)) != 0)
        throw std::invalid_argument("buffer_size must be power of 2");
    
    static const std::vector<std::string> valid = {"S16_LE", "S24_3LE", "S32_LE", "FLOAT_LE", "FLOAT64_LE"};
    if (std::find(valid.begin(), valid.end(), format) == valid.end())
        throw std::invalid_argument("format not supported. Use: S16_LE, S24_3LE, S32_LE, FLOAT_LE, FLOAT64_LE");
}

nlohmann::json AudioConfig::to_json() const {
    return {
        {"device_name", device_name},
        {"sample_rate", sample_rate},
        {"channels", channels},
        {"buffer_size", buffer_size},
        {"format", format},
        {"vad_threshold", vad_threshold},
        {"allow_resample", allow_resample}
    };
}

AudioConfig AudioConfig::from_json(const nlohmann::json& j) {
    AudioConfig c;
    if (j.contains("device_name")) c.device_name = j.at("device_name").get<std::string>();
    if (j.contains("sample_rate")) c.sample_rate = j.at("sample_rate").get<unsigned int>();
    if (j.contains("channels")) c.channels = j.at("channels").get<unsigned int>();
    if (j.contains("buffer_size")) c.buffer_size = j.at("buffer_size").get<unsigned int>();
    if (j.contains("format")) c.format = j.at("format").get<std::string>();
    if (j.contains("vad_threshold")) c.vad_threshold = j.at("vad_threshold").get<float>();
    if (j.contains("allow_resample")) c.allow_resample = j.at("allow_resample").get<bool>();
    return c;
}

} // namespace auralis::capture
