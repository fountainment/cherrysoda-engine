#include <CherrySoda/Engine.h>

#include <CherrySoda/Scene.h>
#include <CherrySoda/Audio/Audio.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Input/MInput.h>
#include <CherrySoda/Interface/Window.h>
#include <CherrySoda/Util/Commands.h>
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

using cherrysoda::Audio;
using cherrysoda::Color;
using cherrysoda::Commands;
using cherrysoda::CommandBatches;
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

Engine::~Engine()
{
	ms_instance = nullptr;
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

void Engine::SetFullscreen()
{
	m_fullscreen = true;
	if (m_window) {
		m_window->SetFullscreen(true);
	}
}

void Engine::SetWindowed()
{
	m_fullscreen = false;
	if (m_window) {
		m_window->SetFullscreen(false);
	}
}

void Engine::ToggleFullscreen()
{
	m_fullscreen = !m_fullscreen;
	if (m_window) {
		m_fullscreen = !(m_window->IsFullscreen());
		m_window->SetFullscreen(m_fullscreen);
	}
}

void Engine::ShowCursor(bool show)
{
	m_showCursor = show;
	if (m_window) {
		m_window->ShowCursor(show);
	}
}

void Engine::WindowResizable(bool resizable)
{
	m_windowResizable = resizable;
	if (m_window) {
		m_window->Resizable(resizable);
	}
}

void Engine::Run(int argc/* = 0*/, char* argv[]/* = {}*/)
{
	ParseArgs(argc, argv);
	Initialize();
	LoadContent();

	m_gameTime = 0.0;
	m_rawGameTime = 0.0;
	m_lastFrameTime = Time::GetSystemTime();

	if (!m_shouldExit) {
		// Render one frame before showing window to prevent white blink
		Time::SleepForMilliseconds(16);
		Update();
		Draw();

		m_window->Show();
		Draw();
	}

	IsActive(true);

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

	Terminate();
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

void Engine::OnTextInput(const char* text)
{
	GUI::TextInput(text);
}

void Engine::IsActive(bool active)
{
	if (m_active != active) {
		m_active = active;
		if (m_active)
			OnActivated();
		else
			OnDeactivated();
	}
}

void Engine::OnActivated()
{
	if (m_scene) {
		m_scene->GainFocus();
	}
}

void Engine::OnDeactivated()
{
	if (m_scene) {
		m_scene->LoseFocus();
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
	if (m_enableInternalAudio) {
		Audio::Initialize();
	}
	Commands::Initialize();

	m_graphicsDevice = Graphics::Instance();
	m_initialized = true;
}

void Engine::Terminate()
{
	// TODO: What if the scene is not allocated on heap
	if (m_scene) {
		delete m_scene;
		m_scene = nullptr;
	}

	Commands::Terminate();
	Audio::Terminate();
	GUI::Terminate();
	Draw::Terminate();
	Graphics::Terminate();
	MInput::Terminate();

	m_window->DestroyWindow();
	delete m_window;
	m_window = nullptr;

	Window::Terminate();

	m_initialized = false;
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
	m_graphicsDevice->SetRenderTarget(nullptr);
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
	m_rawGameTime += m_rawDeltaTime;
	m_gameTime += m_deltaTime;
	m_lastFrameTime = m_currentTime;

	// Update input
	MInput::Update();

	// Update GUI
	GUI::Update();

	// Update command batches
	CommandBatches::Update();

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
