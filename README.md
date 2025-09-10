# HyperX UserHID Daemon for macOS

A user-space daemon written in **C** for macOS that listens to raw HID input reports from the **HyperX Pulsefire Haste 2** gaming mouse using **IOKit**.

This project aims to reverse-engineer proprietary HID messages for **DPI changes**, **RGB modes**, **custom buttons**, and more — without relying on official software (like HyperX NGenuity, which is not available for macOS).

---

## 🎯 Features

- 🔍 Detects and filters devices by **Vendor ID / Product ID**
- 🧠 Logs raw **HID input reports** to terminal
- 🖱 Tracks standard HID usage: movement (X/Y), clicks, scroll
- 🧩 Captures **non-standard HID usage**, such as DPI or macro events
- 🧪 Reverse engineering-friendly foundation

---

## 🛠 Requirements

- macOS 11.0+ (Intel or Apple Silicon)
- Xcode Command Line Tools (`clang`)
- Terminal permissions for input monitoring (System Settings → Privacy → Input Monitoring)

---

## 📦 Build & Run

```bash
make
./hyperxhid
```

## 🔗 References
- https://github.com/phracker/MacOSX-SDKs/tree/master/MacOSX10.9.sdk/System/Library/Frameworks/IOKit.framework/Versions/A/Headers/hid
- https://developer.apple.com/documentation/iokit
- https://developer.apple.com/library/archive/documentation/DeviceDrivers/Conceptual/HID/intro/intro.html
- https://www.usb.org/document-library/hid-usage-tables-16

