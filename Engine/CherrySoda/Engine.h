#ifndef _CHERRYSODA_ENGINE_H_
#define _CHERRYSODA_ENGINE_H_

#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/String.h>


namespace cherrysoda {

class Graphics;
class Scene;
class Window;

class Engine
{
public:
#ifdef __EMSCRIPTEN__
	static void MainLoop();
#endif // __EMSCRIPTEN__

	Engine() : Engine(500, 500, "CherrySoda") {}
	Engine(int width, int height, const String& title) : Engine(width, height, width, height, title, false) {}
	Engine(int width, int height, int windowWidth, int windowHeight, const String& title, bool fullscreen);

	virtual ~Engine();

	CHERRYSODA_GETTER_SETTER_OF_TYPE(double, FreezeTimer, m_freezeTimer);
	CHERRYSODA_GETTER_SETTER_OF_BOOL(ExitOnEscapeKeypress, m_exitOnEscapeKeypress);

	inline int GetWidth() { return m_width; }
	inline int GetHeight() { return m_height; }
	inline Math::IVec2 GetViewSize() { return Math::IVec2(GetWidth(), GetHeight()); }
	inline float GetAspectRatio() { return static_cast<float>(GetWidth()) / static_cast<float>(GetHeight()); }
	inline int GetWindowWidth() { return m_windowWidth; }
	inline int GetWindowHeight() { return m_windowHeight; }
	inline Math::IVec2 GetWindowSize() { return Math::IVec2(GetWindowWidth(), GetWindowHeight()); }
	inline const String GetTitle() { return m_title; }
	void SetTitle(const String& title);
	inline const Color GetClearColor() { return m_clearColor; }
	void SetClearColor(const Color& color);
	Math::IVec2 GetWindowPosition();
	void SetMousePosition(const Math::IVec2& pos);
	void SetFullscreen();
	void SetWindowed();
	void ToggleFullscreen();
	void ShowCursor(bool show);
	inline void ShowCursor() { ShowCursor(true); }
	inline void HideCursor() { ShowCursor(false); }
	void WindowResizable(bool resizable);
	static const char* GetClipboardText();
	static void SetClipboardText(const char* text);

	inline float RawGameTime() const { return static_cast<float>(m_rawGameTime); }
	inline float GameTime() const { return static_cast<float>(m_gameTime); }
	inline float RawDeltaTime() const { return static_cast<float>(m_rawDeltaTime); }
	inline float DeltaTime() const { return static_cast<float>(m_deltaTime); }
	inline double TimeRate() const { return m_timeRate; }
	inline void TimeRate(double timeRate) { m_timeRate = timeRate; }
	inline int FPS() { return m_FPS; }
	inline bool ConsoleOpened() { return m_consoleOpened; }
	inline bool DoShowCursor() const { return m_showCursor; }

	inline void DisableInternalAudio()
	{
		CHERRYSODA_ASSERT(!m_initialized, "Internel audio can only be disabled before engine initialization.\n");
		m_enableInternalAudio = false;
	}

	void Run(int argc = 0, char* argv[] = {});
	void Exit();

	inline Scene* GetScene() { return m_scene; }
	void SetScene(Scene* scene);

	inline bool IsActive()  { return m_active; }

	inline bool Initialized() { return m_initialized; }

	static inline Engine* Instance() { return ms_instance; }

protected:
	virtual void OnClientSizeChanged(int width, int height);
	virtual void OnTextInput(const char* text);

	virtual void OnActivated();
	virtual void OnDeactivated();

	virtual void ParseArgs(int argc, char* argv[]);
	virtual void Initialize();
	virtual void Terminate();
	virtual void LoadContent();
	virtual void UnloadContent();

	virtual void Update();
	virtual void Draw();

	virtual void RenderCore();

	virtual void OnSceneTransition(Scene* from, Scene* to);

private:
	friend class Window;

	inline void SetWindowSize(int width, int height) { m_windowWidth = width; m_windowHeight = height; }
	inline void SetViewSize(int width, int height) { m_width = width; m_height = height; }

	inline void ToggleConsole() { m_consoleOpened = !m_consoleOpened; }

	void IsActive(bool active);

	int m_width;
	int m_height;
	int m_windowWidth;
	int m_windowHeight;
	String m_title;
	Color m_clearColor = Color::Black;
	Window* m_window = nullptr;
	bool m_fullscreen = false;
	bool m_shouldExit = false;
	bool m_initialized = false;
	bool m_resizing = false;
	bool m_active = false;
	bool m_consoleOpened = false;
	bool m_enableInternalAudio = true;
	bool m_showCursor = true;
	bool m_windowResizable = true;
	bool m_exitOnEscapeKeypress = true;

	double m_deltaTime = 0.0;
	double m_rawDeltaTime = 0.0;
	double m_timeRate = 1.0;
	double m_freezeTimer = 0.0;
	double m_rawGameTime = 0.0;
	double m_gameTime = 0.0;
	double m_currentTime = 0.0;
	double m_lastFrameTime = 0.0;
	double m_counterElapsed = 0.0;
	int m_fpsCounter = 0;
	int m_FPS = 0;

	// graphics
	Graphics* m_graphicsDevice = nullptr;

	// scene
	Scene* m_scene = nullptr;
	Scene* m_nextScene = nullptr;

	static Engine* ms_instance;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_ENGINE_H_
