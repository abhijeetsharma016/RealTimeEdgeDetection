package com.example.realtimeedgedetection

object JNIBridge {
    init {
        System.loadLibrary("realtimedetector")
    }


    external fun initialize()


    external fun processFrame(
        width: Int, height: Int,
        yPlane: java.nio.ByteBuffer,
        uPlane: java.nio.ByteBuffer,
        vPlane: java.nio.ByteBuffer,
        yStride: Int, uvStride: Int
    )

    external fun onDrawFrame()
}