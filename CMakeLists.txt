cmake_minimum_required(VERSION 3.8)

if(${CMAKE_VERSION} VERSION_LESS 3.15)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
  cmake_policy(VERSION 3.15)
endif()

project(CherrySoda VERSION 0.01
                   DESCRIPTION "A C++ Game Engine Based on BGFX and SDL2"
                   LANGUAGES CXX)

include(cmake/SysDetect.cmake)

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

if(EMSCRIPTEN)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 \
                                          -s WASM=1 \
                                          -s NO_EXIT_RUNTIME=1 \
                                          -s USE_WEBGL2=1 \
                                          -s FULL_ES3=1 \
                                          -s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=1 \
                                          -s OFFSCREENCANVAS_SUPPORT=1")
  set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -O0 -g4 -s ASSERTIONS=2 -s GL_ASSERTIONS=2")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Oz -s ASSERTIONS=0 -s GL_ASSERTIONS=0 --closure 1") 
  # set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Oz -s ASSERTIONS=0 -s GL_ASSERTIONS=0 \
  #                                                         --closure 1 -s WASM_OBJECT_FILES=0 --llvm-lto 1 \
  #                                                         -s AGGRESSIVE_VARIABLE_ELIMINATION=1") 
  add_compile_options(-fno-exceptions -fno-rtti)
else()
  if(LINUX)
    set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -O0 -g")
    set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_PROFILE} -O1 -g -pg")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3") 
  endif()
endif()

add_subdirectory(External)
add_subdirectory(Engine)
add_subdirectory(Playground)
add_subdirectory(Examples)