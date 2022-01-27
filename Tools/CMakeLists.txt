add_subdirectory(crunch)

cherrysoda_set_targets_folder("libcrunch" Tools/crunch)
if(NOT EMSCRIPTEN)
  cherrysoda_set_targets_folder("crunch" Tools/crunch)
endif()

set(CHERRYSODA_EXECUTABLE_FOLDER Tools)
add_subdirectory(sfxr_port)
add_subdirectory(ParticleEditor)
