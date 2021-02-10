add_subdirectory(bgfx)
add_subdirectory(glm)
add_subdirectory(imgui)
add_subdirectory(rapidjson)

cherrysoda_set_targets_folder("bgfx;bimg;bimg_decode;bx;imgui" External)
if(NOT EMSCRIPTEN)
  cherrysoda_set_targets_folder("fcpp;glslang;spirv-cross;spirv-opt;shaderc" Tools/shaderc)
endif()
