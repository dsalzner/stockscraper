cmake_minimum_required(VERSION 2.8)
project(stockscraper LANGUAGES CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -Wall")

add_library(${CMAKE_PROJECT_NAME} common.cpp CHttpsGet.cpp CStocks.cpp public/stockscraper.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} ${CONAN_LIBS})

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION lib)
