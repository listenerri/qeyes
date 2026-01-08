# qeyes

[中文文档](README.zh-CN.md) | **English**

A lightweight Qt system tray utility that displays a pair of eyes following your mouse cursor.

## Features
- Real-time rendered eyes in the system tray icon with pupils tracking the mouse cursor position.
- Left-click or middle-click on the tray icon to open the menu for quick access.
- Automatically falls back to the bottom-right corner of the primary screen when tray position is unavailable.
- Refreshes at approximately 20 FPS (50ms timer interval).

## Build
Prerequisites: CMake ≥ 3.7, Qt 5 Widgets (defaults to Qt 5.15.2 msvc2019, can be modified via QTDIR in CMakeUserPresets), MSVC toolchain, and Ninja.

Build release version using CMake Presets:
```bash
cmake --preset qt-release
cmake --build --preset qt-release
```

Debug version:
```bash
cmake --preset qt-debug
cmake --build --preset qt-debug
```

The generated executable will be located in the build/release or build/debug directory.

## Running and Interaction
- After launching, the program stays in the system tray without opening any windows.
- The tray icon menu contains an Exit option to quit the program.
- The pupils follow the mouse cursor as it moves. Left/middle-click also opens the menu (same as right-click).