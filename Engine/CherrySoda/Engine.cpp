#include <CherrySoda/Engine.h>

#include <CherrySoda/Audio/Audio.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Input/MInput.h>
#include <CherrySoda/Interface/Window.h>
#include <CherrySoda/Scene.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Commands.h>
#include <CherrySoda/Util/Draw.h>
#include <CherrySoda/Util/GUI.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/Profile.h>
#include <CherrySoda/Util/String.h>
#include <CherrySoda/Util/Time.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif // __EMSCRIPTEN__

#include <SDL3/SDL.h>

#include <algorithm>

namespace cherrysoda {

#ifdef __EMSCRIPTEN__
void Engine::MainLoop()
{
	Engine::Instance()->Update();
	Engine::Instance()->Draw();
}
#endif // __EMSCRIPTEN__

Engine::Engine(int width, int height, int windowWidth, int windowHeight, const String& title, bool fullscreen)
{
	ms_instance = this;
#ifdef CHIP
	m_width = m_windowWidth = 480;
	m_height = m_windowHeight = 274;
	// Intentionally give two more pixels on height
	// to avoid graphics stuck issue on PocketCHIP
	m_pixelWidth = m_windowWidth;
	m_pixelHeight = m_windowHeight;
#elif defined(CLOCKWORK_PI)
	m_width = m_windowWidth = 320;
	m_height = m_windowHeight = 240;
	m_pixelWidth = m_windowWidth;
	m_pixelHeight = m_windowHeight;
#else
	m_width = width;
	m_height = height;
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
	m_pixelWidth = windowWidth;
	m_pixelHeight = windowHeight;
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
	if (auto* gfxInstance = Graphics::Instance()) {
		gfxInstance->SetClearColor(color);
	}
}

Math::IVec2 Engine::GetWindowPosition()
{
	int x = 0, y = 0;
	if (m_window) {
		m_window->GetPosition(&x, &y);
	}
	return {x, y};
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
		m_fullscreen = !m_window->IsFullscreen();
		m_window->SetFullscreen(m_fullscreen);
	}
}

void Engine::ShowCursor(bool show)
{
	m_showCursor = show;
	if (m_window) {
		cherrysoda::Window::ShowCursor(show);
	}
}

void Engine::WindowResizable(bool resizable)
{
	m_windowResizable = resizable;
	if (m_window) {
		m_window->Resizable(resizable);
	}
}

void Engine::SetTextInputEnabled(bool enable)
{
	if (m_window) {
		m_window->SetTextInputEnabled(enable);
	}
}

#ifdef __EMSCRIPTEN__
EM_JS(void, EM_SetClipboardText, (const char* text), { navigator.clipboard.writeText(UTF8ToString(text)); });
#endif // __EMSCRIPTEN__

const char* Engine::GetClipboardText()
{
	// TODO: make it work on emscripten build
	return SDL_GetClipboardText();
}

void Engine::SetClipboardText(const char* text)
{
#ifdef __EMSCRIPTEN__
	EM_SetClipboardText(text);
#else
	SDL_SetClipboardText(text);
#endif // __EMSCRIPTEN__
}

