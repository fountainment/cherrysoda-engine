#include <CherrySoda/Engine.h>

#include <CherrySoda/Scene.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Input/MInput.h>
#include <CherrySoda/Interface/Window.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/String.h>
#include <CherrySoda/Util/Time.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif // __EMSCRIPTEN__

using cherrysoda::Engine;

using cherrysoda::Color;
using cherrysoda::Graphics;
using cherrysoda::MInput;
using cherrysoda::Scene;
using cherrysoda::String;
using cherrysoda::Time;

#ifdef __EMSCRIPTEN__
void Engine::MainLoop()
{
	Engine::Instance()->Update();
	Engine::Instance()->Draw();
}
#endif // __EMSCRIPTEN__

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
	if (auto gfxInstance = Graphics::Instance()) {
		gfxInstance->SetClearColor(color);
	}
}

void Engine::Run()
{
	Initialize();
	LoadContent();

	m_window->Show();

	m_lastFrameTime = Time::GetSystemTime();

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(&Engine::MainLoop, -1, 1);
#else
	while (!m_shouldExit) {
		m_window->PollEvents();
		Update();
		Draw();
	}	
#endif // __EMSCRIPTEN__ 

	Graphics::Terminate();

	m_window->DestroyWindow();
	Window::Terminate();
}

void Engine::SetScene(Scene* scene)
{
	m_nextScene = scene;
}

void Engine::OnClientSizeChanged(int width, int height)
{
	if (!m_resizing) {
		m_resizing = true;
		SetWindowSize(width, height);
		SetViewSize(width, height);
		if (auto gfxInstance = Graphics::Instance()) {
			gfxInstance->UpdateView();
		}
		m_resizing = false;
	}
}

void Engine::Initialize()
{
	if (!Window::Initialize()) {
		return;
	}

	m_window = new Window();
	m_window->CreateWindow();

	MInput::Initialize();
	Graphics::Initialize();
	m_graphicsDevice = Graphics::Instance();
	m_graphicsDevice->UpdateView();
	m_initialized = true;
}

void Engine::LoadContent()
{
}

void Engine::RenderCore()
{
	if (m_scene) {
		m_scene->BeforeRender();
	}

	m_graphicsDevice->RenderPass(0);
	// TODO: add RenderTarget
	// m_graphicsDevice->SetRenderTarget(nullptr);
	m_graphicsDevice->SetViewport(0, 0, GetWidth(), GetHeight());
	m_graphicsDevice->SetClearColor(m_clearColor);
	m_graphicsDevice->Touch();

	if (m_scene) {
		m_scene->Render();
		m_scene->AfterRender();
	}
}

void Engine::OnSceneTransition(Scene* from, Scene* to)
{
	m_timeRate = 1.0;
}

void Engine::Update()
{
	m_window->PollEvents();

	m_currentTime = Time::GetSystemTime();
	m_rawDeltaTime = m_currentTime - m_lastFrameTime;
	m_deltaTime = m_rawDeltaTime * m_timeRate;
	m_lastFrameTime = m_currentTime;

	// Update input
	MInput::Update();

	// Update current scene
	if (m_scene != nullptr) {
		m_scene->BeforeUpdate();
		m_scene->Update();
		m_scene->AfterUpdate();
	}

	// Changing scenes
	if (m_scene != m_nextScene)
	{
		auto lastScene = m_scene;
		if (m_scene != nullptr) {
			m_scene->End();
		}
		m_scene = m_nextScene;
		OnSceneTransition(lastScene, m_nextScene);
		if (m_scene != nullptr) {
			m_scene->Begin();
		}
	}
}

void Engine::Draw()
{
	RenderCore();
	m_graphicsDevice->RenderFrame();

	// Frame counter
	m_fpsCounter++;
	m_counterElapsed += m_rawDeltaTime;
	if (m_counterElapsed > 1.0) {
#ifndef NDEBUG
		m_window->SetTitle(m_title + " " + std::to_string(m_fpsCounter) + " fps");
#endif
		m_FPS = m_fpsCounter;
		m_fpsCounter = 0;
		m_counterElapsed -= 1.0;
	}
}

void Engine::Exit()
{
	m_shouldExit = true;
}

Engine* Engine::ms_instance = nullptr;