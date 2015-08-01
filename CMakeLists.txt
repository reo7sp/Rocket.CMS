cmake_minimum_required(VERSION 3.0.0)
project(rocketcms)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g -fpermissive")
set(CMAKE_FIND_LIBRARY_PREFIXES lib cyg)
set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .dll .so .a .dll.a .30.dll.a .30.dll)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(cpp)