void Engine::Run(int argc /* = 0*/, char* argv[] /* = {}*/)
{
	ParseArgs(argc, argv);
	Initialize();
	if (!m_initialized) {
		CHERRYSODA_LOG("Engine initialization failed!\n");
		return;
	}
	LoadContent();

	m_gameTime = 0.0;
	m_rawGameTime = 0.0;
	m_lastFrameTime = Time::GetSystemTime();

	if (!m_shouldExit) {
		// Render one frame before showing window to prevent white blink
		Time::SleepForMilliseconds(16);
		Update();
		Draw();

#ifdef SDL_PLATFORM_MACOS
		// bgfx's Metal backend skips presenting to hidden (occluded) windows, so
		// unlike on Windows the pre-rendered frame above never reaches the
		// CAMetalLayer; tint it with the clear color so the window doesn't flash
		// the default white NSWindow background before the first real present.
		m_window->SetOpaqueNativeBackground(GetClearColor());
#endif

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
	if (width <= 0 || height <= 0) {
		return;
	}
	// Logical window size in SDL window coordinates (the mouse-coordinate space):
	// store it and re-derive the logical view size. The drawable pixel size is
	// tracked separately by OnPixelSizeChanged, which also drives the swapchain.
	SetWindowSize(width, height);
	SetViewSize((int)SDL_max(1, SDL_lroundf(width / m_contentScale)),
				(int)SDL_max(1, SDL_lroundf(height / m_contentScale)));
}

void Engine::OnPixelSizeChanged(int width, int height)
{
	CHERRYSODA_DEBUG_FORMAT("cherrysoda::Engine::OnPixelSizeChanged(%d, %d)\n", width, height);
	if (width <= 0 || height <= 0) {
		return;
	}
	// Drawable pixels: the swapchain/viewport axis. On macOS with
	// SDL_WINDOW_HIGH_PIXEL_DENSITY this is denser than the window size.
	SetPixelSize(width, height);
	Graphics::UpdateView();
}

void Engine::OnDisplayScaleChanged()
{
	// The window moved to a display with a different content scale / pixel
	// density, or the OS scale changed: refresh both factors and re-derive
	// everything that depends on them.
	RefreshDisplayScale();
	SetViewSize((int)SDL_max(1, SDL_lroundf(GetWindowWidth() / m_contentScale)),
				(int)SDL_max(1, SDL_lroundf(GetWindowHeight() / m_contentScale)));
	Graphics::UpdateView();
	GUI::RefreshDpiScale();
}

void Engine::RefreshDisplayScale()
{
	if (m_window == nullptr || m_window->m_mainWindow == nullptr) {
		return;
	}
	float contentScale = SDL_GetDisplayContentScale(SDL_GetDisplayForWindow(m_window->m_mainWindow));
	if (contentScale <= 0.0f) {
		contentScale = 1.0f;
	}
	m_contentScale = contentScale;
	float pixelDensity = SDL_GetWindowPixelDensity(m_window->m_mainWindow);
	if (pixelDensity <= 0.0f) {
		pixelDensity = 1.0f;
	}
	m_pixelDensity = pixelDensity;
	CHERRYSODA_DEBUG_FORMAT("cherrysoda::Engine::RefreshDisplayScale(contentScale = %g, pixelDensity = %g)\n",
							m_contentScale, m_pixelDensity);
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

void Engine::ParseArgs(int /*argc*/, char* /*argv*/[])
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
	UnloadContent();

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

void Engine::UnloadContent()
{
}

void Engine::RenderCore()
{
	CHERRYSODA_PROFILE_FUNCTION();

	if (m_scene) {
		m_scene->BeforeRender();
	}

	cherrysoda::Graphics::BeginRenderPass(0);
	m_graphicsDevice->SetRenderTarget(nullptr);
	// the viewport covers the drawable pixels; camera/view sizes stay in logical
	// points so content is scaled up on high-DPI displays
	m_graphicsDevice->SetViewport(0, 0, GetPixelWidth(), GetPixelHeight());
	m_graphicsDevice->SetClearColor(m_clearColor);
	m_graphicsDevice->Touch();

	if (m_scene) {
		m_scene->Render();
		m_scene->AfterRender();
	}

	GUI::Render();
}

void Engine::OnSceneTransition(Scene* /*from*/, Scene* /*to*/)
{
	m_timeRate = 1.0;
}

void Engine::Update()
{
	CHERRYSODA_PROFILE_FUNCTION();

	Window::PollEvents();

	m_currentTime = Time::GetSystemTime();
	m_rawDeltaTime = m_currentTime - m_lastFrameTime;
	// Avoid big deltatime
	m_rawDeltaTime = std::min(m_rawDeltaTime, 0.1);
	m_deltaTime = m_rawDeltaTime * m_timeRate;
	m_rawGameTime += m_rawDeltaTime;
	m_gameTime += m_deltaTime;
	m_lastFrameTime = m_currentTime;

	// Update input
	MInput::Update();

	// Reclaim finished one-shot audio instances
	if (m_enableInternalAudio) {
		Audio::Update();
	}

	if (ExitOnEscapeKeypress() && MInput::Keyboard()->Pressed(Keys::Escape)) {
		Exit();
		return;
	}

	// Update GUI
	GUI::Update();

	// Update command batches
	CommandBatches::Update();

	// Update current scene
	if (m_freezeTimer > 0.0) {
		m_freezeTimer = Math_Max(m_freezeTimer - m_rawDeltaTime, 0.0);
	}
	else if (m_scene != nullptr) {
		m_scene->BeforeUpdate();
		m_scene->Update();
		m_scene->AfterUpdate();
	}

	// Changing scenes
	if (m_scene != m_nextScene) {
		auto* lastScene = m_scene;
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
	Graphics::RenderFrame();

	// Frame counter
	m_fpsCounter++;
	m_counterElapsed += m_rawDeltaTime;
	if (m_counterElapsed > 1.0) {
#if defined(CHERRYSODA_ENABLE_DEBUG) || defined(CHERRYSODA_ENABLE_PROFILE)
		m_window->SetTitle(m_title + " " + std::to_string(m_fpsCounter) + " fps");
#endif
		m_fps = m_fpsCounter;
		m_fpsCounter = 0;
		m_counterElapsed -= 1.0;
	}
}

void Engine::Exit()
{
	m_shouldExit = true;
}

Engine* Engine::ms_instance = nullptr;

} // namespace cherrysoda
