set(PROJECT_SRC
  Program.cpp
  VoxelExperiment.cpp
  VoxelExperiment.h
  Scenes/MainScene.cpp
  Scenes/MainScene.h
  Voxel/Block.cpp
  Voxel/Block.h
  Voxel/Chunk.cpp
  Voxel/Chunk.h
  Voxel/ChunkGraphicsComponent.cpp
  Voxel/ChunkGraphicsComponent.h
)

add_executable(VoxelExperiment ${PROJECT_SRC})
target_include_directories(VoxelExperiment PRIVATE .)
target_link_libraries(VoxelExperiment PRIVATE CherrySoda)