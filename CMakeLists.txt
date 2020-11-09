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
  add_compile_options(-fno-exceptions)
  # TODO: replace all dynamic_cast and close rtti in the future
  # add_compile_options(-fno-exceptions -fno-rtti)
else()
  if(LINUX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -O0 -g")
    set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_PROFILE} -O1 -g -pg -DCHERRYSODA_ENABLE_PROFILE -DTRACY_ENABLE")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3") 
  else()
    if(MSVC)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP -D_CRT_SECURE_NO_WARNINGS")
      set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /DCHERRYSODA_ENABLE_PROFILE /DTRACY_ENABLE")
    endif()
  endif()
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CHERRYSODA_PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(CHERRYSODA_ENGINE_PATH ${CHERRYSODA_PROJECT_PATH}/Engine)
set(CHERRYSODA_TOOL_PATH ${CHERRYSODA_PROJECT_PATH}/Tools)
set(CHERRYSODA_EXECUTABLE_FOLDER Default)
set(CHERRYSODA_EXECUTABLE_SRC src/main.cpp)
set(CHERRYSODA_SHADER_SRC "")

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
  set(shader_compile_stamp "")
  if(MSVC)
    if(CMAKE_CL_64)
      file(COPY ${CHERRYSODA_PROJECT_PATH}/External/SDL2-2.0.10/lib/x64/SDL2.dll DESTINATION .)
    else()
      file(COPY ${CHERRYSODA_PROJECT_PATH}/External/SDL2-2.0.10/lib/x86/SDL2.dll DESTINATION .)
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
      file(COPY index.html DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/index.html)
    else()
      execute_process(COMMAND python ${CHERRYSODA_TOOL_PATH}/generate_index_html.py ${executable_name} ./index.html
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    endif()
  endif()
  if(NOT "${shader_src}" STREQUAL "")
    if(WINDOWS OR LINUX OR EMSCRIPTEN)
      if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
        set(shader_compile_stamp ${CMAKE_CURRENT_BINARY_DIR}/shader_compile.stamp)
        add_custom_command(OUTPUT ${shader_compile_stamp}
                           COMMAND python ${CHERRYSODA_TOOL_PATH}/compile_shader.py "${CMAKE_CURRENT_SOURCE_DIR}" --file-list \"${shader_src}\"
                           COMMAND ${CMAKE_COMMAND} -E touch ${shader_compile_stamp}
                           COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders ${CMAKE_CURRENT_BINARY_DIR}/assets/shaders
                           DEPENDS ${shader_src}
                           WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
      endif()
    endif()
  endif()
  add_executable(${executable_name} ${program_src} ${shader_src} ${shader_compile_stamp})
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${program_src} ${shader_src})
  target_link_libraries(${executable_name} PRIVATE CherrySoda)
  target_include_directories(${executable_name} PRIVATE src)
  cherrysoda_set_target_folder(${executable_name} ${CHERRYSODA_EXECUTABLE_FOLDER})
endfunction(add_cherrysoda_executable)

add_subdirectory(External)
add_subdirectory(Tools)
add_subdirectory(Engine)
add_subdirectory(Playground)
add_subdirectory(Examples)
