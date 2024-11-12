cmake_minimum_required(VERSION 3.10)

project(hydra_svm_math CXX)
set(CMAKE_CXX_STANDARD 17)

FILE (GLOB ALL_SOURCES "source/*.cpp")
FILE (GLOB ALL_INCLUDES "include/*.h")

# добавляем цель - dll
add_library(hydra_svm_math SHARED ${ALL_SOURCES} ${ALL_INCLUDES})

# Также нужно добавить как include-путь директорию
target_include_directories(hydra_svm_math PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                                                 $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>)
