#include <CherrySoda/Graphic/Graphic.h>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cstdio>

using cherrysoda::Graphic;
using cherrysoda::Color;

void Graphic::LoadGL()
{
#ifdef CHERRYSODA_OPENGL46
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#endif
#ifdef CHERRYSODA_GLES2
	gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress);
#endif

	std::printf("OpenGL Version: %s\n", glGetString(GL_VERSION)); 
	std::printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void Graphic::SetClearColor(const Color& color)
{
	glClearColor(color.r(), color.g(), color.b(), color.a());
}

void Graphic::SetViewport(int x, int y, int w, int h)
{
	glViewport(x, y, w, h);
}