set(CHERRYSODA_EXECUTABLE_SRC
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

set(CHERRYSODA_SHADER_NAMES "mypbr;skybox")

add_cherrysoda_executable(VoxelExperiment)
