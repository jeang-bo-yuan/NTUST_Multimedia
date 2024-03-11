cmake_minimum_required(VERSION 3.28.0)
project(multimedia LANGUAGES CXX)

if(MSVC)
    add_compile_options(/utf-8)
endif()

find_package(OpenCV CONFIG REQUIRED PATHS "D:/OpenCV/build" NO_DEFAULT_PATH)

add_subdirectory(HW01)
add_subdirectory(HW02)