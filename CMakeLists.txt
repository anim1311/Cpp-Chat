cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)
project(CppNetworking)



add_executable(CppNetworking main.cpp src/Server.cpp)

