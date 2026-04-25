# Uninstallation Guide

CMake does not generate a `make uninstall` target by default. Follow these steps to safely remove Auralis Capture from your system.

## Method 1: Using install_manifest.txt (Recommended)
If you still have the `build/` directory used for installation:
```bash
cd path/to/Auralis/build
sudo xargs rm < install_manifest.txt
```
*Note: This only works if `install_manifest.txt` still exists. If you deleted the build folder, use Method 2.*

## Method 2: Manual Removal
Remove the installed headers, library, and CMake configuration files:
```bash
# Remove headers
sudo rm -rf /usr/local/include/auralis/

# Remove static library
sudo rm -f /usr/local/lib/libauralis_capture.a

# Remove CMake package config
sudo rm -rf /usr/local/lib/cmake/AuralisCapture/

# Update shared library cache (if applicable)
sudo ldconfig
```

## Method 3: If Installed via .deb Package (Future Release)
Once official `.deb` packages are published, simply run:
```bash
sudo apt remove auralis-capture
sudo apt autoremove
```

## Clean Build Artifacts
Remove the local build directory to free up disk space:
```bash
cd path/to/Auralis
rm -rf build/
```

## Verification
Confirm the library is no longer visible to CMake:
```bash
cd /tmp
mkdir test_clean && cd test_clean
echo 'cmake_minimum_required(VERSION 3.14)
project(test)
find_package(AuralisCapture REQUIRED)' > CMakeLists.txt
cmake .
# Expected: CMake Error at CMakeLists.txt:3 (find_package): Could not find a package configuration file provided by "AuralisCapture"
rm -rf /tmp/test_clean
```

If you need to reinstall later, simply follow `INSTALL.md` again.