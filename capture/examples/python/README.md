# Auralis Capture Python Bindings

This directory contains the Python bindings for the Auralis Capture library, implemented using `pybind11`.
It allows you to capture audio directly from Python with minimal setup.

## Installation

### Option 1: Pre-compiled Binary (Recommended for x86_64 Linux)

If you are running on an **x86_64** Linux system with **Python 3.13+**, you can use the pre-compiled shared library provided in this directory.

1. Ensure the core library is installed:
   ```bash
   sudo dpkg -i libauralis-capture0_*.deb
   ```
2. Copy the binding module to your Python path:
   ```bash
   # Find the .so file in this directory
   SO_FILE=$(ls auralis_capture.cpython-313-x86_64-linux-gnu.so)
   sudo cp "$SO_FILE" /usr/lib/python3/dist-packages/
   ```
3. Verify the installation:
   ```bash
   python3 -c "import auralis_capture; print('Success!')"
   ```

### Option 2: Build from Source (For other architectures or Python versions)

If you are on ARM, macOS, or a different Python version, you must compile the binding yourself.

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
3. Use the generated `.so` file:
   The compiled module will be in `build/auralis_capture.cpython-...so`. You can copy it to your project directory or add the `build` folder to your `PYTHONPATH`.

## Usage Example

Here is a complete example of how to capture audio using the `Stream` class and a callback function.

```python
import auralis_capture
import time

def main():
    # 1. Configure audio settings
    cfg = auralis_capture.AudioConfig()
    cfg.sample_rate = 48000
    cfg.channels = 1      # Mono
    cfg.buffer_size = 512 # Frames per buffer
    cfg.validate()

    # 2. Check for available devices
    devices = auralis_capture.get_devices()
    if not devices:
        print("No audio devices found!")
        return

    print(f"Found {len(devices)} device(s).")

    # 3. Define the callback function
    def on_audio_data(data_list, num_frames, vad_active):
        """
        This function is called automatically by the C++ thread when new data arrives.
        
        Args:
            data_list (list[float]): Audio samples as floats (-1.0 to 1.0).
            num_frames (int): Number of frames in this buffer.
            vad_active (bool): Voice Activity Detection status.
        """
        # Simple volume calculation (RMS)
        if len(data_list) > 0:
            rms = sum(x*x for x in data_list) / len(data_list)
            volume = rms ** 0.5
            
            # Print only if there is significant sound
            if volume > 0.01:
                print(f"[Audio] Vol: {volume:.3f} | Frames: {num_frames}")

    # 4. Create and start the stream
    try:
        stream = auralis_capture.Stream(cfg)
        stream.set_callback(on_audio_data)
        stream.start()
        
        print("Capturing audio... Press Ctrl+C to stop.")
        
        # Keep the main thread alive while capturing
        while stream.is_running():
            time.sleep(0.1)
            
    except KeyboardInterrupt:
        print("\\nStopping...")
    finally:
        # Always clean up resources
        if 'stream' in locals():
            stream.stop()
            stream.close()

if __name__ == "__main__":
    main()
```

## Notes
- The `Stream` class runs its capture logic in a separate background thread.
- The callback function (`on_audio_data`) is executed in that background thread. Keep it lightweight to avoid blocking audio processing.
- Always call `stop()` and `close()` to release ALSA resources properly.
