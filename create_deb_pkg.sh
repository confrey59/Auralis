#!/bin/bash
set -e
cd ~/AI/Auralis
mkdir -p debian/source

# 1. control
cat > debian/control << 'CTRL'
Source: auralis-capture
Section: libs
Priority: optional
Maintainer: Dario <dario@example.com>
Build-Depends: debhelper-compat (= 13), cmake, libasound2-dev, nlohmann-json3-dev, pkg-config
Standards-Version: 4.6.2
Homepage: https://github.com/YOUR_USERNAME/Auralis

Package: libauralis-capture-dev
Architecture: any
Depends: ${shlibs:Depends}, ${misc:Depends}, libasound2, nlohmann-json3-dev
Description: C++17 library for professional audio capture on Linux
 Auralis Capture provides a modern, OOP abstraction layer for ALSA audio
 capture. Features include JSON configuration, real-time safe callbacks,
 built-in VAD, and support for multiple audio formats.
CTRL

# 2. rules (con TAB reale obbligatorio per make)
printf '#!/usr/bin/make -f\n%%:\n\tdh $@ --buildsystem=cmake\n' > debian/rules
chmod +x debian/rules

# 3. changelog (data generata automaticamente)
DATE=$(date -R)
cat > debian/changelog << CHANGE
auralis-capture (0.2.0-1) unstable; urgency=medium

  * Initial release: Auralis Capture library
  * C++17 ALSA backend, JSON config, VAD integration
  * Google Test suite & CMake export targets

 -- Dario <dario@example.com>  $DATE
CHANGE

# 4. copyright (DEP5 standard)
cat > debian/copyright << 'COPY'
Format: https://www.debian.org/doc/packaging-manuals/copyright-format/1.0/
Upstream-Name: Auralis
Upstream-Contact: Dario <dario@example.com>
Source: https://github.com/YOUR_USERNAME/Auralis

Files: *
Copyright: 2026 Dario
License: MIT

License: MIT
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 .
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 .
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
COPY

# 5. source format
echo "3.0 (native)" > debian/source/format

echo "✅ Struttura Debian generata in ~/AI/Auralis/debian/"
