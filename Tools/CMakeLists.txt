add_subdirectory(crunch)

cherrysoda_set_targets_folder("libcrunch" Tools)
if(NOT EMSCRIPTEN)
  cherrysoda_set_targets_folder("crunch" Tools)
endif()

add_subdirectory(sfxr_port)
cherrysoda_set_targets_folder("sfxr_port" Tools)
