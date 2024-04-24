cmake_minimum_required(VERSION 3.28.0)
project(multimedia LANGUAGES CXX)

if(MSVC)
    add_compile_options(/utf-8)
endif()

set(YOUR_OPENCV_INSTALL_PATH "D:/OpenCV/build" CACHE FILEPATH "Where is OpenCV library located?")
find_package(OpenCV CONFIG REQUIRED PATHS ${YOUR_OPENCV_INSTALL_PATH} NO_DEFAULT_PATH)

add_subdirectory(HW01)
add_subdirectory(HW02)
add_subdirectory(HW03)
add_subdirectory(HW04)
add_subdirectory(HW05)