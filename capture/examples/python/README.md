# Auralis Capture Python Bindings

This directory contains pre-compiled and source bindings for the Auralis Capture library using `pybind11`.

## Installation

### Option 1: Pre-compiled Binary (x86_64 Linux only)
If you are on an x86_64 Linux system with Python 3.13+, you can use the pre-compiled `.so` file directly.

1. Ensure `libauralis-capture0` is installed:
   ```bash
   sudo dpkg -i libauralis-capture0_*.deb
   ```
2. Copy the shared object to your Python path or add this directory to `PYTHONPATH`:
   ```bash
   export PYTHONPATH="$PYTHONPATH:/percorso/a/capture/examples/python"
   # Or copy it:
   sudo cp auralis_capture.cpython-313-x86_64-linux-gnu.so /usr/lib/python3/dist-packages/
   ```

### Option 2: Build from Source (Recommended for other architectures/Python versions)
1. Install dependencies:
   ```bash
   sudo apt install cmake pybind11-dev python3-dev libauralis-capture-dev
   ```
2. Build the module:
   ```bash
   mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   make -j$(nproc)
   ```
3. Use the generated `.so` file in your project.

## Usage Example

Here is how to capture audio using the `Stream` class with a callback:

```python
import auralis_capture
import time

# 1. Configure audio settings
cfg = auralis_capture.AudioConfig()
cfg.sample_rate = 48000
cfg.channels = 1
cfg.buffer_size = 512
cfg.validate()

# 2. Get available devices
devices = auralis_capture.get_devices()
if not devices:
    print("No audio devices found!")
    exit(1)

print(f"Available devices: {len(devices)}")

# 3. Define the callback function
def on_audio_data(data_list, num_frames, vad_active):
    """
    Called automatically when new audio data arrives.
    
    Args:
        data_list: List of float values representing audio samples (-1.0 to 1.0).
        num_frames: Number of frames in this buffer.
        vad_active: Boolean indicating if Voice Activity Detection is active.
    """
    # Esempio: Stampa il valore RMS (volume approssimativo)
    if len(data_list) > 0:
        rms = sum(x*x for x in data_list) / len(data_list)
        volume = rms ** 0.5
        if volume > 0.01: # Filtra il silenzio
            print(f"[Audio] Volume: {volume:.3f} | Frames: {num_frames}")

# 4. Create and start the stream
try:
    stream = auralis_capture.Stream(cfg)
    stream.set_callback(on_audio_data)
    stream.start()
    
    print("Capturing audio... Press Ctrl+C to stop.")
    
    # Keep the main thread alive
    while stream.is_running():
        time.sleep(0.1)
        
except KeyboardInterrupt:
    print("\\nStopping...")
finally:
    # Always clean up
    if 'stream' in locals():
        stream.stop()
        stream.close()
```

## Notes
- The `Stream` class handles the actual audio capture in a separate thread.
- The callback runs in that background thread, so keep it lightweight.
- Make sure to call `stop()` and `close()` to release resources properly.
