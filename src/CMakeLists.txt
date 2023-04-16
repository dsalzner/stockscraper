project(stockscraper LANGUAGES CXX)
cmake_minimum_required(VERSION 2.8)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 17)
add_executable(stockscraper main.cpp common.cpp CHttpsGet.cpp CStocks.cpp)
target_link_libraries(stockscraper ${CONAN_LIBS})
