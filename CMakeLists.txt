cmake_minimum_required(VERSION 3.10)
project(ImageGuideViewer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_executable(ImageGuideViewer
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/ImageProcessor.cpp
    src/ImageProcessor.h
)

target_link_libraries(ImageGuideViewer PRIVATE Qt5::Widgets)
