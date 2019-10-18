#include <CherrySoda/Engine.h>

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Interface/Window.h>
#include <CherrySoda/Utility/Color.h>
#include <CherrySoda/Utility/String.h>

using cherrysoda::Engine;

using cherrysoda::Color;
using cherrysoda::Graphics;
using cherrysoda::String;

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

void Engine::SetClearColor(const Color& color)
{
	m_clearColor = color;
	if (Graphics::GetInstance() != nullptr) {
		Graphics::SetClearColor(color);
	}
}

void Engine::Run()
{
	if (!Window::Init()) {
		return;
	}

	m_window = new Window();
	m_window->CherrySodaCreateWindow();

	Graphics::Init();
	Graphics::Reset(false);
	Graphics::SetClearColor(m_clearColor);
	Graphics::RenderFrame();

	m_window->Show();

	while (!m_shouldExit) {
		Window::PollEvents();

		Update();
		Graphics::RenderFrame();
	}

	Graphics::Terminate();
	Window::Terminate();
}

void Engine::Update()
{
}

void Engine::Exit()
{
	m_shouldExit = true;
}

Engine* Engine::ms_instance = nullptr;