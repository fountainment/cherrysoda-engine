cmake_minimum_required(VERSION 3.12)
project(tinyxml2 VERSION 9.0.0)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

add_library(tinyxml2 STATIC tinyxml2.cpp tinyxml2.h)

target_compile_features(tinyxml2 PUBLIC cxx_std_14)
target_include_directories(tinyxml2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(
    tinyxml2
    PUBLIC $<$<CONFIG:Debug>:TINYXML2_DEBUG>
    PRIVATE $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
)

set_target_properties(
    tinyxml2
    PROPERTIES
    DEFINE_SYMBOL "TINYXML2_EXPORT"
    VERSION "${tinyxml2_VERSION}"
    SOVERSION "${tinyxml2_VERSION_MAJOR}"
)
