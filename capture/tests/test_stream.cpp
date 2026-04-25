#include <gtest/gtest.h>
#include <auralis/capture/stream.hpp>
#include <auralis/capture/device_manager.hpp>
#include <atomic>
#include <thread>
#include <chrono>

TEST(AudioStreamTest, StartStopAndCallbackInvocation) {
    auto devices = auralis::capture::DeviceManager::scan_devices();
    if (devices.empty()) {
        GTEST_SKIP() << "Nessun dispositivo ALSA trovato. Test saltato.";
    }

    auralis::capture::AudioConfig cfg;
    cfg.sample_rate = 48000;
    cfg.buffer_size = 512;
    cfg.validate();

    std::atomic<int> callback_count{0};
    auralis::capture::Stream stream(cfg);
    stream.set_callback([&](const float*, size_t, bool) {
        callback_count.fetch_add(1);
    });

    stream.start();
    EXPECT_TRUE(stream.is_running());
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    stream.stop();

    EXPECT_GT(callback_count.load(), 0) << "La callback non è mai stata chiamata";
    EXPECT_FALSE(stream.is_running());
}
