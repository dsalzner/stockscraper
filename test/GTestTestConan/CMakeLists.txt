cmake_minimum_required(VERSION 2.8)
project(stockscraper-test CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -Wall")

include(GoogleTest)
include(CMakePrintHelpers)
cmake_print_variables(CONAN_LIBS)

add_executable(${CMAKE_PROJECT_NAME} test.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} ${CONAN_LIBS})

enable_testing()
gtest_add_tests(${CMAKE_PROJECT_NAME} "" AUTO)
