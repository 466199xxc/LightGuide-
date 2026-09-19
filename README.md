# ImageGuideViewer

一个基于 Qt5 的简易图像处理上位机，属于空间光影交互系统项目的原型探索。

主要用来验证：GUI 界面、图像读取显示、以及把耗时处理放到后台线程的流程。

## 功能

- 打开本地图片（png / jpg / bmp / gif 等）
- 显示原图、灰度化、Sobel 边缘检测
- 灰度化与边缘检测在后台线程中执行，避免卡顿界面
- 状态栏实时显示当前图片尺寸

## 环境依赖

- Qt 5.x（Widgets 模块）
- CMake 3.10+
- 支持 C++11 的编译器（如 MSVC / MinGW / GCC）

## 编译运行

```bash
mkdir build && cd build
cmake ..
make          # Windows 上使用 cmake --build .
./ImageGuideViewer
```

Windows + MinGW 示例：

```bash
mkdir build && cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
```

## 目录结构

```
ImageGuideViewer/
├── CMakeLists.txt
├── README.md
└── src/
    ├── main.cpp
    ├── MainWindow.h / .cpp        # 主窗口与界面逻辑
    └── ImageProcessor.h / .cpp    # 后台图像处理线程
```

## 说明

- `ImageProcessor` 继承自 `QThread`，重写 `run()`，通过信号 `processed()` 把结果送回主线程。
- 边缘检测使用经典的 3x3 Sobel 算子，未做任何优化，仅用于流程验证。
