project(glad LANGUAGES C)

add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC include)
target_link_libraries(glad PUBLIC ${CMAKE_DL_LIBS} GLESv2 EGL)
target_compile_definitions(glad PUBLIC CHERRYSODA_GLES2)