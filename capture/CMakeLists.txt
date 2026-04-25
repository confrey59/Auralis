cmake_minimum_required(VERSION 3.14)
project(auralis_capture VERSION 0.2.0 LANGUAGES CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ALSA REQUIRED alsa)
find_package(nlohmann_json REQUIRED)

add_library(auralis_capture SHARED
    src/config.cpp
    src/config_manager.cpp
    src/device_manager.cpp
    src/stream.cpp
)

target_include_directories(auralis_capture PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(auralis_capture SYSTEM PUBLIC ${ALSA_INCLUDE_DIRS})
target_link_libraries(auralis_capture PRIVATE ${ALSA_LIBRARIES} nlohmann_json::nlohmann_json pthread)

add_executable(capture_test ../main.cpp)
target_link_libraries(capture_test PRIVATE auralis_capture)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
set_target_properties(auralis_capture PROPERTIES VERSION 0.2.0 SOVERSION 0)
