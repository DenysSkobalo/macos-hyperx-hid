# HyperX UserHID Daemon for macOS

This project is a user-space daemon written in C for macOS. It listens to raw HID events from the HyperX Pulsefire Haste 2 gaming mouse and decodes proprietary messages such as DPI changes, RGB control, and button usage.

**Goals**
- Understand HID protocol internals
- Communicate with HyperX gaming mouse without official software
- Read and decode raw HID reports
- Provide hooks for future DPI/RGB/macros support
- Be fully native and lightweight (C + IOKit)

**Target Device**
- HyperX Pulsefire Haste 2 Gaming Mouse (Regulatory Mode)
- Vendor ID: `None`
- Product ID: `None` (example – you will need to confirm it)

**Platform**
- macOS 11+ (Tested on macOS 14 Sonoma, Apple Silicon M1)
- Built with `clang`, using `IOKit` HID APIs

**Status**
> 🚧 Work In Progress — intended as an educational project

---
