cmake_minimum_required(VERSION 2.8)
project(StockScraper-test CXX)

find_package(GTest)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 17)
enable_testing()

add_executable(${PROJECT_NAME} test.cpp)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS}) # GTest::gtest_main

add_test(NAME runCli COMMAND ${CMAKE_BINARY_DIR}/bin/stockscraper)
add_test(NAME runCliInvalidIsin COMMAND ${CMAKE_BINARY_DIR}/bin/stockscraper quote DEADBEEF)

