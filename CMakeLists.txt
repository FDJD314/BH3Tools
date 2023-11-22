cmake_minimum_required(VERSION 3.5.0)
project(BH3Tool VERSION 0.1.0 LANGUAGES C)

set(LIBRARY_OUTPUT_PATH  ${CMAKE_SOURCE_DIR}/apps)
set(EXECUTABLE_OUTPUT_PATH  ${CMAKE_SOURCE_DIR}/apps)

add_subdirectory(src)