cmake_minimum_required(VERSION 2.8)
project(stockscraper LANGUAGES CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -Wall")

add_executable(${CMAKE_PROJECT_NAME} main.cpp common.cpp CHttpsGet.cpp CStocks.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} ${CONAN_LIBS})


