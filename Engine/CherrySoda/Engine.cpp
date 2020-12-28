#include <CherrySoda/Engine.h>

#include <CherrySoda/Scene.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Input/MInput.h>
#include <CherrySoda/Interface/Window.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Draw.h>
#include <CherrySoda/Util/GUI.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/Profile.h>
#include <CherrySoda/Util/String.h>
#include <CherrySoda/Util/Time.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif // __EMSCRIPTEN__

using cherrysoda::Engine;

using cherrysoda::Color;
using cherrysoda::Draw;
using cherrysoda::Graphics;
using cherrysoda::GUI;
using cherrysoda::Math;
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
#if defined(CHIP)
	m_width = m_windowWidth = 480;
	m_height = m_windowHeight = 274;
	// Intentionally give two more pixels on height
	// to avoid graphics stuck issue on PocketCHIP
#elif defined(CLOCKWORK_PI)
	m_width = m_windowWidth = 320;
	m_height = m_windowHeight = 240;
#else
	m_width = width;
	m_height = height;
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
#endif
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

Math::IVec2 Engine::GetWindowPosition()
{
	int x = 0, y = 0;
	if (m_window) {
		m_window->GetPosition(&x, &y);
	}
	return Math::IVec2(x, y);
}

void Engine::SetMousePosition(const Math::IVec2& pos)
{
	if (m_window) {
		m_window->SetMousePosition(pos.x, pos.y);
	}
}

void Engine::Run(int argc/* = 0*/, char* argv[]/* = {}*/)
{
	ParseArgs(argc, argv);
	Initialize();
	LoadContent();

	m_lastFrameTime = Time::GetSystemTime();

	// Render one frame before showing window to prevent white blink
	Time::SleepForMilliseconds(16);
	Update();
	Draw();

	m_window->Show();
	Draw();

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(&Engine::MainLoop, -1, 1);
#else
	while (!m_shouldExit) {
		CHERRYSODA_PROFILE_FRAME_MARK();
		CHERRYSODA_PROFILE("MainLoop");
		Update();
		Draw();
	}	
#endif // __EMSCRIPTEN__ 

	GUI::Terminate();
	Draw::Terminate();
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
	CHERRYSODA_DEBUG_FORMAT("cherrysoda::Engine::OnClientSizeChanged(%d, %d)\n", width, height);
	if (width > 0 && height > 0 && !m_resizing) {
		m_resizing = true;
		SetWindowSize(width, height);
		SetViewSize(width, height);
		Graphics::UpdateView();
		m_resizing = false;
	}
}

void Engine::ParseArgs(int argc, char* argv[])
{
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
	Graphics::UpdateView();
	Draw::Initialize();
	GUI::Initialize();
	m_graphicsDevice = Graphics::Instance();
	m_initialized = true;
}

void Engine::LoadContent()
{
}

void Engine::RenderCore()
{
	CHERRYSODA_PROFILE_FUNCTION();

	if (m_scene) {
		m_scene->BeforeRender();
	}

	m_graphicsDevice->BeginRenderPass(0);
	// TODO: add RenderTarget
	// m_graphicsDevice->SetRenderTarget(nullptr);
	m_graphicsDevice->SetViewport(0, 0, GetWidth(), GetHeight());
	m_graphicsDevice->SetClearColor(m_clearColor);
	m_graphicsDevice->Touch();

	if (m_scene) {
		m_scene->Render();
		m_scene->AfterRender();
	}

	GUI::Render();
}

void Engine::OnSceneTransition(Scene* from, Scene* to)
{
	m_timeRate = 1.0;
}

void Engine::Update()
{
	CHERRYSODA_PROFILE_FUNCTION();

	m_window->PollEvents();

	m_currentTime = Time::GetSystemTime();
	m_rawDeltaTime = m_currentTime - m_lastFrameTime;
	// Avoid big deltatime
	if (m_rawDeltaTime > 0.1) m_rawDeltaTime = 0.1;
	m_deltaTime = m_rawDeltaTime * m_timeRate;
	m_lastFrameTime = m_currentTime;

	// Update input
	MInput::Update();

	// Update GUI
	GUI::Update();

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
	CHERRYSODA_PROFILE_FUNCTION();

	RenderCore();
	m_graphicsDevice->RenderFrame();

	// Frame counter
	m_fpsCounter++;
	m_counterElapsed += m_rawDeltaTime;
	if (m_counterElapsed > 1.0) {
#if defined(CHERRYSODA_ENABLE_DEBUG) || defined(CHERRYSODA_ENABLE_PROFILE)
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
