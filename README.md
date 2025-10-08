# Real-Time Edge Detection Viewer

This Android application is a solution for the "Android + OpenCV-C++ + OpenGL Assessment". It performs real-time edge detection on a live camera feed by leveraging native C++ for processing and OpenGL for high-performance rendering. The project demonstrates a robust integration of the Android SDK with the NDK, JNI, OpenCV, and OpenGL ES.

## Demo

Here is the application running and processing a live camera feed to detect edges.

<img width="230" height="700" alt="image" src="https://github.com/user-attachments/assets/551fcbc0-489c-43ee-965d-8bdcbab9ca19" />


---
## ✅ Features Implemented

* **Real-time Camera Processing**: Captures a high-framerate video stream using Android's **CameraX** library.
* **Native C++ Performance**: All image processing is offloaded to native C++ via **JNI** for maximum performance and efficiency.
* **Canny Edge Detection**: Uses the **OpenCV** library in C++ to apply a Canny edge detection filter to each frame.
* **High-Performance Rendering**: Displays the final processed video feed using **OpenGL ES 2.0**, drawing the output as a texture for smooth, real-time performance.
* **Screen Rotation Handling**: Correctly handles device orientation changes by rotating the camera frames within the native C++ layer.
* **Standalone Web Viewer**: A minimal **TypeScript** web page is included to demonstrate the ability to display a processed frame in a web environment.

---
## 🔧 Tech Stack

* **Android**: Android SDK, Kotlin, CameraX
* **Native**: C++, Android NDK, JNI
* **Processing**: OpenCV 4.x
* **Rendering**: OpenGL ES 2.0
* **Web**: TypeScript, HTML

---
## 🧠 Architecture and Data Flow

The application is designed to be efficient by keeping I/O and UI logic in the Kotlin layer while offloading all heavy computation to the native C++ layer.

The data flows through the app in the following sequence:

1.  **Camera Capture (Kotlin)**: The `MainActivity` uses the **CameraX** API to capture frames. The `ImageAnalysis` use case provides frames as YUV `ImageProxy` objects on a dedicated background thread.
2.  **JNI Bridge (Kotlin -> C++)**: For each frame, the raw YUV byte buffers and the current device rotation (0, 90, 270 degrees) are passed from `MainActivity.kt` to the native C++ layer through functions defined in `JNIBridge.kt`.
3.  **Native Processing (C++ & OpenCV)**: Inside `native-lib.cpp`, the YUV data is converted into an OpenCV `cv::Mat` object. This `Mat` is then **rotated** according to the device orientation data to ensure the image is upright. Finally, the Canny edge detection algorithm is applied to the corrected frame.
4.  **OpenGL ES Rendering (C++ & GL)**: The final processed `Mat` (containing only the white edges on a black background) is uploaded to an OpenGL ES 2.0 texture. This texture is then rendered onto a simple quad that fills the screen. This entire render loop is managed within the native C++ code for maximum performance.

---
## 📂 Project Structure

The project follows a modular structure to separate concerns.
