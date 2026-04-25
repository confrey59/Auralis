#pragma once
#include <auralis/capture/config.hpp>
#include <alsa/asoundlib.h>
#include <functional>
#include <atomic>
#include <thread>
#include <memory>
#include <vector>
#include <cstdint>

namespace auralis::capture {

class Stream {
public:
    using Callback = std::function<void(const float* data, size_t frames, bool vad_active)>;

    explicit Stream(const AudioConfig& config);
    ~Stream();

    void start();
    void stop();
    bool is_running() const;
    void set_callback(Callback cb);

private:
    AudioConfig config_;
    snd_pcm_t* handle_ = nullptr;
    std::thread capture_thread_;
    std::atomic<bool> running_ = false;
    Callback user_callback_;
    
    std::vector<uint8_t> raw_buffer_;   // ← AGGIUNTO: buffer grezzo ALSA
    std::vector<float> float_buffer_;   // ← AGGIUNTO: buffer float per callback

    static float compute_rms(const float* data, size_t frames);
    void capture_loop();
};

} // namespace auralis::capture
