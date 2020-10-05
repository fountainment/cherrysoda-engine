add_subdirectory(bgfx)
add_subdirectory(glm-0.9.9.7)
add_subdirectory(imgui)
add_subdirectory(rapidjson)

cherrysoda_set_targets_folder("bgfx;bimg;bimg_decode;bx;glm_static;imgui" External)
