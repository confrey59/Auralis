cmake_minimum_required(VERSION 3.14)
project(AuralisPythonBinding LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(pybind11 REQUIRED)
find_package(AuralisCapture REQUIRED)

pybind11_add_module(auralis_capture binding.cpp)
target_link_libraries(auralis_capture PRIVATE Auralis::auralis_capture)
