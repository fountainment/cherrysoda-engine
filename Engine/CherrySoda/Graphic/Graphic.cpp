#include <CherrySoda/Graphic/Graphic.h>

#include <CherrySoda/Utility/Log.h>
#include <CherrySoda/Utility/String.h>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cstdio>

using cherrysoda::Graphic;

using cherrysoda::Color;
using cherrysoda::String;
using cherrysoda::StringUtil;

void Graphic::LoadGraphicAPI()
{
#ifdef CHERRYSODA_OPENGL46
	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
#endif
#ifdef CHERRYSODA_GLES2
	gladLoadGLES2Loader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
#endif

	CherrySodaDebug(StringUtil::Format("OpenGL Version: %s\n", glGetString(GL_VERSION)));
	CherrySodaDebug(StringUtil::Format("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION)));
}

void Graphic::SetClearColor(const Color& color)
{
	glClearColor(color.R(), color.G(), color.B(), color.A());
}

void Graphic::SetViewport(int x, int y, int w, int h)
{
	glViewport(x, y, w, h);
}

void Graphic::ClearColorAndDepth()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

#ifdef CHERRYSODA_GLES2
void Graphic::TriangleGLES2()
{
	CherrySodaDebug("TriangleGLES2\n");
	GLfloat vertex[] = {-0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f};
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertex);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
	glDisableVertexAttribArray(0);
}
#endif
