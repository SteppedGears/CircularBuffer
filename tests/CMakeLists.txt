cmake_minimum_required(VERSION 3.10)
project(circularBufferTests VERSION 0.1.0)


add_executable(circularBufferTests
                    AllTests.cpp
                    CircularBufferTests.cpp)  
target_link_libraries(circularBufferTests CircularBuffer CppUTest CppUTestExt)
target_link_directories(circularBufferTests PUBLIC 
                                "${PROJECT_BINARY_DIR}/.."
                                "${PROJECT_BINARY_DIR}/../cpputest")

target_include_directories(circularBufferTests PUBLIC
            "${PROJECT_SOURCE_DIR}/../cpputest/include"
            ../)