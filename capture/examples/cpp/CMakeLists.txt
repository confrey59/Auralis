cmake_minimum_required(VERSION 3.14)
project(AuralisCppExample LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(AuralisCapture REQUIRED)

add_executable(auralis_cpp_example main.cpp)
target_link_libraries(auralis_cpp_example PRIVATE Auralis::auralis_capture)
