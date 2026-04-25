cmake_minimum_required(VERSION 3.14)
project(Auralis VERSION 0.2.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Test gestiti dal sistema Debian
option(BUILD_TESTING "Build unit tests" ON)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(capture/tests)
endif()

add_subdirectory(capture)

# Install library and headers
install(TARGETS auralis_capture
    EXPORT AuralisCaptureTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY capture/include/auralis
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp"
)

install(EXPORT AuralisCaptureTargets
    FILE AuralisCaptureTargets.cmake
    NAMESPACE Auralis::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/AuralisCapture
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/AuralisConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/AuralisCaptureConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/AuralisCapture
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/AuralisCaptureConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/AuralisCaptureConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/AuralisCaptureConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/AuralisCapture
)
