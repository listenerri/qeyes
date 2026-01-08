# qeyes

轻量级 Qt 托盘小工具，托盘图标是一双盯着鼠标的眼睛。

## 功能
- 托盘图标实时绘制两只眼睛，瞳孔会朝向当前鼠标位置移动。
- 左键或中键点击托盘图标时也会弹出菜单，方便快速退出程序。
- 失去托盘坐标时自动回退到主屏幕右下角，保证瞳孔指向逻辑正常运行。
- 约 20 FPS 的刷新频率（50ms 定时器）。

## 构建
前置条件：CMake ≥ 3.7，Qt 5 Widgets（预设默认指向 Qt 5.15.2 msvc2019，可在 CMakeUserPresets 中修改 QTDIR），合适的 MSVC 工具链和 Ninja。

使用 CMake Presets 构建发布版：
```
cmake --preset qt-release
cmake --build --preset qt-release
```

调试版本：
```
cmake --preset qt-debug
cmake --build --preset qt-debug
```

生成的可执行文件位于 build/release 或 build/debug 目录中。

## 运行与交互
- 首次启动后程序驻留系统托盘，不会打开窗口。
- 托盘图标菜单包含 Exit，可用来退出程序。
- 鼠标移动时瞳孔会朝向光标方向。左/中键点击同样可弹出菜单（与右键一致）。
