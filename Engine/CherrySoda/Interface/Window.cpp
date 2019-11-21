#include <CherrySoda/Interface/Window.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/NumType.h>


#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#	if ENTRY_CONFIG_USE_WAYLAND
#		include <wayland-egl.h>
#	endif 
#elif BX_PLATFORM_WINDOWS
#	define SDL_MAIN_HANDLED
#endif

#include <bx/os.h>

#include <SDL2/SDL.h>

BX_PRAGMA_DIAGNOSTIC_PUSH()
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG("-Wextern-c-compat")
#include <SDL2/SDL_syswm.h>
BX_PRAGMA_DIAGNOSTIC_POP()

#include <bgfx/platform.h>
#if defined(None) // X11 defines this...
#	undef None
#endif // defined(None)
#if defined(CreateWindow)
#   undef CreateWindow
#endif // defined(CreateWindow)

using cherrysoda::Color;
using cherrysoda::String;

namespace entry {

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
		wl_egl_window* win_impl = (wl_egl_window*)SDL_GetWindowData(_window, "wl_egl_window");
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

	static void sdlDestroyWindow(SDL_Window* _window)
	{
		if (!_window)
			return;
#	if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#		if ENTRY_CONFIG_USE_WAYLAND
		wl_egl_window* win_impl = (wl_egl_window*)SDL_GetWindowData(_window, "wl_egl_window");
		if (win_impl)
		{
			SDL_SetWindowData(_window, "wl_egl_window", nullptr);
			wl_egl_window_destroy(win_impl);
		}
#		endif
#	endif
		SDL_DestroyWindow(_window);
	}

} // namespace entry

cherrysoda::Window::Window()
{
}

void cherrysoda::Window::CreateWindow()
{
	int windowWidth = Engine::Instance()->GetWindowWidth();
	int windowHeight = Engine::Instance()->GetWindowHeight();
	String title = Engine::Instance()->GetTitle();
	m_mainWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE);
	entry::sdlSetWindow(m_mainWindow);
}

void cherrysoda::Window::DestroyWindow()
{
	entry::sdlDestroyWindow(m_mainWindow);
}

void cherrysoda::Window::SetSize(int width, int height)
{
	SDL_SetWindowSize(m_mainWindow, width, height);
}

void cherrysoda::Window::SetTitle(const String& title)
{
	SDL_SetWindowTitle(m_mainWindow, title.c_str());
}

void cherrysoda::Window::SetFullscreen(bool fullscreen)
{
	Engine::Instance()->m_fullscreen = fullscreen;
	SDL_SetWindowFullscreen(m_mainWindow, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

void cherrysoda::Window::ToggleFullscreen()
{
	SetFullscreen(!(Engine::Instance()->m_fullscreen));
}

void cherrysoda::Window::Show()
{
	SDL_ShowWindow(m_mainWindow);
}

void cherrysoda::Window::PollEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			Engine::Instance()->Exit();
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				Engine::Instance()->Exit();
				break;
			case SDLK_F11:
				ToggleFullscreen();
				break;
			}
			break;
		case SDL_WINDOWEVENT:
		{
			const SDL_WindowEvent& wev = event.window;
			switch (wev.event)
			{
			case SDL_WINDOWEVENT_SIZE_CHANGED:
			{
				Engine::Instance()->OnClientSizeChanged(wev.data1, wev.data2);
			}
			break;
			}
		}
		break;
		}
	}
}

bool cherrysoda::Window::Init()
{
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
		return false;
	}
	return true;
}

void cherrysoda::Window::Terminate()
{
	SDL_Quit();
}