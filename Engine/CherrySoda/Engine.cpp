#include <CherrySoda/Engine.h>

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Interface/Window.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/String.h>

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
	if (auto gfxInstance = Graphics::GetInstance()) {
		gfxInstance->SetClearColor(color);
	}
}

void Engine::Run()
{
	if (!Window::Init()) {
		return;
	}

	m_window = new Window();
	m_window->CreateWindow();

	Graphics::Init();
	Graphics::GetInstance()->UpdateView();
	Graphics::GetInstance()->SetClearColor(m_clearColor);
	Graphics::GetInstance()->RenderFrame();

	m_window->Show();

	while (!m_shouldExit) {
		Window::PollEvents();

		Update();
		Graphics::GetInstance()->RenderFrame();
	}

	Graphics::Terminate();

	m_window->DestroyWindow();
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