#include <CherrySoda/Interface/Window.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Utility/Color.h>
#include <CherrySoda/Utility/NumTypes.h>

#include <SDL.h>

using cherrysoda::Window;

using cherrysoda::Color;
using cherrysoda::String;

Window::Window()
{
}

void Window::CreateWindow()
{

	int context_flags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
#ifndef NDEBUG
	context_flags |= SDL_GL_CONTEXT_DEBUG_FLAG;
#endif

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, context_flags);

#ifdef CHERRYSODA_OPENGL46
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
#endif
#ifdef CHERRYSODA_GLES2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	int windowWidth = Engine::GetInstance()->GetWindowWidth();
	int windowHeight = Engine::GetInstance()->GetWindowHeight();
	String title = Engine::GetInstance()->GetTitle();
	m_mainWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
	m_glContext = SDL_GL_CreateContext(m_mainWindow);

	MakeContextCurrent();
	SetVsyncEnabled(true);

	Graphics::LoadGraphicsAPI();
	Graphics::SetViewport(0, 0, windowWidth, windowHeight);
}

void Window::DestroyWindow()
{
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_mainWindow);
	m_glContext = nullptr;
	m_mainWindow = nullptr;
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

void Window::SwapBuffers()
{
	SDL_GL_SwapWindow(m_mainWindow);
}

void Window::MakeContextCurrent()
{
	SDL_GL_MakeCurrent(m_mainWindow, m_glContext);
}

void Window::SetClearColor(const Color& color)
{
	Graphics::SetClearColor(color);
}

void Window::SetVsyncEnabled(bool enabled)
{
	//TODO: SDL Vsync
	//glfwSwapInterval(enabled ? 1 : 0);
}

void Window::PollEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			Engine::GetInstance()->Exit();
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				Engine::GetInstance()->Exit();
				break;
			}
			break;
		}
	}
}

bool Window::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
		return false;
	}
	return true;
}

void Window::Terminate()
{
	SDL_Quit();
}