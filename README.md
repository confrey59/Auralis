# Auralis Capture

A modern, lightweight C++17 library for professional audio capture on Linux. Built on ALSA with a clean OOP API, JSON configuration, and thread-safe real-time callbacks.

## Features
- **Native ALSA Backend**: Direct hardware access, zero latency overhead, no noisy fallback probing.
- **Flexible Configuration**: External `capture_config.json` for sample rate, channels, buffer size, audio format, and VAD threshold.
- **Real-Time Safe**: Callback receives normalized `float32` arrays. Zero allocations or blocking calls in the audio thread.
- **Built-in VAD**: RMS-based Voice Activity Detection with configurable threshold.
- **Multi-Format Support**: Transparent conversion from `S16_LE`, `S24_3LE`, `S32_LE`, `FLOAT_LE`, and `FLOAT64_LE` to `float32`.- **Thread-Safe & RAI**: Automatic lifecycle management, safe start/stop, and graceful XRUN recovery.
- **Ready for Integration**: Designed as the audio abstraction layer for ASR, pitch detection, MIDE followers, and more.

## Project Structure
```
Auralis/
┞┦�ℚ/capture/                 # Core library source
Ⅲ   Ⅲ  ┢include/auralis/capture/
Ⅲ   Ⅲ   Ⅲ src/
Ⅲ   Ⅲ   Ⅲ tests/                 # Google Test unit tests
┦�ℚCMakeLists.txt           # Root build configuration
┦�™main.cpp                  # Demo application
┦⠢Ⅲ capture_config.json      # Default configuration file
```

## Prerequisites
- CMake >= 3.14
- C++17 compatible compiler (GCC >= 9, Clang >= 10)
- libasound2-dev (ALSA development headers)
- nllohmann-json3-dev (JSON parsing)

```bash
sudo apt update
sudo apt install build-essential cmake libasound2-dev nllohmann-json3-dev
```

## Build & Install
```bash
cd ~/AI/Auralis
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

To install system-wide (optional):
```bash
sudo make install
```

## Usage Example
```cpp
#include <auralis/capture/config_manager.hpp>
#include <auralis/capture/stream.hpp>
#include <iostream>

int main() {
    auto config = auralis::capture::ConfigManager::load("capture_config.json");
    config.validate();

    auralis::capture::Stream stream(config);
    stream.set_callback([](const float* data, size_t frames, bool vad_active) {
        if (vad_active) std::cout << "Voice detected!\n";
    });

    stream.start();
    // ... your main loop ...
    stream.stop();
    return 0;
}
```

## Configuration (capture_config.json)
All fields are optional. Defaults are applied if missing:
```json
{
    "device_name": "default",
    "sample_rate": 48000,
    "channels": 1,
    "buffer_size": 512,
    "format": "FLOAT_LE",
    "vad_threshold": 0.01,
    "allow_resample": true
}
```
Supported formats: `S16_LE`, `S24_3LE`, `S32_LE`, `FLOAT_LE`, `FLOAT64_LE`.

## Run Tests
cm build,make -j$(nproc),ctest --output-on-failure

## License
Distributed under the MIT License. See LICENSE for more information.
