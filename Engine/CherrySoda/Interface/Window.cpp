#include <CherrySoda/Interface/Window.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Utility/Color.h>
#include <CherrySoda/Utility/NumTypes.h>

#include <bx/os.h>
#include <bgfx/platform.h>

#include <SDL2/SDL.h>

BX_PRAGMA_DIAGNOSTIC_PUSH()
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG("-Wextern-c-compat")
#include <SDL2/SDL_syswm.h>
BX_PRAGMA_DIAGNOSTIC_POP()

using cherrysoda::Window;

using cherrysoda::Color;
using cherrysoda::String;

static void* sdlNativeWindowHandle(SDL_Window* _window)
{
	SDL_SysWMinfo wmi;
	SDL_VERSION(&wmi.version);
	if (!SDL_GetWindowWMInfo(_window, &wmi))
	{
		return NULL;
	}

#	if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#		if ENTRY_CONFIG_USE_WAYLAND
	wl_egl_window *win_impl = (wl_egl_window*)SDL_GetWindowData(_window, "wl_egl_window");
	if (!win_impl)
	{
		int width, height;
		SDL_GetWindowSize(_window, &width, &height);
		struct wl_surface* surface = wmi.info.wl.surface;
		if (!surface)
			return nullptr;
		win_impl = wl_egl_window_create(surface, width, height);
		SDL_SetWindowData(_window, "wl_egl_window", win_impl);
	}
	return (void*)(uintptr_t)win_impl;
#		else
	return (void*)wmi.info.x11.window;
#		endif
#	elif BX_PLATFORM_OSX
	return wmi.info.cocoa.window;
#	elif BX_PLATFORM_WINDOWS
	return wmi.info.win.window;
#	elif BX_PLATFORM_STEAMLINK
	return wmi.info.vivante.window;
#	endif // BX_PLATFORM_
}

inline bool sdlSetWindow(SDL_Window* _window)
{
	SDL_SysWMinfo wmi;
	SDL_VERSION(&wmi.version);
	if (!SDL_GetWindowWMInfo(_window, &wmi))
	{
		return false;
	}

	bgfx::PlatformData pd;
#	if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#		if ENTRY_CONFIG_USE_WAYLAND
	pd.ndt = wmi.info.wl.display;
#		else
	pd.ndt = wmi.info.x11.display;
#		endif
#	elif BX_PLATFORM_OSX
	pd.ndt = NULL;
#	elif BX_PLATFORM_WINDOWS
	pd.ndt = NULL;
#	elif BX_PLATFORM_STEAMLINK
	pd.ndt = wmi.info.vivante.display;
#	endif // BX_PLATFORM_
	pd.nwh = sdlNativeWindowHandle(_window);

	pd.context = NULL;
	pd.backBuffer = NULL;
	pd.backBufferDS = NULL;
	bgfx::setPlatformData(pd);

	return true;
}

Window::Window()
{
}

void Window::CherrySodaCreateWindow()
{
	int windowWidth = Engine::GetInstance()->GetWindowWidth();
	int windowHeight = Engine::GetInstance()->GetWindowHeight();
	String title = Engine::GetInstance()->GetTitle();
	m_mainWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	
	sdlSetWindow(m_mainWindow);
}

void Window::DestroyWindow()
{
	SDL_DestroyWindow(m_mainWindow);
	m_mainWindow = nullptr;
}

void Window::SetSize(int width, int height)
{
}

void Window::SetTitle(const String& title)
{
}

void Window::SetFullscreen(bool fullscreen)
{
}

void Window::SwapBuffers()
{
	//SDL_GL_SwapWindow(m_mainWindow);
}

void Window::MakeContextCurrent()
{
	//SDL_GL_MakeCurrent(m_mainWindow, m_glContext);
}

void Window::SetClearColor(const Color& color)
{
	Graphics::SetClearColor(color);
}

void Window::SetVsyncEnabled(bool enabled)
{
	//SDL_GL_SetSwapInterval(enabled ? 1 : 0);
}

void Window::PollEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			Engine::GetInstance()->Exit();
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				Engine::GetInstance()->Exit();
				break;
			}
			break;
		}
	}
}

bool Window::Init()
{
	if (SDL_Init(0 | SDL_INIT_GAMECONTROLLER) != 0) {
		return false;
	}
	return true;
}

void Window::Terminate()
{
	SDL_Quit();
}
