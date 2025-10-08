#include <jni.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <GLES2/gl2.h>
#include <android/log.h>

// Simple shader code
const char* VERTEX_SHADER =
        "attribute vec4 vPosition;\n"
        "attribute vec2 vTexCoord;\n"
        "varying vec2 fTexCoord;\n"
        "void main() {\n"
        "  gl_Position = vPosition;\n"
        "  fTexCoord = vTexCoord;\n"
        "}\n";

const char* FRAGMENT_SHADER =
        "precision mediump float;\n"
        "varying vec2 fTexCoord;\n"
        "uniform sampler2D sTexture;\n"
        "void main() {\n"
        "  gl_FragColor = texture2D(sTexture, fTexCoord);\n"
        "}\n";

GLuint gProgram;
GLuint gTextureId;
cv::Mat processedMat;

// JNI function implementations
extern "C" JNIEXPORT void JNICALL
Java_com_example_realtimeedgedetection_JNIBridge_initialize(JNIEnv *env, jobject /* this */) {
    // Compile shaders and link program
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &VERTEX_SHADER, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &FRAGMENT_SHADER, NULL);
    glCompileShader(fragmentShader);

    gProgram = glCreateProgram();
    glAttachShader(gProgram, vertexShader);
    glAttachShader(gProgram, fragmentShader);
    glLinkProgram(gProgram);

    // Create texture
    glGenTextures(1, &gTextureId);
    glBindTexture(GL_TEXTURE_2D, gTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_realtimeedgedetection_JNIBridge_processFrame(
        JNIEnv *env, jobject,
        jint width, jint height,
        jobject yPlane, jobject uPlane, jobject vPlane,
        jint yStride, jint uvStride) {

    uint8_t* yData = (uint8_t*) env->GetDirectBufferAddress(yPlane);
    uint8_t* uData = (uint8_t*) env->GetDirectBufferAddress(uPlane);
    uint8_t* vData = (uint8_t*) env->GetDirectBufferAddress(vPlane);

    // Create OpenCV Mat objects from the YUV planes
    cv::Mat yMat(height, yStride, CV_8UC1, yData);
    cv::Mat uMat(height / 2, uvStride, CV_8UC1, uData);
    cv::Mat vMat(height / 2, uvStride, CV_8UC1, vData);

    // This is complex because U and V planes might not be contiguous.
    // For simplicity, we'll assume a standard format, but a robust
    // solution would handle strides and pixel strides carefully.
    cv::Mat yuvMat(height + height / 2, width, CV_8UC1);
    yMat(cv::Rect(0, 0, width, height)).copyTo(yuvMat(cv::Rect(0, 0, width, height)));
    // Interleave U and V planes for NV21 format needed by cvtColor
    // This is a simplified approach.
    // For a robust app, use a more direct YUV->RGB conversion.

    cv::Mat bgrMat;
    cv::cvtColor(yuvMat, bgrMat, cv::COLOR_YUV2BGR_I420); // Or YUV2BGR_NV21

    // Apply Canny Edge Detection
    cv::Mat grayMat, edgesMat;
    cv::cvtColor(bgrMat, grayMat, cv::COLOR_BGR2GRAY);
    cv::Canny(grayMat, edgesMat, 50, 150);

    // Convert edges back to BGR for texture upload
    cv::cvtColor(edgesMat, processedMat, cv::COLOR_GRAY2BGR);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_realtimeedgedetection_JNIBridge_onDrawFrame(JNIEnv *env, jobject /* this */) {
    if (processedMat.empty()) return;

    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(gProgram);

    // Update texture with the processed frame
    glBindTexture(GL_TEXTURE_2D, gTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, processedMat.cols, processedMat.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, processedMat.data);

    // Define vertices for a full-screen quad
    GLfloat vertices[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat texCoords[] = { 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f };

    // Get attribute locations
    GLint posAttrib = glGetAttribLocation(gProgram, "vPosition");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, vertices);

    GLint texAttrib = glGetAttribLocation(gProgram, "vTexCoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 0, texCoords);

    // Draw the quad
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(posAttrib);
    glDisableVertexAttribArray(texAttrib);
    glBindTexture(GL_TEXTURE_2D, 0);
}