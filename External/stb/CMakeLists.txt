add_library(stb INTERFACE)
add_library(stb_vorbis STATIC stb_vorbis.c)
target_include_directories(stb INTERFACE .)
target_compile_features(stb_vorbis PRIVATE cxx_std_14)
