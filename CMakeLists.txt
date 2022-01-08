cmake_minimum_required(VERSION 3.12)

if(${CMAKE_VERSION} VERSION_LESS 3.15)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
  cmake_policy(VERSION 3.15)
endif()

project(CherrySoda VERSION 0.2.1
                   DESCRIPTION "A C++ Game Engine Based on BGFX and SDL2"
                   LANGUAGES C CXX)

option(CHERRYSODA_BUILD_TOOLS      "Build tools"               ON)
option(CHERRYSODA_BUILD_SHADERC    "Build shaderc"             OFF)
option(CHERRYSODA_LIGHT_BUILD      "Build only necessaries"    OFF)
option(CHERRYSODA_BUILD_EXAMPLES   "Build examples"            ON)
option(CHERRYSODA_BUILD_PLAYGROUND "Build playground projects" ON)
option(CHERRYSODA_BUILD_BENCH      "Build benchmark projects"  ON)

option(ENABLE_LTO "Enable link time optimization" ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
include(Documentation)
include(LTO)
include(SysDetect)

find_lto(CXX)

find_package(Python)
if(NOT Python_FOUND)
  message(FATAL_ERROR "Please install python3 first!")
endif()

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

set(OUTSIDE_PROJECTS_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/../cherrysoda-projects)
set(OUTSIDE_PROJECTS_FOLDER_EXISTS OFF)
if(EXISTS ${OUTSIDE_PROJECTS_FOLDER}/CMakeLists.txt)
  set(OUTSIDE_PROJECTS_FOLDER_EXISTS ON)
endif()

if(OUTSIDE_PROJECTS_FOLDER_EXISTS OR ANDROID)
  set(CHERRYSODA_LIGHT_BUILD ON)
endif()

if(EMSCRIPTEN)
  set(CHERRYSODA_BUILD_SHADERC OFF)
endif()

if(ANDROID)
  set(CHERRYSODA_BUILD_TOOLS OFF)
endif()

# MIPS
if(CMAKE_SYSTEM_PROCESSOR MATCHES "(mips64)|(MIPS64)")
  add_definitions(-Umips)
endif()

# Lemote Fuloong 2.0
if(LOONGSON)
  message(STATUS "Build on Loongson3A platform")
  add_definitions(-march=loongson3a -mmsa)
endif()

# PocketCHIP
if(CHIP)
  message(STATUS "Build on PocketCHIP")
  add_definitions(-DCHIP)
  add_definitions(-march=armv7-a -mtune=cortex-a8 -ftree-vectorize -ffast-math)
endif()

# ClockworkPi
if(CPI)
  message(STATUS "Build on GameShell")
  add_definitions(-DCLOCKWORK_PI)
  add_definitions(-mcpu=cortex-a7 -ftree-vectorize -ffast-math)
endif()

if(APPLE AND NOT XCODE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ObjC++")
endif()

set(CHERRYSODA_SIMD "")
if(NOT EMSCRIPTEN)
  if(NOT MSVC)
    if(CHERRYSODA_USE_SSE2)
      message(STATUS "Enabling SSE2")
      set(CHERRYSODA_SIMD "-mfpmath=sse -msse2 -DRAPIDJSON_SSE2")
    elseif(CHERRYSODA_USE_SSE42)
      message(STATUS "Enabling SSE4.2")
      set(CHERRYSODA_SIMD "-mfpmath=sse -msse4.2 -DRAPIDJSON_SSE42")
    elseif(CHERRYSODA_USE_AVX2)
      message(STATUS "Enabling AVX2")
      set(CHERRYSODA_SIMD "-mfpmath=avx -mavx2 -mfma")
    elseif(CHERRYSODA_USE_NEON)
      message(STATUS "Enabling NEON")
      set(CHERRYSODA_SIMD "-mfpu=neon -mfloat-abi=hard -DRAPIDJSON_NEON")
    endif()
  else(NOT MSVC)
    if(CHERRYSODA_USE_SSE2 OR CHERRYSODA_USE_SSE42)
      if(NOT CMAKE_CL_64)
        set(CHERRYSODA_SIMD "/arch:SSE2")
      endif()
    endif()
    if(CHERRYSODA_USE_SSE2)
      message(STATUS "Enabling SSE2")
      set(CHERRYSODA_SIMD "${CHERRYSODA_SIMD} /DRAPIDJSON_SSE2")
    elseif(CHERRYSODA_USE_SSE42)
      message(STATUS "Enabling SSE4.2")
      set(CHERRYSODA_SIMD "${CHERRYSODA_SIMD} /D__SSE4_2__ /DRAPIDJSON_SSE42")
    elseif(CHERRYSODA_USE_AVX2)
      message(STATUS "Enabling AVX2")
      set(CHERRYSODA_SIMD "/arch:AVX2")
    endif()
  endif()
endif()

add_definitions(-D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS)
if(MSVC)
  add_definitions(-D_HAS_EXCEPTIONS=0 -D_HAS_ITERATOR_DEBUGGING=0 
                  -D_SCL_SECURE=0 -D_SECURE_SCL=0
                  -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE)
  # no-exception
  string(REGEX REPLACE "/EH[a-z]+" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHs-c-")
  # no-rtti
  string(REGEX REPLACE "/GR" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")
endif()

if(EMSCRIPTEN)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s MAX_WEBGL_VERSION=2 \
                                                        -s WASM=1 \
                                                        -s NO_EXIT_RUNTIME=1 \
                                                        -s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=1 \
                                                        -s OFFSCREENCANVAS_SUPPORT=1 \
                                                        -s ALLOW_MEMORY_GROWTH=1")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG   "${CMAKE_EXE_LINKER_FLAGS_DEBUG}   -s ASSERTIONS=2 -s GL_ASSERTIONS=2 --source-map-base=./")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s ASSERTIONS=0 -s GL_ASSERTIONS=0")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti \
                                          -s USE_SDL=2 \
                                          --preload-file assets \
                                          -Wno-unused-command-line-argument")
  set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -O0 -gsource-map -DCHERRYSODA_ENABLE_DEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Oz --closure 1") 
  # set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Oz -s ASSERTIONS=0 -s GL_ASSERTIONS=0 \
  #                                                         --closure 1 -s WASM_OBJECT_FILES=0 --llvm-lto 1 \
  #                                                         -s AGGRESSIVE_VARIABLE_ELIMINATION=1")
elseif(NOT MSVC)
  if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pie")
  endif()
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_definitions(-Wno-unneeded-internal-declaration)
  endif()
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall -pedantic -Wno-pedantic -fPIE ${CHERRYSODA_SIMD}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti -Wall -pedantic -Wno-pedantic -fPIE ${CHERRYSODA_SIMD}")
  set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -O0 -g -DCHERRYSODA_ENABLE_DEBUG")
  set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_PROFILE} -O1 -g -pg -DCHERRYSODA_ENABLE_PROFILE -DTRACY_ENABLE")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
  set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O1 -g -pg -DCHERRYSODA_ENABLE_PROFILE -DTRACY_ENABLE")
else() # MSVC
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /source-charset:utf-8 /MP ${CHERRYSODA_SIMD}")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DCHERRYSODA_ENABLE_DEBUG")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /DCHERRYSODA_ENABLE_PROFILE /DTRACY_ENABLE")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CHERRYSODA_PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(CHERRYSODA_ENGINE_PATH ${CHERRYSODA_PROJECT_PATH}/Engine)
set(CHERRYSODA_EXTERNAL_PATH ${CHERRYSODA_PROJECT_PATH}/External)
set(CHERRYSODA_TOOL_PATH ${CHERRYSODA_PROJECT_PATH}/Tools)
set(CHERRYSODA_EXECUTABLE_FOLDER Default)
set(CHERRYSODA_EXECUTABLE_SRC src/main.cpp)
set(CHERRYSODA_MODULE_LINKS "")
set(CHERRYSODA_SHADER_SRC "")
set(CHERRYSODA_SDL2_PATH ${CHERRYSODA_EXTERNAL_PATH}/SDL2-2.0.14)

set(CHERRYSODA_PREBUILT_SHADERC_AVAILABLE OFF)
if((WINDOWS OR LINUX) AND CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "(x86_64)|(X86_64)|(amd64)|(AMD64)")
  set(CHERRYSODA_PREBUILT_SHADERC_AVAILABLE ON)
elseif(EXISTS ${CHERRYSODA_TOOL_PATH}/bin/shaderc.local${CMAKE_EXECUTABLE_SUFFIX})
  set(CHERRYSODA_PREBUILT_SHADERC_AVAILABLE ON)
endif()

set(CHERRYSODA_SHADERC_EXECUTABLE)
if(CHERRYSODA_BUILD_SHADERC)
  set(CHERRYSODA_SHADERC_EXECUTABLE shaderc)
endif()

if(WINDOWS)
  if(NOT DEFINED ENV{SDL2_DIR})
    if(NOT EXISTS ${CHERRYSODA_SDL2_PATH})
      if(MSVC AND CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
        execute_process(COMMAND ${Python_EXECUTABLE} ${CHERRYSODA_TOOL_PATH}/setup_sdl2_for_windows.py
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
      endif()
    endif()
    set(ENV{SDL2_DIR} ${CHERRYSODA_SDL2_PATH})
  else()
    set(${CHERRYSODA_SDL2_PATH} ENV{SDL2_DIR})
  endif()
endif()

function(cherrysoda_set_target_folder target folder)
  set_target_properties(${target} PROPERTIES FOLDER ${folder})
endfunction(cherrysoda_set_target_folder)

function(cherrysoda_set_targets_folder targets folder)
  foreach(target ${targets})
    cherrysoda_set_target_folder(${target} ${folder})
  endforeach()
endfunction(cherrysoda_set_targets_folder)

function(add_cherrysoda_executable executable_name)
  set(program_src "${CHERRYSODA_EXECUTABLE_SRC}")
  set(shader_src "${CHERRYSODA_SHADER_SRC}")
  set(module_links "${CHERRYSODA_MODULE_LINKS}")
  set(shader_compile_stamp "")
  set(program_icon)
  if(ANDROID)
    set(executable_name main)
  endif()
  if(WINDOWS)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/icon.rc)
      set(program_icon icon.rc)
    endif()
    if(NOT EXISTS ${CHERRYSODA_SDL2_PATH})
      if(MSVC AND CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
        execute_process(COMMAND ${Python_EXECUTABLE} ${CHERRYSODA_TOOL_PATH}/setup_sdl2_for_windows.py
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
      endif()
    endif()
    if(CMAKE_CL_64)
      file(COPY ${CHERRYSODA_SDL2_PATH}/lib/x64/SDL2.dll DESTINATION .)
    else()
      file(COPY ${CHERRYSODA_SDL2_PATH}/lib/x86/SDL2.dll DESTINATION .)
    endif()
  endif()
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/assets)
    file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  endif()
  if(EMSCRIPTEN)
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/assets AND NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
      file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assets)
      file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/assets/placeholder)
    endif()
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/index.html)
      file(COPY index.html DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
    else()
      execute_process(COMMAND ${Python_EXECUTABLE} ${CHERRYSODA_TOOL_PATH}/generate_index_html.py ${executable_name} ./index.html
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    endif()
  endif()
  if(NOT "${shader_src}" STREQUAL "")
    if(CHERRYSODA_PREBUILT_SHADERC_AVAILABLE OR CHERRYSODA_BUILD_SHADERC)
      set(shader_compile_stamp ${CMAKE_CURRENT_BINARY_DIR}/shader_compile.stamp)
      add_custom_command(OUTPUT ${shader_compile_stamp}
                         COMMAND ${Python_EXECUTABLE} ${CHERRYSODA_TOOL_PATH}/compile_shader.py "${CMAKE_CURRENT_SOURCE_DIR}" --file-list \"${shader_src}\"
                         COMMAND ${CMAKE_COMMAND} -E touch ${shader_compile_stamp}
                         COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders ${CMAKE_CURRENT_BINARY_DIR}/assets/shaders
                         DEPENDS ${shader_src} ${CHERRYSODA_SHADERC_EXECUTABLE}
                         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    endif()
  endif()
  if(ANDROID)
    add_library(${executable_name} SHARED ${program_src} ${shader_src} ${shader_compile_stamp})
  else()
    add_executable(${executable_name} WIN32 ${program_src} ${shader_src} ${shader_compile_stamp} ${program_icon})
  endif()
  if(CMAKE_GENERATOR STREQUAL "Xcode")
    cmake_minimum_required(VERSION 3.17)
    # XCODE_SCHEME_WORKING_DIRECTORY requires cmake 3.17
    set_target_properties(${executable_name} PROPERTIES
        XCODE_GENERATE_SCHEME TRUE
        XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )
  endif()
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${program_src} ${shader_src})
  target_link_libraries(${executable_name} PRIVATE ${module_links} CherrySoda)
  target_include_directories(${executable_name} PRIVATE src)
  target_enable_lto(${executable_name} optimized)
  cherrysoda_set_target_folder(${executable_name} ${CHERRYSODA_EXECUTABLE_FOLDER})
endfunction(add_cherrysoda_executable)

add_subdirectory(External)
add_subdirectory(Engine)

if(CHERRYSODA_BUILD_TOOLS)
  add_subdirectory(Tools)
endif()

if(NOT CHERRYSODA_LIGHT_BUILD)
  if(CHERRYSODA_BUILD_EXAMPLES)
    add_subdirectory(Examples)
  endif()
  if(CHERRYSODA_BUILD_PLAYGROUND)
    add_subdirectory(Playground)
  endif()
  if(CHERRYSODA_BUILD_BENCH)
    add_subdirectory(Bench)
  endif()
endif()

set(CHERRYSODA_EXECUTABLE_FOLDER Projects)
if(OUTSIDE_PROJECTS_FOLDER_EXISTS)
  add_subdirectory(${OUTSIDE_PROJECTS_FOLDER} Projects)
elseif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Projects/CMakeLists.txt)
  add_subdirectory(Projects)
endif()
