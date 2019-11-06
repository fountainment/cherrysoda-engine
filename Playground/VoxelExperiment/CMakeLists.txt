set(PROJECT_SRC
  Program.cpp
  VoxelExperiment.cpp
  VoxelExperiment.h
  Scenes/MainScene.h
  Scenes/MainScene.cpp
)

add_executable(VoxelExperiment ${PROJECT_SRC})
target_include_directories(VoxelExperiment PRIVATE .)
target_link_libraries(VoxelExperiment PRIVATE CherrySoda)