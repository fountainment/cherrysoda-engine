#ifndef _CHERRYSODA_INTERFACE_WINDOW_H_
#define _CHERRYSODA_INTERFACE_WINDOW_H_

#include <CherrySoda/Utility/String.h>

class GLFWwindow;

namespace cherrysoda {

class Window
{
	friend class Engine;

private:
	Window();

	void SetSize(int width, int height);
	void SetTitle(const String& string);
	void SetFullscreen(bool fullscreen);

	void InitWindow();
	bool ShouldClose();
	void SwapBuffers();

	void Clear();

	static void PollEvents();

	static bool Init();
	static void Terminate();

	GLFWwindow* m_glfwWindow = nullptr;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_INTERFACE_WINDOW_H_