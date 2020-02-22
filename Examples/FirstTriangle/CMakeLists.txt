set(PROJECT_SRC
  src/Program.cpp
  src/Program.h
  src/FirstTriangle.cpp
  src/FirstTriangle.h
)

# TODO: Add shader compiling
file(COPY assets DESTINATION .)

if (EMSCRIPTEN)
  file(COPY index.html DESTINATION .)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s TOTAL_MEMORY=128MB --preload-file assets")
endif()

add_executable(FirstTriangle ${PROJECT_SRC})
target_include_directories(FirstTriangle PRIVATE src)
target_link_libraries(FirstTriangle PRIVATE CherrySoda)
