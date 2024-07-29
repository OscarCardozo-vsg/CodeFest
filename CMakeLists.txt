cmake_minimum_required(VERSION 3.10)
project(CodeFest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(ImageMagick COMPONENTS Magick++ REQUIRED)
include_directories(${ImageMagick_INCLUDE_DIRS})

set(IMAGES_DIR "${CMAKE_SOURCE_DIR}/imagenes")
set(ENCRYPTED_DIR "${CMAKE_SOURCE_DIR}/encrypted")

add_definitions(-DIMAGES_DIR=\"${IMAGES_DIR}\")
add_definitions(-DENCRYPTED_DIR=\"${ENCRYPTED_DIR}\")

add_executable(CodeFest main.cpp encrypt.cpp decrypt.cpp)
target_link_libraries(CodeFest ${ImageMagick_LIBRARIES})