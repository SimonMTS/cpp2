cmake_minimum_required(VERSION 3.1.3)
project(avansSteganography)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wpedantic)#  -Werror)
    add_compile_options("$<$<CONFIG:DEBUG>:-g>")
endif()

add_library(avansSteganography STATIC
  src/main.cpp)

target_compile_features(avansSteganography PUBLIC cxx_std_17)

add_executable(aSteg
    src/main.cpp
    src/helpers/bitVector.cpp
    src/helpers/optionParser.cpp
    src/fileParsers/cafParser.cpp
    src/fileParsers/wavParser.cpp)
