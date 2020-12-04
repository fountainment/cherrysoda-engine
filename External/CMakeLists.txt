add_subdirectory(bgfx)
add_subdirectory(glm)
add_subdirectory(imgui)
add_subdirectory(rapidjson)

cherrysoda_set_targets_folder("bgfx;bimg;bimg_decode;bimg_encode;bx;imgui" External)
