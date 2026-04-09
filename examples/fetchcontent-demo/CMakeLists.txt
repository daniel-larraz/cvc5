cmake_minimum_required(VERSION 3.16)

project(fetchcontent-demo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ENABLE_AUTO_DOWNLOAD ON CACHE BOOL "" FORCE)

include(FetchContent)

FetchContent_Declare(
    cvc5
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

FetchContent_MakeAvailable(cvc5)

add_executable(demo main.cpp)

target_link_libraries(demo PRIVATE cvc5)