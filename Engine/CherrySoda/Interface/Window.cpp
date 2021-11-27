#include <CherrySoda/Interface/Window.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Input/MInput.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>

#include <bx/bx.h>

#ifndef ENTRY_CONFIG_USE_WAYLAND
#	define ENTRY_CONFIG_USE_WAYLAND 0
#endif // ENTRY_CONFIG_USE_WAYLAND

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#	if ENTRY_CONFIG_USE_WAYLAND
#		include <wayland-egl.h>
#	endif
#elif BX_PLATFORM_WINDOWS
#	define SDL_MAIN_HANDLED
#endif

#include <bx/os.h>

#include <SDL.h>

BX_PRAGMA_DIAGNOSTIC_PUSH()
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG("-Wextern-c-compat")
#include <SDL_syswm.h>
BX_PRAGMA_DIAGNOSTIC_POP()

#include <bgfx/platform.h>
#if defined(None) // X11 defines this...
#	undef None
#endif // defined(None)
#if defined(CreateWindow)
#   undef CreateWindow
#endif // defined(CreateWindow)

using cherrysoda::Color;
using cherrysoda::Graphics;
using cherrysoda::Keys;
using cherrysoda::MInput;
using cherrysoda::String;
using cherrysoda::STL;

#ifdef CHERRYSODA_ENABLE_DEBUG
#define ENUM_NAME_PAIR(ENUM) { (int)ENUM, #ENUM }
static STL::HashMap<int,const char*> s_debugEventLookUp =
{
	ENUM_NAME_PAIR(SDL_FIRSTEVENT),
	ENUM_NAME_PAIR(SDL_QUIT),
	ENUM_NAME_PAIR(SDL_APP_TERMINATING),
	ENUM_NAME_PAIR(SDL_APP_LOWMEMORY),
	ENUM_NAME_PAIR(SDL_APP_WILLENTERBACKGROUND),
	ENUM_NAME_PAIR(SDL_APP_DIDENTERBACKGROUND),
	ENUM_NAME_PAIR(SDL_APP_WILLENTERFOREGROUND),
	ENUM_NAME_PAIR(SDL_APP_DIDENTERFOREGROUND),
	ENUM_NAME_PAIR(SDL_DISPLAYEVENT),
	ENUM_NAME_PAIR(SDL_WINDOWEVENT),
	ENUM_NAME_PAIR(SDL_SYSWMEVENT),
	ENUM_NAME_PAIR(SDL_KEYDOWN),
	ENUM_NAME_PAIR(SDL_KEYUP),
	ENUM_NAME_PAIR(SDL_TEXTEDITING),
	ENUM_NAME_PAIR(SDL_TEXTINPUT),
	ENUM_NAME_PAIR(SDL_KEYMAPCHANGED),
	ENUM_NAME_PAIR(SDL_MOUSEMOTION),
	ENUM_NAME_PAIR(SDL_MOUSEBUTTONDOWN),
	ENUM_NAME_PAIR(SDL_MOUSEBUTTONUP),
	ENUM_NAME_PAIR(SDL_MOUSEWHEEL),
	ENUM_NAME_PAIR(SDL_JOYAXISMOTION),
	ENUM_NAME_PAIR(SDL_JOYBALLMOTION),
	ENUM_NAME_PAIR(SDL_JOYHATMOTION),
	ENUM_NAME_PAIR(SDL_JOYBUTTONDOWN),
	ENUM_NAME_PAIR(SDL_JOYBUTTONUP),
	ENUM_NAME_PAIR(SDL_JOYDEVICEADDED),
	ENUM_NAME_PAIR(SDL_JOYDEVICEREMOVED),
	ENUM_NAME_PAIR(SDL_CONTROLLERAXISMOTION),
	ENUM_NAME_PAIR(SDL_CONTROLLERBUTTONDOWN),
	ENUM_NAME_PAIR(SDL_CONTROLLERBUTTONUP),
	ENUM_NAME_PAIR(SDL_CONTROLLERDEVICEADDED),
	ENUM_NAME_PAIR(SDL_CONTROLLERDEVICEREMOVED),
	ENUM_NAME_PAIR(SDL_CONTROLLERDEVICEREMAPPED),
	ENUM_NAME_PAIR(SDL_FINGERDOWN),
	ENUM_NAME_PAIR(SDL_FINGERUP),
	ENUM_NAME_PAIR(SDL_FINGERMOTION),
	ENUM_NAME_PAIR(SDL_DOLLARGESTURE),
	ENUM_NAME_PAIR(SDL_DOLLARRECORD),
	ENUM_NAME_PAIR(SDL_MULTIGESTURE),
	ENUM_NAME_PAIR(SDL_CLIPBOARDUPDATE),
	ENUM_NAME_PAIR(SDL_DROPFILE),
	ENUM_NAME_PAIR(SDL_DROPTEXT),
	ENUM_NAME_PAIR(SDL_DROPBEGIN),
	ENUM_NAME_PAIR(SDL_DROPCOMPLETE),
	ENUM_NAME_PAIR(SDL_AUDIODEVICEADDED),
	ENUM_NAME_PAIR(SDL_AUDIODEVICEREMOVED),
	ENUM_NAME_PAIR(SDL_SENSORUPDATE),
	ENUM_NAME_PAIR(SDL_RENDER_TARGETS_RESET),
	ENUM_NAME_PAIR(SDL_RENDER_DEVICE_RESET),
	ENUM_NAME_PAIR(SDL_USEREVENT),
	ENUM_NAME_PAIR(SDL_LASTEVENT)
};
static STL::HashMap<int,const char*> s_debugWindowEventLookUp =
{
	ENUM_NAME_PAIR(SDL_WINDOWEVENT_NONE),
	ENUM_NAME_PAIR(SDL_WINDOWEVENT_SHOWN),
	ENUM_NAME_PAIR(SDL_WINDOWEVENT_HIDDEN),
	ENUM_NAME_PAIR(SDL_WINDOWEVENT_EXPOSED),
	ENUM_NAME_PAIR(SDL_WINDOWEVENT_MOVED),
	ENUM_NAME_PAIR(SDL_WINDOWEVENT_RESIZED),
	ENUM_NAME_PAIR(SDL_WINDOWEVENT_SIZE_CHANGED),
	ENUM_NAME_PAIR(SDL_WINDOWEVENT_MINIMIZED),
	ENUM_NAME_PAIR(SDL_WINDOWEVENT_MAXIMIZED),
	ENUM_NAME_PAIR(SDL_WINDOWEVENT_RESTORED),
	ENUM_NAME_PAIR(SDL_WINDOWEVENT_ENTER),
	ENUM_NAME_PAIR(SDL_WINDOWEVENT_LEAVE),
	ENUM_NAME_PAIR(SDL_WINDOWEVENT_FOCUS_GAINED),
	ENUM_NAME_PAIR(SDL_WINDOWEVENT_FOCUS_LOST),
	ENUM_NAME_PAIR(SDL_WINDOWEVENT_CLOSE),
	ENUM_NAME_PAIR(SDL_WINDOWEVENT_TAKE_FOCUS),
	ENUM_NAME_PAIR(SDL_WINDOWEVENT_HIT_TEST)
};
#undef ENUM_NAME_PAIR
#endif // CHERRYSODA_ENABLE_DEBUG

