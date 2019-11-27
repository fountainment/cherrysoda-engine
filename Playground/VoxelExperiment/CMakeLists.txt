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

configure_file(fs_mypbr.bin ${PROJECT_BINARY_DIR}/fs_mypbr.bin COPYONLY)
configure_file(vs_mypbr.bin ${PROJECT_BINARY_DIR}/vs_mypbr.bin COPYONLY)
configure_file(fs_mypbr.bin fs_mypbr.bin COPYONLY)
configure_file(vs_mypbr.bin vs_mypbr.bin COPYONLY)
add_executable(VoxelExperiment ${PROJECT_SRC})
target_include_directories(VoxelExperiment PRIVATE .)
target_link_libraries(VoxelExperiment PRIVATE CherrySoda)
