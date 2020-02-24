set(PROJECT_EXECUTABLE_SRC
  src/Program.cpp
  src/Program.h
  src/VoxelExperiment.cpp
  src/VoxelExperiment.h
  src/Scenes/MainScene.cpp
  src/Scenes/MainScene.h
  src/Scenes/Skybox.cpp
  src/Scenes/Skybox.h
  src/Voxel/Block.cpp
  src/Voxel/Block.h
  src/Voxel/Chunk.cpp
  src/Voxel/Chunk.h
  src/Voxel/ChunkGraphicsComponent.cpp
  src/Voxel/ChunkGraphicsComponent.h
  src/Voxel/World.cpp
  src/Voxel/World.h
)

set(PROJECT_SHADER_SRC
  shaders/uniform.sh
  shaders/varying.def.sc
  shaders/vs_mypbr.sc
  shaders/fs_mypbr.sc
  shaders/vs_skybox.sc
  shaders/fs_skybox.sc
)

add_cherrysoda_executable_new(VoxelExperiment "${PROJECT_EXECUTABLE_SRC}" "${PROJECT_SHADER_SRC}")
