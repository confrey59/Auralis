# Installation Guide

This guide covers building and installing Auralis Capture from source on Linux.

## Prerequisites
Ensure your system has the following packages installed:
```bash
sudo apt update
sudo apt install build-essential cmake pkg-config libasound2-dev nlohmann-json3-dev
```
*(For non-Debian systems, install the equivalent `alsa-lib`, `nlohmann-json`, and `cmake` packages.)*

## Build from Source
```bash
git clone https://github.com/YOUR_USERNAME/Auralis.git
cd Auralis
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```
The compiled library (`libauralis_capture.a`) and test executable will be in `build/capture/`.

## System Installation
To install the library, headers, and CMake config files system-wide:
```bash
sudo make install
sudo ldconfig
```
This installs files to:
- Headers: `/usr/local/include/auralis/capture/`
- Library: `/usr/local/lib/libauralis_capture.a`
- CMake Config: `/usr/local/lib/cmake/AuralisCapture/`

## Using in Your CMake Project
After installation, link Auralis Capture in your own `CMakeLists.txt`:
```cmake
find_package(AuralisCapture REQUIRED)
add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE Auralis::capture)
```
Compile normally with `cmake .. && make`.

## Running Tests
```bash
cd build
ctest --output-on-failure
```

## Troubleshooting
- **ALSA probing warnings**: Normal during initialization. They do not affect functionality.
- **Permission denied on audio**: Ensure your user is in the `audio` group: `sudo usermod -aG audio $USER && newgrp audio`
- **Missing `nlohmann/json.hpp`**: Install `nlohmann-json3-dev` (Debian/Ubuntu) or `nlohmann-json` (Arch/Fedora).
- **CMake `FetchContent` fails**: Check internet connection. Dependencies (GoogleTest) are downloaded automatically on first build.

## Uninstallation
See `UNINSTALL.md` for detailed removal instructions.