static STL::List<Keys> s_keyboardKeys;
static STL::HashMap<int, Keys> s_keycodeToKeys = {
	{ (int)SDLK_a, Keys::A },
	{ (int)SDLK_b, Keys::B },
	{ (int)SDLK_c, Keys::C },
	{ (int)SDLK_d, Keys::D },
	{ (int)SDLK_e, Keys::E },
	{ (int)SDLK_f, Keys::F },
	{ (int)SDLK_g, Keys::G },
	{ (int)SDLK_h, Keys::H },
	{ (int)SDLK_i, Keys::I },
	{ (int)SDLK_j, Keys::J },
	{ (int)SDLK_k, Keys::K },
	{ (int)SDLK_l, Keys::L },
	{ (int)SDLK_m, Keys::M },
	{ (int)SDLK_n, Keys::N },
	{ (int)SDLK_o, Keys::O },
	{ (int)SDLK_p, Keys::P },
	{ (int)SDLK_q, Keys::Q },
	{ (int)SDLK_r, Keys::R },
	{ (int)SDLK_s, Keys::S },
	{ (int)SDLK_t, Keys::T },
	{ (int)SDLK_u, Keys::U },
	{ (int)SDLK_v, Keys::V },
	{ (int)SDLK_w, Keys::W },
	{ (int)SDLK_x, Keys::X },
	{ (int)SDLK_y, Keys::Y },
	{ (int)SDLK_z, Keys::Z },
	{ (int)SDLK_0, Keys::D0 },
	{ (int)SDLK_1, Keys::D1 },
	{ (int)SDLK_2, Keys::D2 },
	{ (int)SDLK_3, Keys::D3 },
	{ (int)SDLK_4, Keys::D4 },
	{ (int)SDLK_5, Keys::D5 },
	{ (int)SDLK_6, Keys::D6 },
	{ (int)SDLK_7, Keys::D7 },
	{ (int)SDLK_8, Keys::D8 },
	{ (int)SDLK_9, Keys::D9 },
	{ (int)SDLK_KP_0, Keys::NumPad0 },
	{ (int)SDLK_KP_1, Keys::NumPad1 },
	{ (int)SDLK_KP_2, Keys::NumPad2 },
	{ (int)SDLK_KP_3, Keys::NumPad3 },
	{ (int)SDLK_KP_4, Keys::NumPad4 },
	{ (int)SDLK_KP_5, Keys::NumPad5 },
	{ (int)SDLK_KP_6, Keys::NumPad6 },
	{ (int)SDLK_KP_7, Keys::NumPad7 },
	{ (int)SDLK_KP_8, Keys::NumPad8 },
	{ (int)SDLK_KP_9, Keys::NumPad9 },
	{ (int)SDLK_KP_CLEAR, Keys::OemClear },
	{ (int)SDLK_KP_DECIMAL, Keys::Decimal },
	{ (int)SDLK_KP_DIVIDE, Keys::Divide },
	{ (int)SDLK_KP_ENTER, Keys::Enter },
	{ (int)SDLK_KP_MINUS, Keys::Subtract },
	{ (int)SDLK_KP_MULTIPLY, Keys::Multiply },
	{ (int)SDLK_KP_PERIOD, Keys::OemPeriod },
	{ (int)SDLK_KP_PLUS, Keys::Add },
	{ (int)SDLK_F1, Keys::F1 },
	{ (int)SDLK_F2, Keys::F2 },
	{ (int)SDLK_F3, Keys::F3 },
	{ (int)SDLK_F4, Keys::F4 },
	{ (int)SDLK_F5, Keys::F5 },
	{ (int)SDLK_F6, Keys::F6 },
	{ (int)SDLK_F7, Keys::F7 },
	{ (int)SDLK_F8, Keys::F8 },
	{ (int)SDLK_F9, Keys::F9 },
	{ (int)SDLK_F10, Keys::F10 },
	{ (int)SDLK_F11, Keys::F11 },
	{ (int)SDLK_F12, Keys::F12 },
	{ (int)SDLK_F13, Keys::F13 },
	{ (int)SDLK_F14, Keys::F14 },
	{ (int)SDLK_F15, Keys::F15 },
	{ (int)SDLK_F16, Keys::F16 },
	{ (int)SDLK_F17, Keys::F17 },
	{ (int)SDLK_F18, Keys::F18 },
	{ (int)SDLK_F19, Keys::F19 },
	{ (int)SDLK_F20, Keys::F20 },
	{ (int)SDLK_F21, Keys::F21 },
	{ (int)SDLK_F22, Keys::F22 },
	{ (int)SDLK_F23, Keys::F23 },
	{ (int)SDLK_F24, Keys::F24 },
	{ (int)SDLK_SPACE, Keys::Space },
	{ (int)SDLK_UP, Keys::Up },
	{ (int)SDLK_DOWN, Keys::Down },
	{ (int)SDLK_LEFT, Keys::Left },
	{ (int)SDLK_RIGHT, Keys::Right },
	{ (int)SDLK_LALT, Keys::LeftAlt },
	{ (int)SDLK_RALT, Keys::RightAlt },
	{ (int)SDLK_LCTRL, Keys::LeftControl },
	{ (int)SDLK_RCTRL, Keys::RightControl },
	{ (int)SDLK_LGUI, Keys::LeftSuper },
	{ (int)SDLK_RGUI, Keys::RightSuper },
	{ (int)SDLK_LSHIFT, Keys::LeftShift },
	{ (int)SDLK_RSHIFT, Keys::RightShift },
	{ (int)SDLK_APPLICATION, Keys::Apps },
	{ (int)SDLK_SLASH, Keys::OemQuestion },
	{ (int)SDLK_BACKSLASH, Keys::OemBackslash },
	{ (int)SDLK_LEFTBRACKET, Keys::OemOpenBrackets },
	{ (int)SDLK_RIGHTBRACKET, Keys::OemCloseBrackets },
	{ (int)SDLK_CAPSLOCK, Keys::CapsLock },
	{ (int)SDLK_COMMA, Keys::OemComma },
	{ (int)SDLK_DELETE, Keys::Delete },
	{ (int)SDLK_END, Keys::End },
	{ (int)SDLK_BACKSPACE, Keys::Back },
	{ (int)SDLK_RETURN, Keys::Enter },
	{ (int)SDLK_ESCAPE, Keys::Escape },
	{ (int)SDLK_HOME, Keys::Home },
	{ (int)SDLK_INSERT, Keys::Insert },
	{ (int)SDLK_MINUS, Keys::OemMinus },
	{ (int)SDLK_NUMLOCKCLEAR, Keys::NumLock },
	{ (int)SDLK_PAGEUP, Keys::PageUp },
	{ (int)SDLK_PAGEDOWN, Keys::PageDown },
	{ (int)SDLK_PAUSE, Keys::Pause },
	{ (int)SDLK_PERIOD, Keys::OemPeriod },
	{ (int)SDLK_EQUALS, Keys::OemPlus },
	{ (int)SDLK_PRINTSCREEN, Keys::PrintScreen },
	{ (int)SDLK_QUOTE, Keys::OemQuotes },
	{ (int)SDLK_SCROLLLOCK, Keys::Scroll },
	{ (int)SDLK_SEMICOLON, Keys::OemSemicolon },
	{ (int)SDLK_SLEEP, Keys::Sleep },
	{ (int)SDLK_TAB, Keys::Tab },
	{ (int)SDLK_BACKQUOTE, Keys::OemTilde },
	{ (int)SDLK_VOLUMEUP, Keys::VolumeUp },
	{ (int)SDLK_VOLUMEDOWN, Keys::VolumeDown },
	{ (int)SDLK_UNKNOWN, Keys::None }
};

