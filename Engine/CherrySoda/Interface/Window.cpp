#include <CherrySoda/Interface/Window.h>
#include <CherrySoda/Engine.h>
#include <CherrySoda/Utility/NumTypes.h>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace cherrysoda {

Window::Window()
{
}

void Window::InitWindow()
{
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

#ifdef CHERRYSODA_OPENGL46
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
#ifdef CHERRYSODA_GLES2
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
#endif

	int windowWidth = Engine::GetInstance()->GetWindowWidth();
	int windowHeight = Engine::GetInstance()->GetWindowHeight();
	String title = Engine::GetInstance()->GetTitle();
	m_glfwWindow = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), NULL, NULL);

	glfwMakeContextCurrent(m_glfwWindow);

#ifdef CHERRYSODA_OPENGL46
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#endif
#ifdef CHERRYSODA_GLES2
	gladLoadGLESLoader((GLADloadproc)glfwGetProcAddress);
#endif
	glfwSwapInterval(1);

	std::printf("OpenGL Version: %s\n", glGetString(GL_VERSION)); 
	std::printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glViewport(0, 0, windowWidth, windowHeight);
}

void Window::SetSize(int width, int height)
{
}

void Window::SetTitle(const String& title)
{
}

void Window::SetFullscreen(bool fullscreen)
{
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(m_glfwWindow);
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(m_glfwWindow);
}

void Window::Clear()
{
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::PollEvents()
{
	glfwPollEvents();
}

bool Window::Init()
{
	if (GLFW_FALSE == glfwInit())
	{
		return false;
	}
	else
	{
		glfwDefaultWindowHints();
	}

	return true;
}

void Window::Terminate()
{
	glfwTerminate();
}

} // namespace cherrysoda