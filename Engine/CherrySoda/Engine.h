#ifndef _CHERRYSODA_ENGINE_H_
#define _CHERRYSODA_ENGINE_H_

#include <CherrySoda/Utility/String.h>

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
	Engine() : Engine(320, 240, 320, 240, "CherrySoda", false) {}
	Engine(int width, int height, int windowWidth, int windowHeight,
	       const String& title, bool fullscreen);

	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }
	int GetWindowWidth() { return m_windowWidth; }
	int GetWindowHeight() { return m_windowHeight; }
    std::string GetTitle() { return m_title; }
	void SetTitle(const String& title);

	void Run();

    static Engine* GetInstance() { return ms_instance; }

private:
	int m_width;
	int m_height;
	int m_windowWidth;
	int m_windowHeight;
    String m_title;
	bool m_fullscreen;
    Window* m_window = nullptr;

    static Engine* ms_instance;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_ENGINE_H_