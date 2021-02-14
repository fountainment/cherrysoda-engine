add_subdirectory(bgfx)
add_subdirectory(glm)
add_subdirectory(imgui)
add_subdirectory(rapidjson)

cherrysoda_set_targets_folder("bgfx;bimg;bimg_decode;bx;imgui" External)
if(CHERRYSODA_BUILD_SHADERC)
  cherrysoda_set_targets_folder("fcpp;glslang;glsl-optimizer;spirv-cross;spirv-opt;shaderc" Tools/shaderc)
endif()
