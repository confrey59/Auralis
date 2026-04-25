#include <auralis/capture/stream.hpp>
#include <auralis/capture/device_manager.hpp>
#include <cmath>
#include <iostream>
#include <chrono>
#include <map>
#include <cstring>
#include <algorithm>

namespace auralis::capture {

static const std::map<std::string, snd_pcm_format_t> FORMAT_MAP = {
    {"S16_LE", SND_PCM_FORMAT_S16_LE},
    {"S24_3LE", SND_PCM_FORMAT_S24_3LE},
    {"S32_LE", SND_PCM_FORMAT_S32_LE},
    {"FLOAT_LE", SND_PCM_FORMAT_FLOAT_LE},
    {"FLOAT64_LE", SND_PCM_FORMAT_FLOAT64_LE}
};

static void convert_to_float(const void* raw, size_t total_samples, snd_pcm_format_t fmt, std::vector<float>& out) {
    out.resize(total_samples);
    switch (fmt) {
        case SND_PCM_FORMAT_S16_LE:
            for (size_t i = 0; i < total_samples; ++i)
                out[i] = static_cast<float>(reinterpret_cast<const int16_t*>(raw)[i]) / 32768.0f;
            break;
        case SND_PCM_FORMAT_S32_LE:
            for (size_t i = 0; i < total_samples; ++i)
                out[i] = static_cast<float>(reinterpret_cast<const int32_t*>(raw)[i]) / 2147483648.0f;
            break;
        case SND_PCM_FORMAT_FLOAT_LE:
            std::memcpy(out.data(), raw, total_samples * sizeof(float));
            break;
        case SND_PCM_FORMAT_FLOAT64_LE:
            std::transform(reinterpret_cast<const double*>(raw), 
                           reinterpret_cast<const double*>(raw) + total_samples, 
                           out.begin(), [](double d){ return static_cast<float>(d); });
            break;
        default:
            std::fill(out.begin(), out.end(), 0.0f);
    }
}

Stream::Stream(const AudioConfig& config) : config_(config) {
    config_.validate();

    int err = snd_pcm_open(&handle_, config_.device_name.c_str(), SND_PCM_STREAM_CAPTURE, SND_PCM_NONBLOCK);
    if (err < 0) throw std::runtime_error("Cannot open device '" + config_.device_name + "': " + snd_strerror(err));

    auto it = FORMAT_MAP.find(config_.format);
    snd_pcm_format_t format = (it != FORMAT_MAP.end()) ? it->second : SND_PCM_FORMAT_FLOAT_LE;

    err = snd_pcm_set_params(handle_, format,
                             SND_PCM_ACCESS_RW_INTERLEAVED,
                             config_.channels,
                             config_.sample_rate,
                             config_.allow_resample ? 1 : 0,
                             config_.buffer_size);
    if (err < 0) {
        snd_pcm_close(handle_);
        throw std::runtime_error("Cannot set params: " + std::string(snd_strerror(err)));
    }

    raw_buffer_.resize(config_.buffer_size * config_.channels * 4); // 4 byte max per sample
    std::clog << "[Auralis Capture] Device: [" << config_.device_name << "] @ " << config_.sample_rate
              << "Hz | Format: " << config_.format << " | Ch: " << config_.channels << "\n";
}

Stream::~Stream() {
    stop();
    if (capture_thread_.joinable()) capture_thread_.join();
    if (handle_) { snd_pcm_drain(handle_); snd_pcm_close(handle_); }
}

void Stream::start() {
    if (running_) return;
    running_ = true;
    snd_pcm_prepare(handle_);
    capture_thread_ = std::thread(&Stream::capture_loop, this);
}

void Stream::stop() {
    if (!running_) return;
    running_ = false;
    if (capture_thread_.joinable()) capture_thread_.join();
}

bool Stream::is_running() const { return running_; }

void Stream::set_callback(Callback cb) { user_callback_ = std::move(cb); }

float Stream::compute_rms(const float* data, size_t frames) {
    if (frames == 0 || !data) return 0.0f;
    double sum = 0.0;
    for (size_t i = 0; i < frames; ++i) {
        float v = data[i];
        if (std::isnan(v) || std::isinf(v)) v = 0.0f;
        sum += static_cast<double>(v) * v;
    }
    return static_cast<float>(std::sqrt(sum / frames));
}

void Stream::capture_loop() {
    auto it = FORMAT_MAP.find(config_.format);
    snd_pcm_format_t fmt = (it != FORMAT_MAP.end()) ? it->second : SND_PCM_FORMAT_FLOAT_LE;
    size_t sample_bytes = snd_pcm_format_width(fmt) / 8;
    std::vector<float> float_buffer;

    while (running_) {
        snd_pcm_sframes_t frames = snd_pcm_readi(handle_, raw_buffer_.data(), config_.buffer_size);
        
        if (frames == -EPIPE) { snd_pcm_prepare(handle_); continue; }
        if (frames == -EAGAIN) { std::this_thread::sleep_for(std::chrono::milliseconds(5)); continue; }
        if (frames < 0) {
            std::cerr << "[ALSA] " << snd_strerror(static_cast<int>(frames)) << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        if (user_callback_ && frames > 0) {
            size_t total_samples = static_cast<size_t>(frames) * config_.channels;
            convert_to_float(raw_buffer_.data(), total_samples, fmt, float_buffer);
            float rms = compute_rms(float_buffer.data(), total_samples);
            user_callback_(float_buffer.data(), total_samples, rms > config_.vad_threshold);
        }
    }
}

} // namespace auralis::capture
