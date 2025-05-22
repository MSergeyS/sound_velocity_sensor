cmake_minimum_required(VERSION 3.10)

project(hydra-svm-math)

set(cmake_c_compiler C:/dev/msys64/mingw64/bin/gcc.exe)

FILE (GLOB ALL_SOURCES "source/*.c*")
FILE (GLOB ALL_INCLUDES "include/*.h")

# добавляем цель - dll
add_library(hydra-svm-math SHARED ${ALL_SOURCES} ${ALL_INCLUDES})

# Также нужно добавить как include-путь директорию
target_include_directories(hydra-svm-math PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                                                 $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>)
