# Sets the minimum version of CMake required to build the native library.
cmake_minimum_required(VERSION 3.10)

# Declares the project name.
project("realtimedetector")

# IMPORTANT: Set this path to where your OpenCV Android SDK is located
set(OpenCV_DIR C:/SDKs/OpenCV-android-sdk/sdk/native/jni) # <-- CHANGE THIS PATH
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_library(
        realtimedetector # Sets the name of the library.
        SHARED           # Sets the library as a shared library.
        native-lib.cpp)  # Provides a relative path to your source file(s).

target_link_libraries(
        realtimedetector
        ${OpenCV_LIBS}
        android
        EGL
        GLESv2
        log)