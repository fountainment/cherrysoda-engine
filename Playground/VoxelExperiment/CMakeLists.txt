set(PROJECT_SRC
  Program.cpp
  Program.h
  VoxelExperiment.cpp
  VoxelExperiment.h
)

add_executable(VoxelExperiment ${PROJECT_SRC})
target_include_directories(VoxelExperiment PRIVATE .)
target_link_libraries(VoxelExperiment PRIVATE CherrySoda)