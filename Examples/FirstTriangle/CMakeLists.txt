set(CHERRYSODA_EXECUTABLE_SRC
  src/Program.cpp
  src/Program.h
  src/FirstTriangle.cpp
  src/FirstTriangle.h
)

set(CHERRYSODA_SHADER_SRC
  shaders/vs_basic.sc
  shaders/fs_basic.sc
)

add_cherrysoda_executable(FirstTriangle)
