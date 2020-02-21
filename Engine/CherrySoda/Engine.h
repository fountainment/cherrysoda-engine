#ifndef _CHERRYSODA_ENGINE_H_
#define _CHERRYSODA_ENGINE_H_

#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/String.h>

#ifdef _MSC_VER
	#pragma comment(linker, "/NODEFAULTLIB:LIBCMT.lib")
	#ifdef NDEBUG
		#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
	#else
		#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
		#pragma comment(linker, "/NODEFAULTLIB:MSVCRT.lib")
	#endif
#endif

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

	Engine() : Engine(500, 500, 500, 500, "CherrySoda", false) {}
	Engine(int width, int height, int windowWidth, int windowHeight,
	       const String& title, bool fullscreen);

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

	inline float RawDeltaTime() const { return static_cast<float>(m_rawDeltaTime); }
	inline float DeltaTime() const { return static_cast<float>(m_deltaTime); }
	inline double TimeRate() const { return m_timeRate; }
	inline void TimeRate(double timeRate) { m_timeRate = timeRate; }
	inline int FPS() { return m_FPS; }

	void Run();
	void Exit();

	void SetScene(Scene* scene);

	inline bool Initialized() { return m_initialized; }

	static Engine* Instance() { return ms_instance; }

protected:
	virtual void OnClientSizeChanged(int width, int height);

	virtual void Initialize();
	virtual void LoadContent();

	virtual void Update();
	virtual void Draw();

	virtual void RenderCore();

	virtual void OnSceneTransition(Scene* from, Scene* to); 

private:
	friend class Window;

	inline void SetWindowSize(int width, int height) { m_windowWidth = width; m_windowHeight = height; }
	inline void SetViewSize(int width, int height) { m_width = width; m_height = height; }

	int m_width;
	int m_height;
	int m_windowWidth;
	int m_windowHeight;
	String m_title;
	Color m_clearColor = Color::Black;
	bool m_fullscreen = false;
	Window* m_window = nullptr;
	bool m_shouldExit = false;
	bool m_initialized = false;
	bool m_resizing = false;

	double m_rawDeltaTime = 0.0;
	double m_timeRate = 1.0;
	double m_deltaTime = 0.0;
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