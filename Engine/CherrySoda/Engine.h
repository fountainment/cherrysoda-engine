#ifndef _CHERRYSODA_ENGINE_H_
#define _CHERRYSODA_ENGINE_H_

#include <CherrySoda/Util/Color.h>
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

class Window;

class Engine
{
public:
	Engine() : Engine(480, 272, 480, 272, "CherrySoda", false) {}
	Engine(int width, int height, int windowWidth, int windowHeight,
	       const String& title, bool fullscreen);

	inline int GetWidth() { return m_width; }
	inline int GetHeight() { return m_height; }
	inline float GetAspectRatio() { return static_cast<float>(GetWidth()) / static_cast<float>(GetHeight()); }
	inline int GetWindowWidth() { return m_windowWidth; }
	inline int GetWindowHeight() { return m_windowHeight; }
	inline const String GetTitle() { return m_title; }
	void SetTitle(const String& title);
	inline const Color GetClearColor() { return m_clearColor; }
	void SetClearColor(const Color& color);

	inline float GetRawDeltaTime() const { return static_cast<float>(m_rawDeltaTime); }
	inline float GetDeltaTime() const { return static_cast<float>(m_deltaTime); }
	inline double GetTimeRate() const { return m_timeRate; }
	inline void SetTimeRate(double timeRate) { m_timeRate = timeRate; }

	void Run();
	void Exit();

	virtual void Initialize();
	virtual void LoadContent();
	virtual void Update();
	virtual void Draw();

	static Engine* GetInstance() { return ms_instance; }

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
	bool m_fullscreen;
	Window* m_window = nullptr;
	bool m_shouldExit = false;

	double m_rawDeltaTime = 0.0;
	double m_timeRate = 1.0;
	double m_deltaTime = 0.0;
	double m_currentTime = 0.0;
	double m_lastFrameTime = 0.0;
	double m_counterElapsed = 0.0;
	int m_fpsCounter = 0;
	int m_FPS;

	static Engine* ms_instance;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_ENGINE_H_