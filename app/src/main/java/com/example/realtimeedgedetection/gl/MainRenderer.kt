package com.example.realtimeedgedetection.gl

import android.opengl.GLSurfaceView
import com.example.realtimeedgedetection.JNIBridge
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MainRenderer : GLSurfaceView.Renderer {
    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        // Initialize shaders, program, and textures in C++
        JNIBridge.initialize()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        // Viewport changes are handled implicitly by the full-screen quad
    }

    override fun onDrawFrame(gl: GL10?) {
        // Call the native C++ rendering function
        JNIBridge.onDrawFrame()
    }
}