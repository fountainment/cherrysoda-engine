set(CHERRYSODA_EXECUTABLE_SRC
  src/Program.cpp
  src/Program.h
  src/sfxr_port.cpp
  src/sfxr_port.h
  src/sfxr/sfxr.cpp
  src/sfxr/sfxr.h
)

set(CHERRYSODA_SHADER_SRC
)

add_cherrysoda_executable(sfxr_port)
if(EMSCRIPTEN)
  target_link_options(sfxr_port PRIVATE -s "EXPORTED_FUNCTIONS=['_main','_UnserializeSettingForJS']")
endif()
