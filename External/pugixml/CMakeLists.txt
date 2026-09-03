cmake_minimum_required(VERSION 3.16)
project(pugixml VERSION 1.16)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

add_library(pugixml STATIC src/pugixml.cpp)

target_compile_features(pugixml PUBLIC cxx_std_14)
target_include_directories(pugixml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# The engine builds with -fno-exceptions; keep the library consistent
target_compile_definitions(pugixml PUBLIC PUGIXML_NO_EXCEPTIONS)
