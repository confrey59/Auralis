find_package(GTest REQUIRED)

add_executable(test_capture main.cpp)
target_link_libraries(test_capture PRIVATE auralis_capture GTest::gtest_main)
add_test(NAME capture_tests COMMAND test_capture)
