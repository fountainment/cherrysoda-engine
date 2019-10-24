#ifndef _CHERRYSODA_INTERFACE_WINDOW_H_
#define _CHERRYSODA_INTERFACE_WINDOW_H_

#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/String.h>

#define CHERRYSODA_WINDOW SDL_Window
struct CHERRYSODA_WINDOW;

namespace cherrysoda {

class Window
{
private:
	friend class Engine;

	Window();

	void SetSize(int width, int height);
	void SetTitle(const String& string);
	void SetFullscreen(bool fullscreen);

	void CreateWindow();
	void DestroyWindow();

	void Show();

	static void PollEvents();

	static bool Init();
	static void Terminate();

	CHERRYSODA_WINDOW* m_mainWindow = nullptr;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_INTERFACE_WINDOW_H_