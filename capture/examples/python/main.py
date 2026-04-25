import auralis_capture

print("[Python] Auralis Capture Test (via pybind11)")
cfg = auralis_capture.AudioConfig()
cfg.validate()
print(f"[Python] Config: SR={cfg.sample_rate}, Ch={cfg.channels}, Buf={cfg.buffer_size}")

devices = auralis_capture.get_devices()
print(f"[Python] Devices found: {len(devices)}")
for name, desc in devices:
    print(f"  - {name}: {desc}")

print("[Python] ✅ Integration successful.")
