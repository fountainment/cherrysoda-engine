#include <CherrySoda/Engine.h>
#include <CherrySoda/Interface/Window.h>
#include <CherrySoda/Utility/String.h>

namespace cherrysoda {

Engine::Engine(int width, int height, int windowWidth, int windowHeight,
               const String& title, bool fullscreen)
{
	ms_instance = this;
	m_width = width;
	m_height = height;
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
	m_title = title;
	m_fullscreen = fullscreen;
}

void Engine::SetTitle(const String& title)
{
	m_title = title;
	if (m_window) {
		m_window->SetTitle(title);
	}
}

void Engine::Run()
{
	if (!Window::Init()) {
		return;
	}

	m_window = new Window();
	m_window->InitWindow();

	while (!m_window->ShouldClose())
	{
		m_window->Clear();
		// TODO: Engine Loop

		m_window->SwapBuffers();
		Window::PollEvents();
	}

	Window::Terminate();
}

Engine* Engine::ms_instance = nullptr;

} // namespace cherrysoda