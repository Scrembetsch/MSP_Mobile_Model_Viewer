
# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("msp_android_opengl_es")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions -Wall")
if (${ANDROID_PLATFORM_LEVEL} LESS 12)
    message(FATAL_ERROR "OpenGL 2 is not supported before API level 11 \
                      (currently using ${ANDROID_PLATFORM_LEVEL}).")
    return()
elseif (${ANDROID_PLATFORM_LEVEL} LESS 18)
    add_definitions("-DDYNAMIC_ES3")
    set(GL3STUB_SRC gl3stub.c)
    set(OPENGL_LIB GLESv2)
else ()
    set(OPENGL_LIB GLESv3)
endif (${ANDROID_PLATFORM_LEVEL} LESS 12)

add_library(gl_es_3x_jni SHARED
        ${GL3STUB_SRC}
        gl/gl_util.cpp
        primitive/vertex.cpp
        primitive/mesh.cpp
        primitive/cube.cpp
        jni_bridge.cpp
        renderer_es_3x.cpp
        renderer.cpp
        util.cpp
        )

# Include libraries needed for gles3jni lib
target_link_libraries(gl_es_3x_jni
        ${OPENGL_LIB}
        android
        EGL
        log
        m)
