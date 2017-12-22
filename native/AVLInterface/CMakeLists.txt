cmake_minimum_required(VERSION 3.8)
project(AVLInterface)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../../../jni-lib)
set(SOURCE_FILES DataHelper.h avl.c avl.h avl_def.h jni.h jni_md.h DataHelper.cpp)
add_library(avl-lib SHARED ${SOURCE_FILES})

