add_library(stb INTERFACE)
add_library(stb_vorbis STATIC stb_vorbis.c)
target_include_directories(stb INTERFACE .)
