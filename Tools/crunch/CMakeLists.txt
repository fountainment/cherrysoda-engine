file(GLOB LIBCRUNCH_SRC crunch/*.cpp crunch/*.h crunch/*.hpp)
list(REMOVE_ITEM LIBCRUNCH_SRC ${CMAKE_CURRENT_SOURCE_DIR}/crunch/main.cpp)
add_library(libcrunch STATIC ${LIBCRUNCH_SRC})
target_include_directories(libcrunch PUBLIC .)
target_compile_features(libcrunch PRIVATE cxx_std_14)
target_compile_definitions(libcrunch PUBLIC UNICODE _UNICODE)
if(MINGW OR LINUX_GCC)
    target_compile_options(libcrunch PUBLIC -Wno-restrict)
endif()

if(NOT EMSCRIPTEN)
  add_executable(crunch crunch/main.cpp)
  target_link_libraries(crunch libcrunch)
  target_compile_features(crunch PRIVATE cxx_std_14)
  if(NOT WINDOWS)
    add_custom_command(TARGET crunch
                       POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:crunch>/crunch ${CHERRYSODA_TOOL_PATH}/bin/crunch)
  else()
    add_custom_command(TARGET crunch
                       POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:crunch>/crunch.exe ${CHERRYSODA_TOOL_PATH}/bin/crunch.exe)
  endif()
endif()
