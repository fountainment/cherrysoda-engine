if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
  add_subdirectory(glad_gl4.6)
else()
  add_subdirectory(glad_gles2)
endif()

add_subdirectory(glfw-3.3)
add_subdirectory(glm-0.9.9.6)