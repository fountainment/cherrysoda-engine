#ifndef _CHERRYSODA_INTERFACE_WINDOW_H_
#define _CHERRYSODA_INTERFACE_WINDOW_H_

#include <CherrySoda/Utility/Color.h>
#include <CherrySoda/Utility/String.h>

#define CHERRYSODA_WINDOW SDL_Window
struct CHERRYSODA_WINDOW;

namespace cherrysoda {

class Window
{
	friend class Engine;

private:
	Window();

	void SetSize(int width, int height);
	void SetTitle(const String& string);
	void SetFullscreen(bool fullscreen);

	void CherrySodaCreateWindow();
	void DestroyWindow();
	void SwapBuffers();

	void MakeContextCurrent();
	void SetClearColor(const Color& color);
	void SetVsyncEnabled(bool enabled);

	static void PollEvents();

	static bool Init();
	static void Terminate();

	CHERRYSODA_WINDOW* m_mainWindow = nullptr;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_INTERFACE_WINDOW_H_