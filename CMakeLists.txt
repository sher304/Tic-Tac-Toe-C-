cmake_minimum_required(VERSION 3.27)
project(utpFirstProject)

set(CMAKE_CXX_STANDARD 20)

set(JAVA_HOME "/Users/esherow/Library/Java/JavaVirtualMachines/openjdk-21.0.2/Contents/Home")
include_directories(${JAVA_HOME}/include)
include_directories(${JAVA_HOME}/include/darwin)

add_library(utpFirstProject SHARED library.cpp)
