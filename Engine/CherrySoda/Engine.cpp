#include <CherrySoda/Engine.h>

#include <CherrySoda/Graphic/Graphic.h>
#include <CherrySoda/Interface/Window.h>
#include <CherrySoda/Utility/Color.h>
#include <CherrySoda/Utility/String.h>

using cherrysoda::Engine;

using cherrysoda::Color;
using cherrysoda::Graphic;
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
	if (m_window) {
		m_window->SetClearColor(color);
	}
}

void Engine::Run()
{
	if (!Window::Init()) {
		return;
	}

	m_window = new Window();
	m_window->CreateWindow();

	m_window->SetClearColor(m_clearColor);

	while (!m_window->ShouldClose())
	{
		Graphic::ClearColorAndDepth();

		Update();

		m_window->SwapBuffers();
		Window::PollEvents();
	}

	m_window->DestroyWindow();
	Window::Terminate();
}

void Engine::Update()
{
}

Engine* Engine::ms_instance = nullptr;
