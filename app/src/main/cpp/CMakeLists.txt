cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fno-rtti -fno-exceptions -Wall")
set(OPENGL_LIB GLESv3)

add_library(msp_mobile_model_viewer SHARED
        gl/gl_util.cpp
        primitive/texture.cpp
        primitive/material.cpp
        primitive/vertex.cpp
        primitive/mesh.cpp
        primitive/cube.cpp
        renderer.cpp
        renderer_es_30.cpp
        model_viewer_jni_bridge.cpp)

# Include libraries needed for gles3jni lib
target_link_libraries(msp_mobile_model_viewer
        ${OPENGL_LIB}
        android
        EGL
        log
        m)