static STL::HashMap<int, Keys> s_scancodeToKeys = {
	{ (int)SDL_SCANCODE_A, Keys::A },
	{ (int)SDL_SCANCODE_B, Keys::B },
	{ (int)SDL_SCANCODE_C, Keys::C },
	{ (int)SDL_SCANCODE_D, Keys::D },
	{ (int)SDL_SCANCODE_E, Keys::E },
	{ (int)SDL_SCANCODE_F, Keys::F },
	{ (int)SDL_SCANCODE_G, Keys::G },
	{ (int)SDL_SCANCODE_H, Keys::H },
	{ (int)SDL_SCANCODE_I, Keys::I },
	{ (int)SDL_SCANCODE_J, Keys::J },
	{ (int)SDL_SCANCODE_K, Keys::K },
	{ (int)SDL_SCANCODE_L, Keys::L },
	{ (int)SDL_SCANCODE_M, Keys::M },
	{ (int)SDL_SCANCODE_N, Keys::N },
	{ (int)SDL_SCANCODE_O, Keys::O },
	{ (int)SDL_SCANCODE_P, Keys::P },
	{ (int)SDL_SCANCODE_Q, Keys::Q },
	{ (int)SDL_SCANCODE_R, Keys::R },
	{ (int)SDL_SCANCODE_S, Keys::S },
	{ (int)SDL_SCANCODE_T, Keys::T },
	{ (int)SDL_SCANCODE_U, Keys::U },
	{ (int)SDL_SCANCODE_V, Keys::V },
	{ (int)SDL_SCANCODE_W, Keys::W },
	{ (int)SDL_SCANCODE_X, Keys::X },
	{ (int)SDL_SCANCODE_Y, Keys::Y },
	{ (int)SDL_SCANCODE_Z, Keys::Z },
	{ (int)SDL_SCANCODE_0, Keys::D0 },
	{ (int)SDL_SCANCODE_1, Keys::D1 },
	{ (int)SDL_SCANCODE_2, Keys::D2 },
	{ (int)SDL_SCANCODE_3, Keys::D3 },
	{ (int)SDL_SCANCODE_4, Keys::D4 },
	{ (int)SDL_SCANCODE_5, Keys::D5 },
	{ (int)SDL_SCANCODE_6, Keys::D6 },
	{ (int)SDL_SCANCODE_7, Keys::D7 },
	{ (int)SDL_SCANCODE_8, Keys::D8 },
	{ (int)SDL_SCANCODE_9, Keys::D9 },
	{ (int)SDL_SCANCODE_KP_0, Keys::NumPad0 },
	{ (int)SDL_SCANCODE_KP_1, Keys::NumPad1 },
	{ (int)SDL_SCANCODE_KP_2, Keys::NumPad2 },
	{ (int)SDL_SCANCODE_KP_3, Keys::NumPad3 },
	{ (int)SDL_SCANCODE_KP_4, Keys::NumPad4 },
	{ (int)SDL_SCANCODE_KP_5, Keys::NumPad5 },
	{ (int)SDL_SCANCODE_KP_6, Keys::NumPad6 },
	{ (int)SDL_SCANCODE_KP_7, Keys::NumPad7 },
	{ (int)SDL_SCANCODE_KP_8, Keys::NumPad8 },
	{ (int)SDL_SCANCODE_KP_9, Keys::NumPad9 },
	{ (int)SDL_SCANCODE_KP_CLEAR, Keys::OemClear },
	{ (int)SDL_SCANCODE_KP_DECIMAL, Keys::Decimal },
	{ (int)SDL_SCANCODE_KP_DIVIDE, Keys::Divide },
	{ (int)SDL_SCANCODE_KP_ENTER, Keys::Enter },
	{ (int)SDL_SCANCODE_KP_MINUS, Keys::Subtract },
	{ (int)SDL_SCANCODE_KP_MULTIPLY, Keys::Multiply },
	{ (int)SDL_SCANCODE_KP_PERIOD, Keys::OemPeriod },
	{ (int)SDL_SCANCODE_KP_PLUS, Keys::Add },
	{ (int)SDL_SCANCODE_F1, Keys::F1 },
	{ (int)SDL_SCANCODE_F2, Keys::F2 },
	{ (int)SDL_SCANCODE_F3, Keys::F3 },
	{ (int)SDL_SCANCODE_F4, Keys::F4 },
	{ (int)SDL_SCANCODE_F5, Keys::F5 },
	{ (int)SDL_SCANCODE_F6, Keys::F6 },
	{ (int)SDL_SCANCODE_F7, Keys::F7 },
	{ (int)SDL_SCANCODE_F8, Keys::F8 },
	{ (int)SDL_SCANCODE_F9, Keys::F9 },
	{ (int)SDL_SCANCODE_F10, Keys::F10 },
	{ (int)SDL_SCANCODE_F11, Keys::F11 },
	{ (int)SDL_SCANCODE_F12, Keys::F12 },
	{ (int)SDL_SCANCODE_F13, Keys::F13 },
	{ (int)SDL_SCANCODE_F14, Keys::F14 },
	{ (int)SDL_SCANCODE_F15, Keys::F15 },
	{ (int)SDL_SCANCODE_F16, Keys::F16 },
	{ (int)SDL_SCANCODE_F17, Keys::F17 },
	{ (int)SDL_SCANCODE_F18, Keys::F18 },
	{ (int)SDL_SCANCODE_F19, Keys::F19 },
	{ (int)SDL_SCANCODE_F20, Keys::F20 },
	{ (int)SDL_SCANCODE_F21, Keys::F21 },
	{ (int)SDL_SCANCODE_F22, Keys::F22 },
	{ (int)SDL_SCANCODE_F23, Keys::F23 },
	{ (int)SDL_SCANCODE_F24, Keys::F24 },
	{ (int)SDL_SCANCODE_SPACE, Keys::Space },
	{ (int)SDL_SCANCODE_UP, Keys::Up },
	{ (int)SDL_SCANCODE_DOWN, Keys::Down },
	{ (int)SDL_SCANCODE_LEFT, Keys::Left },
	{ (int)SDL_SCANCODE_RIGHT, Keys::Right },
	{ (int)SDL_SCANCODE_LALT, Keys::LeftAlt },
	{ (int)SDL_SCANCODE_RALT, Keys::RightAlt },
	{ (int)SDL_SCANCODE_LCTRL, Keys::LeftControl },
	{ (int)SDL_SCANCODE_RCTRL, Keys::RightControl },
	{ (int)SDL_SCANCODE_LGUI, Keys::LeftSuper },
	{ (int)SDL_SCANCODE_RGUI, Keys::RightSuper },
	{ (int)SDL_SCANCODE_LSHIFT, Keys::LeftShift },
	{ (int)SDL_SCANCODE_RSHIFT, Keys::RightShift },
	{ (int)SDL_SCANCODE_APPLICATION, Keys::Apps },
	{ (int)SDL_SCANCODE_SLASH, Keys::OemQuestion },
	{ (int)SDL_SCANCODE_BACKSLASH, Keys::OemBackslash },
	{ (int)SDL_SCANCODE_LEFTBRACKET, Keys::OemOpenBrackets },
	{ (int)SDL_SCANCODE_RIGHTBRACKET, Keys::OemCloseBrackets },
	{ (int)SDL_SCANCODE_CAPSLOCK, Keys::CapsLock },
	{ (int)SDL_SCANCODE_COMMA, Keys::OemComma },
	{ (int)SDL_SCANCODE_DELETE, Keys::Delete },
	{ (int)SDL_SCANCODE_END, Keys::End },
	{ (int)SDL_SCANCODE_BACKSPACE, Keys::Back },
	{ (int)SDL_SCANCODE_RETURN, Keys::Enter },
	{ (int)SDL_SCANCODE_ESCAPE, Keys::Escape },
	{ (int)SDL_SCANCODE_HOME, Keys::Home },
	{ (int)SDL_SCANCODE_INSERT, Keys::Insert },
	{ (int)SDL_SCANCODE_MINUS, Keys::OemMinus },
	{ (int)SDL_SCANCODE_NUMLOCKCLEAR, Keys::NumLock },
	{ (int)SDL_SCANCODE_PAGEUP, Keys::PageUp },
	{ (int)SDL_SCANCODE_PAGEDOWN, Keys::PageDown },
	{ (int)SDL_SCANCODE_PAUSE, Keys::Pause },
	{ (int)SDL_SCANCODE_PERIOD, Keys::OemPeriod },
	{ (int)SDL_SCANCODE_EQUALS, Keys::OemPlus },
	{ (int)SDL_SCANCODE_PRINTSCREEN, Keys::PrintScreen },
	{ (int)SDL_SCANCODE_APOSTROPHE, Keys::OemQuotes },
	{ (int)SDL_SCANCODE_SCROLLLOCK, Keys::Scroll },
	{ (int)SDL_SCANCODE_SEMICOLON, Keys::OemSemicolon },
	{ (int)SDL_SCANCODE_SLEEP, Keys::Sleep },
	{ (int)SDL_SCANCODE_TAB, Keys::Tab },
	{ (int)SDL_SCANCODE_GRAVE, Keys::OemTilde },
	{ (int)SDL_SCANCODE_VOLUMEUP, Keys::VolumeUp },
	{ (int)SDL_SCANCODE_VOLUMEDOWN, Keys::VolumeDown },
	{ (int)SDL_SCANCODE_UNKNOWN, Keys::None }
};

