#include <CherrySoda/Graphics/Graphics.h>

#include <CherrySoda/Utility/Log.h>
#include <CherrySoda/Utility/String.h>

#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using cherrysoda::Graphics;

using cherrysoda::Color;
using cherrysoda::String;
using cherrysoda::StringUtil;

void Graphics::LoadGraphicsAPI()
{
#ifdef CHERRYSODA_OPENGL46
	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
#endif
#ifdef CHERRYSODA_GLES2
	gladLoadGLES2Loader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
#endif

	CHERRYSODA_DEBUG(StringUtil::Format("OpenGL Version: %s\n", glGetString(GL_VERSION)));
	CHERRYSODA_DEBUG(StringUtil::Format("GLSL Version:   %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION)));
}

void Graphics::SetClearColor(const Color& color)
{
	glClearColor(color.R(), color.G(), color.B(), color.A());
}

void Graphics::SetViewport(int x, int y, int w, int h)
{
	glViewport(x, y, w, h);
}

void Graphics::ClearColorAndDepth()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

#ifdef CHERRYSODA_GLES2
void Graphics::TriangleGLES2()
{
	CherrySodaDebug("TriangleGLES2\n");
	GLfloat vertex[] = {-0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f};
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertex);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
	glDisableVertexAttribArray(0);
}
#endif // CHERRYSODA_GLES2