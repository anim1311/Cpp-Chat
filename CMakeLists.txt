cmake_minimum_required(VERSION 3.20)
project(CppNetworking VERSION 1.0 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the Threads package
find_package(Threads REQUIRED)

include_directories(include)
project(CppNetworking)

add_executable(CppNetworking main.cpp src/Server.cpp src/Client.cpp)

target_link_libraries(CppNetworking PRIVATE Threads::Threads)

