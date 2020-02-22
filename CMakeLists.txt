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
                                          -s OFFSCREENCANVAS_SUPPORT=1 \
                                          -s TOTAL_MEMORY=512MB \
                                          --preload-file assets")
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

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CHERRYSODA_PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(CHERRYSODA_EXECUTABLE_FOLDER Default)
set(CHERRYSODA_EXECUTABLE_SRC main.cpp)
set(CHERRYSODA_SHADER_NAMES "")

macro(cherrysoda_compile_shaders shader_names)
  foreach(shader_name ${shader_names})
    message("Compiling shader ${shader_name}...")
    execute_process(COMMAND python Tools/compile_shader.py ${CMAKE_CURRENT_SOURCE_DIR} ${shader_name}
                    WORKING_DIRECTORY ${CHERRYSODA_PROJECT_PATH}
                    OUTPUT_FILE compile_shader.log)
  endforeach()
endmacro(cherrysoda_compile_shaders)

function(cherrysoda_set_target_folder target folder)
  set_target_properties(${target} PROPERTIES
    FOLDER ${folder}
  )
endfunction(cherrysoda_set_target_folder)

function(cherrysoda_set_targets_folder targets folder)
  foreach(target ${targets})
    cherrysoda_set_target_folder(${target} ${folder})
  endforeach()
endfunction(cherrysoda_set_targets_folder)

macro(add_cherrysoda_executable executable_name)
  if (WINDOWS OR LINUX)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
      cherrysoda_compile_shaders("${CHERRYSODA_SHADER_NAMES}")
    endif()
  endif()
  file(COPY assets DESTINATION .)
  if (EMSCRIPTEN)
    file(COPY index.html DESTINATION .)
  endif()
  add_executable(${executable_name} ${CHERRYSODA_EXECUTABLE_SRC})
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src FILES ${CHERRYSODA_EXECUTABLE_SRC})
  target_link_libraries(${executable_name} PRIVATE CherrySoda)
  target_include_directories(${executable_name} PRIVATE src)
  cherrysoda_set_target_folder(${executable_name} ${CHERRYSODA_EXECUTABLE_FOLDER})
endmacro(add_cherrysoda_executable)

add_subdirectory(External)
add_subdirectory(Engine)
add_subdirectory(Playground)
add_subdirectory(Examples)