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

	int windowWidth = Engine::GetInstance()->GetWindowWidth();
	int windowHeight = Engine::GetInstance()->GetWindowHeight();
	String title = Engine::GetInstance()->GetTitle();
	m_glfwWindow = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), NULL, NULL);

	glfwMakeContextCurrent(m_glfwWindow);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	gladLoadGL();
	glfwSwapInterval(1);
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