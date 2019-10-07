#include <CherrySoda/Interface/Window.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Utility/Color.h>
#include <CherrySoda/Utility/NumTypes.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

using cherrysoda::Window;

using cherrysoda::Color;
using cherrysoda::String;

Window::Window()
{
}

void Window::CreateWindow()
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
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
#endif

	int windowWidth = Engine::GetInstance()->GetWindowWidth();
	int windowHeight = Engine::GetInstance()->GetWindowHeight();
	String title = Engine::GetInstance()->GetTitle();
	m_glfwWindow = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), NULL, NULL);

	MakeContextCurrent();
	SetVsyncEnabled(true);

	Graphics::LoadGraphicAPI();
	Graphics::SetViewport(0, 0, windowWidth, windowHeight);
}

void Window::DestroyWindow()
{
	glfwDestroyWindow(m_glfwWindow);
	m_glfwWindow = nullptr;
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

void Window::MakeContextCurrent()
{
	glfwMakeContextCurrent(m_glfwWindow);
}

void Window::SetClearColor(const Color& color)
{
	Graphics::SetClearColor(color);
}

void Window::SetVsyncEnabled(bool enabled)
{
	glfwSwapInterval(enabled ? 1 : 0);
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
	glfwDefaultWindowHints();
	return true;
}

void Window::Terminate()
{
	glfwTerminate();
}
