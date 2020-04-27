set(CHERRYSODA_EXECUTABLE_SRC
  src/Program.cpp
  src/Program.h
  src/FirstSprite.cpp
  src/FirstSprite.h
)

set(CHERRYSODA_SHADER_SRC
  shaders/vs_sprite.sc
  shaders/fs_sprite.sc
)

add_cherrysoda_executable(FirstSprite)
