set(PROJECT_SRC
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

if (EMSCRIPTEN)
  file(COPY index.html DESTINATION .)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s TOTAL_MEMORY=512MB --preload-file assets")
endif()
file(COPY assets DESTINATION .)
add_executable(VoxelExperiment ${PROJECT_SRC})
target_include_directories(VoxelExperiment PRIVATE .)
target_link_libraries(VoxelExperiment PRIVATE CherrySoda)