namespace entry {
#ifdef __EMSCRIPTEN__
	static const char* canvas_id = "#canvas";
#endif // __EMSCRIPTEN__

	static void* sdlNativeWindowHandle(SDL_Window* _window)
	{
		SDL_SysWMinfo wmi;
		SDL_VERSION(&wmi.version);
#ifndef __EMSCRIPTEN__
		if (!SDL_GetWindowWMInfo(_window, &wmi))
		{
			return nullptr;
		}
#endif // __EMSCRIPTEN__

#	if BX_PLATFORM_ANDROID
		return (void*)wmi.info.android.window;
#	elif BX_PLATFORM_LINUX || BX_PLATFORM_BSD
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
#ifdef __EMSCRIPTEN__
		return (void*)canvas_id;
#endif // __EMSCRIPTEN__
		return nullptr;
	}

	inline bool sdlSetWindow(SDL_Window* _window)
	{
		SDL_SysWMinfo wmi;
		SDL_VERSION(&wmi.version);
#ifndef __EMSCRIPTEN__
		if (!SDL_GetWindowWMInfo(_window, &wmi))
		{
			return false;
		}
#endif // __EMSCRIPTEN__

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
#	elif BX_PLATFORM_EMSCRIPTEN
		pd.ndt = NULL;
#	elif BX_PLATFORM_ANDROID
		pd.ndt = NULL;
#	elif BX_PLATFORM_STEAMLINK
		pd.ndt = wmi.info.vivante.display;
#	endif // BX_PLATFORM_
		pd.nwh = sdlNativeWindowHandle(_window);

		pd.context = NULL;
		pd.backBuffer = NULL;
		pd.backBufferDS = NULL;
		Graphics::SetPlatformData(&pd);

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

void cherrysoda::Window::CreateWindow()
{
	int windowWidth = Engine::Instance()->GetWindowWidth();
	int windowHeight = Engine::Instance()->GetWindowHeight();
	String title = Engine::Instance()->GetTitle();

#ifdef ANDROID
	Engine::Instance()->m_windowResizable = false;
	Engine::Instance()->m_fullscreen = true;

	SDL_SetHint(SDL_HINT_VIDEO_EXTERNAL_CONTEXT, "1");
	SDL_DisplayMode mode;
	SDL_GetDesktopDisplayMode(0, &mode);
	windowWidth  = mode.w;
	windowHeight = mode.h;
	m_mainWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
	Engine::Instance()->SetWindowSize(windowWidth, windowHeight);
#else
	bool resizable  = Engine::Instance()->m_windowResizable;
	bool fullscreen = Engine::Instance()->m_fullscreen;
	Uint32 windowFlag = SDL_WINDOW_HIDDEN;
	if (resizable ) windowFlag |= SDL_WINDOW_RESIZABLE;
	if (fullscreen) windowFlag |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	m_mainWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, windowFlag);
#endif

	bool showCursor = Engine::Instance()->m_showCursor;
	ShowCursor(showCursor);

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
	SDL_SetWindowFullscreen(m_mainWindow, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

bool cherrysoda::Window::IsFullscreen()
{
	return SDL_GetWindowFlags(m_mainWindow) & SDL_WINDOW_FULLSCREEN;
}

void cherrysoda::Window::GetPosition(int* x, int* y)
{
	SDL_GetWindowPosition(m_mainWindow, x, y);
}

void cherrysoda::Window::SetMousePosition(int x, int y)
{
	SDL_WarpMouseInWindow(m_mainWindow, x, y);
}

void cherrysoda::Window::Hide()
{
	SDL_HideWindow(m_mainWindow);
}

void cherrysoda::Window::Show()
{
	SDL_ShowWindow(m_mainWindow);
}

void cherrysoda::Window::ShowCursor(bool show)
{
	SDL_ShowCursor(show ? SDL_ENABLE : SDL_DISABLE);
}

void cherrysoda::Window::Resizable(bool resizable)
{
	CHERRYSODA_ASSERT(SDL_VERSION_ATLEAST(2,0,5), "SDL_SetWindowResizable unavailable for SDL version below 2.0.5\n");
#if SDL_VERSION_ATLEAST(2,0,5)
	SDL_SetWindowResizable(m_mainWindow, resizable ? SDL_TRUE : SDL_FALSE);
#endif // SDL_VERSION_ATLEAST(2,0,5)
}

void cherrysoda::Window::PollEvents()
{
	SDL_Event event;
	Keys key;
#ifdef CHERRYSODA_ENABLE_DEBUG
	bool first = true;
#endif
	while (SDL_PollEvent(&event)) {
#ifdef CHERRYSODA_ENABLE_DEBUG
		if (first) {
			CHERRYSODA_DEBUG("cherrysoda::Window::PollEvents(): ");
			first = false;
		}
		else {
			CHERRYSODA_DEBUG("                                  ");
		}
		CHERRYSODA_DEBUG_FORMAT("0x%X__<%s>\n", event.type, s_debugEventLookUp[event.type]);
#endif // CHERRYSODA_ENABLE_DEBUG
		switch (event.type) {
		case SDL_QUIT:
			Engine::Instance()->Exit();
			break;

		case SDL_KEYDOWN:
		{
			SDL_Scancode scancode = event.key.keysym.scancode;
			if (scancode == SDL_SCANCODE_GRAVE) {
				Engine::Instance()->ToggleConsole();
			}
			if (STL::TryGetValue(s_scancodeToKeys, (int)scancode, key)) {
				if (!STL::Contains(s_keyboardKeys, key)) {
					STL::Add(s_keyboardKeys, key);
				}
			}
			break;
		}
		case SDL_KEYUP:
			if (STL::TryGetValue(s_scancodeToKeys, (int)event.key.keysym.scancode, key)) {
				STL::Remove(s_keyboardKeys, key);
			}
			break;

		case SDL_MOUSEWHEEL:
			MInput::ms_internalMouseWheel += event.wheel.y * 120;
			break;

		case SDL_FINGERDOWN:
			break;
		case SDL_FINGERMOTION:
			break;
		case SDL_FINGERUP:
			break;

		case SDL_CONTROLLERDEVICEADDED:
			MInput::AddControllerInstance(event.cdevice.which);
			break;
		case SDL_CONTROLLERDEVICEREMOVED:
			MInput::RemoveControllerInstance(event.cdevice.which);
			break;

		case SDL_WINDOWEVENT:
		{
			const SDL_WindowEvent& wev = event.window;
#ifdef CHERRYSODA_ENABLE_DEBUG
			CHERRYSODA_DEBUG_FORMAT("                                 -> 0x%X__<%s>\n", wev.event, s_debugWindowEventLookUp[wev.event]);
#endif // CHERRYSODA_ENABLE_DEBUG
			switch (wev.event)
			{
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				Engine::Instance()->IsActive(true);
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				Engine::Instance()->IsActive(false);
				break;
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				Engine::Instance()->OnClientSizeChanged(wev.data1, wev.data2);
				break;
			}
		}
		break;

		case SDL_TEXTINPUT:
		{
			const SDL_TextInputEvent& tev = event.text;
			Engine::Instance()->OnTextInput(tev.text);
		}
		break;
		}
	}
	MInput::SetKeyboardKeys(s_keyboardKeys);
}

bool cherrysoda::Window::Initialize()
{
#ifdef _WIN32
	SDL_SetMainReady();
#endif // _WIN32

	if (SDL_Init(0) != 0) {
		return false;
	}
	SDL_InitSubSystem(SDL_INIT_TIMER);

	SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight Portrait");

	return true;
}

void cherrysoda::Window::Terminate()
{
	SDL_QuitSubSystem(SDL_INIT_TIMER);
	SDL_Quit();
}
