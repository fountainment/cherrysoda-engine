#include <CherrySoda/Engine.h>

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Interface/Window.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/String.h>
#include <CherrySoda/Util/Time.h>

using cherrysoda::Engine;

using cherrysoda::Color;
using cherrysoda::Graphics;
using cherrysoda::String;
using cherrysoda::Time;

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
	Initialize();
	LoadContent();

	Graphics::GetInstance()->RenderFrame();
	m_window->Show();

	m_lastFrameTime = Time::GetSystemTime();
	while (!m_shouldExit) {
		Window::PollEvents();
		Update();
		Draw();
	}

	Graphics::Terminate();

	m_window->DestroyWindow();
	Window::Terminate();
}

void Engine::Initialize()
{
	if (!Window::Init()) {
		return;
	}

	m_window = new Window();
	m_window->CreateWindow();

	Graphics::Init();
	Graphics::GetInstance()->UpdateView();
	Graphics::GetInstance()->SetClearColor(m_clearColor);
}

void Engine::LoadContent()
{
}

void Engine::Update()
{
	m_currentTime = Time::GetSystemTime();
	m_rawDeltaTime = m_currentTime - m_lastFrameTime;
	m_deltaTime = m_rawDeltaTime * m_timeRate;
	m_lastFrameTime = m_currentTime;
}

void Engine::Draw()
{
	Graphics::GetInstance()->RenderFrame();
	m_fpsCounter++;
	m_counterElapsed += m_rawDeltaTime;
	if (m_counterElapsed > 1.f) {
		m_FPS = m_fpsCounter;
		m_fpsCounter = 0.f;
		m_counterElapsed -= 1.f;
#ifndef NDEBUG
		m_window->SetTitle(m_title + " " + std::to_string(m_fpsCounter) + " fps");
#endif
	}
}

void Engine::Exit()
{
	m_shouldExit = true;
}

Engine* Engine::ms_instance = nullptr;