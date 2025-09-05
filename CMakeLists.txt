cmake_minimum_required(VERSION 3.16)
project(module)

find_library(VORTEX_SHARED_LIBRARY vortex_shared HINTS ${CMAKE_CURRENT_SOURCE_DIR}/lib/vortex/build/build/)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/vortex/lib/spdlog/include")

if(NOT VORTEX_SHARED_LIBRARY)
    message(FATAL_ERROR "Please build/install the vortex core before compiling modules!")
endif()

file(GLOB_RECURSE SOURCES main/*.cpp)

add_library(module SHARED ${SOURCES})
target_compile_options(module PRIVATE -Wall -Wextra)

target_link_libraries(module PRIVATE ${VORTEX_SHARED_LIBRARY})
target_include_directories(module PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/vortex/main/include/)
target_include_directories(module PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/vortex/)