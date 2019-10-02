project(glad LANGUAGES C)

add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC include)
find_package(OpenGL REQUIRED)
target_link_libraries(glad PUBLIC ${CMAKE_DL_LIBS} ${OPENGL_LIBRARIES})
target_compile_definitions(glad PUBLIC CHERRYSODA_OPENGL46)