cmake_minimum_required(VERSION 3.10)
project(CircularBuffer VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()



add_library(CircularBuffer CircularBuffer.c)

add_subdirectory(cpputest)
add_subdirectory(tests)

add_test(NAME allTests COMMAND circularBufferTests)
add_test(NAME multiThreadTests COMMAND circularBufferMultiThreadTests)


