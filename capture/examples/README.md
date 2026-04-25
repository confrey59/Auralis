# Auralis Capture Examples

This directory contains minimal examples demonstrating how to integrate with the Auralis Capture library.

## Prerequisites

Ensure the library is installed on your system:
```bash
sudo dpkg -i libauralis-capture0_*.deb libauralis-capture-dev_*.deb
sudo ldconfig
```

## C++ Example

Uses CMake and `find_package()`.

1. Navigate to the example directory:
   ```bash
   cd cpp
   ```
2. Configure and build:
   ```bash
   mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   make -j$(nproc)
   ```
3. Run:
   ```bash
   ./auralis_cpp_example
   ```

## Python Example

Uses a lightweight C-bridge compiled with `g++` and invoked via `ctypes`.

1. Navigate to the example directory:
   ```bash
   cd python
   ```
2. Compile the bridge shared object:
   ```bash
   g++ -shared -fPIC -o libauralis_bridge.so bridge.cpp -std=c++17 -lauralis_capture
   ```
3. Run:
   ```bash
   python3 main.py
   ```

## Notes

- The C++ example demonstrates standard CMake integration using exported targets (`Auralis::auralis_capture`).
- The Python example demonstrates dynamic loading of the shared library for scripting purposes.