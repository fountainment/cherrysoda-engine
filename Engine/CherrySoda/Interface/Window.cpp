#include <CherrySoda/Interface/Window.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Input/MInput.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/GUI.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>

#include <bx/bx.h>
#include <bx/os.h>

#include <SDL3/SDL.h>

#include <bgfx/bgfx.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif // __EMSCRIPTEN__

namespace cherrysoda {

static SDL_Cursor* s_mouseCursors[(size_t)CursorTypes::Count] = {};

#ifdef CHERRYSODA_ENABLE_DEBUG
#define ENUM_NAME_PAIR(ENUM) {(int)(ENUM), #ENUM}
static STL::HashMap<int, const char*> s_debugEventLookUp = {
	ENUM_NAME_PAIR(SDL_EVENT_QUIT),
	ENUM_NAME_PAIR(SDL_EVENT_TERMINATING),
	ENUM_NAME_PAIR(SDL_EVENT_LOW_MEMORY),
	ENUM_NAME_PAIR(SDL_EVENT_WILL_ENTER_BACKGROUND),
	ENUM_NAME_PAIR(SDL_EVENT_DID_ENTER_BACKGROUND),
	ENUM_NAME_PAIR(SDL_EVENT_WILL_ENTER_FOREGROUND),
	ENUM_NAME_PAIR(SDL_EVENT_DID_ENTER_FOREGROUND),
	ENUM_NAME_PAIR(SDL_EVENT_LOCALE_CHANGED),
	ENUM_NAME_PAIR(SDL_EVENT_SYSTEM_THEME_CHANGED),
	ENUM_NAME_PAIR(SDL_EVENT_DISPLAY_ORIENTATION),
	ENUM_NAME_PAIR(SDL_EVENT_DISPLAY_ADDED),
	ENUM_NAME_PAIR(SDL_EVENT_DISPLAY_REMOVED),
	ENUM_NAME_PAIR(SDL_EVENT_DISPLAY_MOVED),
	ENUM_NAME_PAIR(SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED),
	ENUM_NAME_PAIR(SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED),
	ENUM_NAME_PAIR(SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED),
	ENUM_NAME_PAIR(SDL_EVENT_DISPLAY_USABLE_BOUNDS_CHANGED),
	ENUM_NAME_PAIR(SDL_EVENT_WINDOW_SHOWN),
	ENUM_NAME_PAIR(SDL_EVENT_WINDOW_HIDDEN),
	ENUM_NAME_PAIR(SDL_EVENT_WINDOW_EXPOSED),
	ENUM_NAME_PAIR(SDL_EVENT_WINDOW_MOVED),
	ENUM_NAME_PAIR(SDL_EVENT_WINDOW_RESIZED),
	ENUM_NAME_PAIR(SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED),
	ENUM_NAME_PAIR(SDL_EVENT_WINDOW_METAL_VIEW_RESIZED),
	ENUM_NAME_PAIR(SDL_EVENT_WINDOW_MINIMIZED),
	ENUM_NAME_PAIR(SDL_EVENT_WINDOW_MAXIMIZED),
	ENUM_NAME_PAIR(SDL_EVENT_WINDOW_RESTORED),
	ENUM_NAME_PAIR(SDL_EVENT_WINDOW_MOUSE_ENTER),
	ENUM_NAME_PAIR(SDL_EVENT_WINDOW_MOUSE_LEAVE),
	ENUM_NAME_PAIR(SDL_EVENT_WINDOW_FOCUS_GAINED),
	ENUM_NAME_PAIR(SDL_EVENT_WINDOW_FOCUS_LOST),
	ENUM_NAME_PAIR(SDL_EVENT_WINDOW_CLOSE_REQUESTED),
	ENUM_NAME_PAIR(SDL_EVENT_WINDOW_HIT_TEST),
	ENUM_NAME_PAIR(SDL_EVENT_WINDOW_ICCPROF_CHANGED),
	ENUM_NAME_PAIR(SDL_EVENT_WINDOW_DISPLAY_CHANGED),
	ENUM_NAME_PAIR(SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED),
	ENUM_NAME_PAIR(SDL_EVENT_WINDOW_SAFE_AREA_CHANGED),
	ENUM_NAME_PAIR(SDL_EVENT_WINDOW_OCCLUDED),
	ENUM_NAME_PAIR(SDL_EVENT_WINDOW_ENTER_FULLSCREEN),
	ENUM_NAME_PAIR(SDL_EVENT_WINDOW_LEAVE_FULLSCREEN),
	ENUM_NAME_PAIR(SDL_EVENT_WINDOW_DESTROYED),
	ENUM_NAME_PAIR(SDL_EVENT_WINDOW_HDR_STATE_CHANGED),
	ENUM_NAME_PAIR(SDL_EVENT_KEY_DOWN),
	ENUM_NAME_PAIR(SDL_EVENT_KEY_UP),
	ENUM_NAME_PAIR(SDL_EVENT_TEXT_EDITING),
	ENUM_NAME_PAIR(SDL_EVENT_TEXT_INPUT),
	ENUM_NAME_PAIR(SDL_EVENT_KEYMAP_CHANGED),
	ENUM_NAME_PAIR(SDL_EVENT_KEYBOARD_ADDED),
	ENUM_NAME_PAIR(SDL_EVENT_KEYBOARD_REMOVED),
	ENUM_NAME_PAIR(SDL_EVENT_TEXT_EDITING_CANDIDATES),
	ENUM_NAME_PAIR(SDL_EVENT_SCREEN_KEYBOARD_SHOWN),
	ENUM_NAME_PAIR(SDL_EVENT_SCREEN_KEYBOARD_HIDDEN),
	ENUM_NAME_PAIR(SDL_EVENT_MOUSE_MOTION),
	ENUM_NAME_PAIR(SDL_EVENT_MOUSE_BUTTON_DOWN),
	ENUM_NAME_PAIR(SDL_EVENT_MOUSE_BUTTON_UP),
	ENUM_NAME_PAIR(SDL_EVENT_MOUSE_WHEEL),
	ENUM_NAME_PAIR(SDL_EVENT_MOUSE_ADDED),
	ENUM_NAME_PAIR(SDL_EVENT_MOUSE_REMOVED),
	ENUM_NAME_PAIR(SDL_EVENT_JOYSTICK_AXIS_MOTION),
	ENUM_NAME_PAIR(SDL_EVENT_JOYSTICK_BALL_MOTION),
	ENUM_NAME_PAIR(SDL_EVENT_JOYSTICK_HAT_MOTION),
	ENUM_NAME_PAIR(SDL_EVENT_JOYSTICK_BUTTON_DOWN),
	ENUM_NAME_PAIR(SDL_EVENT_JOYSTICK_BUTTON_UP),
	ENUM_NAME_PAIR(SDL_EVENT_JOYSTICK_ADDED),
	ENUM_NAME_PAIR(SDL_EVENT_JOYSTICK_REMOVED),
	ENUM_NAME_PAIR(SDL_EVENT_JOYSTICK_BATTERY_UPDATED),
	ENUM_NAME_PAIR(SDL_EVENT_JOYSTICK_UPDATE_COMPLETE),
	ENUM_NAME_PAIR(SDL_EVENT_GAMEPAD_AXIS_MOTION),
	ENUM_NAME_PAIR(SDL_EVENT_GAMEPAD_BUTTON_DOWN),
	ENUM_NAME_PAIR(SDL_EVENT_GAMEPAD_BUTTON_UP),
	ENUM_NAME_PAIR(SDL_EVENT_GAMEPAD_ADDED),
	ENUM_NAME_PAIR(SDL_EVENT_GAMEPAD_REMOVED),
	ENUM_NAME_PAIR(SDL_EVENT_GAMEPAD_REMAPPED),
	ENUM_NAME_PAIR(SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN),
	ENUM_NAME_PAIR(SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION),
	ENUM_NAME_PAIR(SDL_EVENT_GAMEPAD_TOUCHPAD_UP),
	ENUM_NAME_PAIR(SDL_EVENT_GAMEPAD_SENSOR_UPDATE),
	ENUM_NAME_PAIR(SDL_EVENT_GAMEPAD_UPDATE_COMPLETE),
	ENUM_NAME_PAIR(SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED),
	ENUM_NAME_PAIR(SDL_EVENT_FINGER_DOWN),
	ENUM_NAME_PAIR(SDL_EVENT_FINGER_UP),
	ENUM_NAME_PAIR(SDL_EVENT_FINGER_MOTION),
	ENUM_NAME_PAIR(SDL_EVENT_FINGER_CANCELED),
	ENUM_NAME_PAIR(SDL_EVENT_CLIPBOARD_UPDATE),
	ENUM_NAME_PAIR(SDL_EVENT_DROP_FILE),
	ENUM_NAME_PAIR(SDL_EVENT_DROP_TEXT),
	ENUM_NAME_PAIR(SDL_EVENT_DROP_BEGIN),
	ENUM_NAME_PAIR(SDL_EVENT_DROP_COMPLETE),
	ENUM_NAME_PAIR(SDL_EVENT_AUDIO_DEVICE_ADDED),
	ENUM_NAME_PAIR(SDL_EVENT_AUDIO_DEVICE_REMOVED),
	ENUM_NAME_PAIR(SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED),
	ENUM_NAME_PAIR(SDL_EVENT_SENSOR_UPDATE),
	ENUM_NAME_PAIR(SDL_EVENT_RENDER_TARGETS_RESET),
	ENUM_NAME_PAIR(SDL_EVENT_RENDER_DEVICE_RESET),
	ENUM_NAME_PAIR(SDL_EVENT_RENDER_DEVICE_LOST),
};
#undef ENUM_NAME_PAIR
#endif // CHERRYSODA_ENABLE_DEBUG

static STL::List<Keys> s_keyboardKeys;
static STL::HashMap<int, Keys> s_keycodeToKeys = {
	{(int)SDLK_A, Keys::A},
	{(int)SDLK_B, Keys::B},
	{(int)SDLK_C, Keys::C},
	{(int)SDLK_D, Keys::D},
	{(int)SDLK_E, Keys::E},
	{(int)SDLK_F, Keys::F},
	{(int)SDLK_G, Keys::G},
	{(int)SDLK_H, Keys::H},
	{(int)SDLK_I, Keys::I},
	{(int)SDLK_J, Keys::J},
	{(int)SDLK_K, Keys::K},
	{(int)SDLK_L, Keys::L},
	{(int)SDLK_M, Keys::M},
	{(int)SDLK_N, Keys::N},
	{(int)SDLK_O, Keys::O},
	{(int)SDLK_P, Keys::P},
	{(int)SDLK_Q, Keys::Q},
	{(int)SDLK_R, Keys::R},
	{(int)SDLK_S, Keys::S},
	{(int)SDLK_T, Keys::T},
	{(int)SDLK_U, Keys::U},
	{(int)SDLK_V, Keys::V},
	{(int)SDLK_W, Keys::W},
	{(int)SDLK_X, Keys::X},
	{(int)SDLK_Y, Keys::Y},
	{(int)SDLK_Z, Keys::Z},
	{(int)SDLK_0, Keys::D0},
	{(int)SDLK_1, Keys::D1},
	{(int)SDLK_2, Keys::D2},
	{(int)SDLK_3, Keys::D3},
	{(int)SDLK_4, Keys::D4},
	{(int)SDLK_5, Keys::D5},
	{(int)SDLK_6, Keys::D6},
	{(int)SDLK_7, Keys::D7},
	{(int)SDLK_8, Keys::D8},
	{(int)SDLK_9, Keys::D9},
	{(int)SDLK_KP_0, Keys::NumPad0},
	{(int)SDLK_KP_1, Keys::NumPad1},
	{(int)SDLK_KP_2, Keys::NumPad2},
	{(int)SDLK_KP_3, Keys::NumPad3},
	{(int)SDLK_KP_4, Keys::NumPad4},
	{(int)SDLK_KP_5, Keys::NumPad5},
	{(int)SDLK_KP_6, Keys::NumPad6},
	{(int)SDLK_KP_7, Keys::NumPad7},
	{(int)SDLK_KP_8, Keys::NumPad8},
	{(int)SDLK_KP_9, Keys::NumPad9},
	{(int)SDLK_KP_CLEAR, Keys::OemClear},
	{(int)SDLK_KP_DECIMAL, Keys::Decimal},
	{(int)SDLK_KP_DIVIDE, Keys::Divide},
	{(int)SDLK_KP_ENTER, Keys::Enter},
	{(int)SDLK_KP_MINUS, Keys::Subtract},
	{(int)SDLK_KP_MULTIPLY, Keys::Multiply},
	{(int)SDLK_KP_PERIOD, Keys::OemPeriod},
	{(int)SDLK_KP_PLUS, Keys::Add},
	{(int)SDLK_F1, Keys::F1},
	{(int)SDLK_F2, Keys::F2},
	{(int)SDLK_F3, Keys::F3},
	{(int)SDLK_F4, Keys::F4},
	{(int)SDLK_F5, Keys::F5},
	{(int)SDLK_F6, Keys::F6},
	{(int)SDLK_F7, Keys::F7},
	{(int)SDLK_F8, Keys::F8},
	{(int)SDLK_F9, Keys::F9},
	{(int)SDLK_F10, Keys::F10},
	{(int)SDLK_F11, Keys::F11},
	{(int)SDLK_F12, Keys::F12},
	{(int)SDLK_F13, Keys::F13},
	{(int)SDLK_F14, Keys::F14},
	{(int)SDLK_F15, Keys::F15},
	{(int)SDLK_F16, Keys::F16},
	{(int)SDLK_F17, Keys::F17},
	{(int)SDLK_F18, Keys::F18},
	{(int)SDLK_F19, Keys::F19},
	{(int)SDLK_F20, Keys::F20},
	{(int)SDLK_F21, Keys::F21},
	{(int)SDLK_F22, Keys::F22},
	{(int)SDLK_F23, Keys::F23},
	{(int)SDLK_F24, Keys::F24},
	{(int)SDLK_SPACE, Keys::Space},
	{(int)SDLK_UP, Keys::Up},
	{(int)SDLK_DOWN, Keys::Down},
	{(int)SDLK_LEFT, Keys::Left},
	{(int)SDLK_RIGHT, Keys::Right},
	{(int)SDLK_LALT, Keys::LeftAlt},
	{(int)SDLK_RALT, Keys::RightAlt},
	{(int)SDLK_LCTRL, Keys::LeftControl},
	{(int)SDLK_RCTRL, Keys::RightControl},
	{(int)SDLK_LGUI, Keys::LeftSuper},
	{(int)SDLK_RGUI, Keys::RightSuper},
	{(int)SDLK_LSHIFT, Keys::LeftShift},
	{(int)SDLK_RSHIFT, Keys::RightShift},
	{(int)SDLK_APPLICATION, Keys::Apps},
	{(int)SDLK_SLASH, Keys::OemQuestion},
	{(int)SDLK_BACKSLASH, Keys::OemBackslash},
	{(int)SDLK_LEFTBRACKET, Keys::OemOpenBrackets},
	{(int)SDLK_RIGHTBRACKET, Keys::OemCloseBrackets},
	{(int)SDLK_CAPSLOCK, Keys::CapsLock},
	{(int)SDLK_COMMA, Keys::OemComma},
	{(int)SDLK_DELETE, Keys::Delete},
	{(int)SDLK_END, Keys::End},
	{(int)SDLK_BACKSPACE, Keys::Back},
	{(int)SDLK_RETURN, Keys::Enter},
	{(int)SDLK_ESCAPE, Keys::Escape},
	{(int)SDLK_HOME, Keys::Home},
	{(int)SDLK_INSERT, Keys::Insert},
	{(int)SDLK_MINUS, Keys::OemMinus},
	{(int)SDLK_NUMLOCKCLEAR, Keys::NumLock},
	{(int)SDLK_PAGEUP, Keys::PageUp},
	{(int)SDLK_PAGEDOWN, Keys::PageDown},
	{(int)SDLK_PAUSE, Keys::Pause},
	{(int)SDLK_PERIOD, Keys::OemPeriod},
	{(int)SDLK_EQUALS, Keys::OemPlus},
	{(int)SDLK_PRINTSCREEN, Keys::PrintScreen},
	{(int)SDLK_APOSTROPHE, Keys::OemQuotes},
	{(int)SDLK_SCROLLLOCK, Keys::Scroll},
	{(int)SDLK_SEMICOLON, Keys::OemSemicolon},
	{(int)SDLK_SLEEP, Keys::Sleep},
	{(int)SDLK_TAB, Keys::Tab},
	{(int)SDLK_GRAVE, Keys::OemTilde},
	{(int)SDLK_VOLUMEUP, Keys::VolumeUp},
	{(int)SDLK_VOLUMEDOWN, Keys::VolumeDown},
	{(int)SDLK_UNKNOWN, Keys::None},
};

static STL::HashMap<int, Keys> s_scancodeToKeys = {
	{(int)SDL_SCANCODE_A, Keys::A},
	{(int)SDL_SCANCODE_B, Keys::B},
	{(int)SDL_SCANCODE_C, Keys::C},
	{(int)SDL_SCANCODE_D, Keys::D},
	{(int)SDL_SCANCODE_E, Keys::E},
	{(int)SDL_SCANCODE_F, Keys::F},
	{(int)SDL_SCANCODE_G, Keys::G},
	{(int)SDL_SCANCODE_H, Keys::H},
	{(int)SDL_SCANCODE_I, Keys::I},
	{(int)SDL_SCANCODE_J, Keys::J},
	{(int)SDL_SCANCODE_K, Keys::K},
	{(int)SDL_SCANCODE_L, Keys::L},
	{(int)SDL_SCANCODE_M, Keys::M},
	{(int)SDL_SCANCODE_N, Keys::N},
	{(int)SDL_SCANCODE_O, Keys::O},
	{(int)SDL_SCANCODE_P, Keys::P},
	{(int)SDL_SCANCODE_Q, Keys::Q},
	{(int)SDL_SCANCODE_R, Keys::R},
	{(int)SDL_SCANCODE_S, Keys::S},
	{(int)SDL_SCANCODE_T, Keys::T},
	{(int)SDL_SCANCODE_U, Keys::U},
	{(int)SDL_SCANCODE_V, Keys::V},
	{(int)SDL_SCANCODE_W, Keys::W},
	{(int)SDL_SCANCODE_X, Keys::X},
	{(int)SDL_SCANCODE_Y, Keys::Y},
	{(int)SDL_SCANCODE_Z, Keys::Z},
	{(int)SDL_SCANCODE_0, Keys::D0},
	{(int)SDL_SCANCODE_1, Keys::D1},
	{(int)SDL_SCANCODE_2, Keys::D2},
	{(int)SDL_SCANCODE_3, Keys::D3},
	{(int)SDL_SCANCODE_4, Keys::D4},
	{(int)SDL_SCANCODE_5, Keys::D5},
	{(int)SDL_SCANCODE_6, Keys::D6},
	{(int)SDL_SCANCODE_7, Keys::D7},
	{(int)SDL_SCANCODE_8, Keys::D8},
	{(int)SDL_SCANCODE_9, Keys::D9},
	{(int)SDL_SCANCODE_KP_0, Keys::NumPad0},
	{(int)SDL_SCANCODE_KP_1, Keys::NumPad1},
	{(int)SDL_SCANCODE_KP_2, Keys::NumPad2},
	{(int)SDL_SCANCODE_KP_3, Keys::NumPad3},
	{(int)SDL_SCANCODE_KP_4, Keys::NumPad4},
	{(int)SDL_SCANCODE_KP_5, Keys::NumPad5},
	{(int)SDL_SCANCODE_KP_6, Keys::NumPad6},
	{(int)SDL_SCANCODE_KP_7, Keys::NumPad7},
	{(int)SDL_SCANCODE_KP_8, Keys::NumPad8},
	{(int)SDL_SCANCODE_KP_9, Keys::NumPad9},
	{(int)SDL_SCANCODE_KP_CLEAR, Keys::OemClear},
	{(int)SDL_SCANCODE_KP_DECIMAL, Keys::Decimal},
	{(int)SDL_SCANCODE_KP_DIVIDE, Keys::Divide},
	{(int)SDL_SCANCODE_KP_ENTER, Keys::Enter},
	{(int)SDL_SCANCODE_KP_MINUS, Keys::Subtract},
	{(int)SDL_SCANCODE_KP_MULTIPLY, Keys::Multiply},
	{(int)SDL_SCANCODE_KP_PERIOD, Keys::OemPeriod},
	{(int)SDL_SCANCODE_KP_PLUS, Keys::Add},
	{(int)SDL_SCANCODE_F1, Keys::F1},
	{(int)SDL_SCANCODE_F2, Keys::F2},
	{(int)SDL_SCANCODE_F3, Keys::F3},
	{(int)SDL_SCANCODE_F4, Keys::F4},
	{(int)SDL_SCANCODE_F5, Keys::F5},
	{(int)SDL_SCANCODE_F6, Keys::F6},
	{(int)SDL_SCANCODE_F7, Keys::F7},
	{(int)SDL_SCANCODE_F8, Keys::F8},
	{(int)SDL_SCANCODE_F9, Keys::F9},
	{(int)SDL_SCANCODE_F10, Keys::F10},
	{(int)SDL_SCANCODE_F11, Keys::F11},
	{(int)SDL_SCANCODE_F12, Keys::F12},
	{(int)SDL_SCANCODE_F13, Keys::F13},
	{(int)SDL_SCANCODE_F14, Keys::F14},
	{(int)SDL_SCANCODE_F15, Keys::F15},
	{(int)SDL_SCANCODE_F16, Keys::F16},
	{(int)SDL_SCANCODE_F17, Keys::F17},
	{(int)SDL_SCANCODE_F18, Keys::F18},
	{(int)SDL_SCANCODE_F19, Keys::F19},
	{(int)SDL_SCANCODE_F20, Keys::F20},
	{(int)SDL_SCANCODE_F21, Keys::F21},
	{(int)SDL_SCANCODE_F22, Keys::F22},
	{(int)SDL_SCANCODE_F23, Keys::F23},
	{(int)SDL_SCANCODE_F24, Keys::F24},
	{(int)SDL_SCANCODE_SPACE, Keys::Space},
	{(int)SDL_SCANCODE_UP, Keys::Up},
	{(int)SDL_SCANCODE_DOWN, Keys::Down},
	{(int)SDL_SCANCODE_LEFT, Keys::Left},
	{(int)SDL_SCANCODE_RIGHT, Keys::Right},
	{(int)SDL_SCANCODE_LALT, Keys::LeftAlt},
	{(int)SDL_SCANCODE_RALT, Keys::RightAlt},
	{(int)SDL_SCANCODE_LCTRL, Keys::LeftControl},
	{(int)SDL_SCANCODE_RCTRL, Keys::RightControl},
	{(int)SDL_SCANCODE_LGUI, Keys::LeftSuper},
	{(int)SDL_SCANCODE_RGUI, Keys::RightSuper},
	{(int)SDL_SCANCODE_LSHIFT, Keys::LeftShift},
	{(int)SDL_SCANCODE_RSHIFT, Keys::RightShift},
	{(int)SDL_SCANCODE_APPLICATION, Keys::Apps},
	{(int)SDL_SCANCODE_SLASH, Keys::OemQuestion},
	{(int)SDL_SCANCODE_BACKSLASH, Keys::OemBackslash},
	{(int)SDL_SCANCODE_LEFTBRACKET, Keys::OemOpenBrackets},
	{(int)SDL_SCANCODE_RIGHTBRACKET, Keys::OemCloseBrackets},
	{(int)SDL_SCANCODE_CAPSLOCK, Keys::CapsLock},
	{(int)SDL_SCANCODE_COMMA, Keys::OemComma},
	{(int)SDL_SCANCODE_DELETE, Keys::Delete},
	{(int)SDL_SCANCODE_END, Keys::End},
	{(int)SDL_SCANCODE_BACKSPACE, Keys::Back},
	{(int)SDL_SCANCODE_RETURN, Keys::Enter},
	{(int)SDL_SCANCODE_ESCAPE, Keys::Escape},
	{(int)SDL_SCANCODE_HOME, Keys::Home},
	{(int)SDL_SCANCODE_INSERT, Keys::Insert},
	{(int)SDL_SCANCODE_MINUS, Keys::OemMinus},
	{(int)SDL_SCANCODE_NUMLOCKCLEAR, Keys::NumLock},
	{(int)SDL_SCANCODE_PAGEUP, Keys::PageUp},
	{(int)SDL_SCANCODE_PAGEDOWN, Keys::PageDown},
	{(int)SDL_SCANCODE_PAUSE, Keys::Pause},
	{(int)SDL_SCANCODE_PERIOD, Keys::OemPeriod},
	{(int)SDL_SCANCODE_EQUALS, Keys::OemPlus},
	{(int)SDL_SCANCODE_PRINTSCREEN, Keys::PrintScreen},
	{(int)SDL_SCANCODE_APOSTROPHE, Keys::OemQuotes},
	{(int)SDL_SCANCODE_SCROLLLOCK, Keys::Scroll},
	{(int)SDL_SCANCODE_SEMICOLON, Keys::OemSemicolon},
	{(int)SDL_SCANCODE_SLEEP, Keys::Sleep},
	{(int)SDL_SCANCODE_TAB, Keys::Tab},
	{(int)SDL_SCANCODE_GRAVE, Keys::OemTilde},
	{(int)SDL_SCANCODE_VOLUMEUP, Keys::VolumeUp},
	{(int)SDL_SCANCODE_VOLUMEDOWN, Keys::VolumeDown},
	{(int)SDL_SCANCODE_UNKNOWN, Keys::None},
};

namespace entry {
#ifdef __EMSCRIPTEN__
static const char* canvas_id = "#canvas";
#endif // __EMSCRIPTEN__

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
static bool sdlUseWayland()
{
	const char* driver = SDL_GetCurrentVideoDriver();
	return driver != nullptr && SDL_strcmp(driver, "wayland") == 0;
}
#endif // BX_PLATFORM_LINUX || BX_PLATFORM_BSD

static void* sdlNativeWindowHandle(SDL_Window* _window)
{
#ifdef __EMSCRIPTEN__
	return (void*)canvas_id;
#else
	SDL_PropertiesID props = SDL_GetWindowProperties(_window);
#if BX_PLATFORM_WINDOWS
	return SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
#elif BX_PLATFORM_OSX
	return SDL_GetPointerProperty(props, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
#elif BX_PLATFORM_ANDROID
	return SDL_GetPointerProperty(props, SDL_PROP_WINDOW_ANDROID_WINDOW_POINTER, nullptr);
#elif BX_PLATFORM_LINUX || BX_PLATFORM_BSD
	if (sdlUseWayland()) {
		return SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_EGL_WINDOW_POINTER, nullptr);
	}
	return (void*)(uintptr_t)SDL_GetNumberProperty(props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
#else
	return nullptr;
#endif // BX_PLATFORM_
#endif // __EMSCRIPTEN__
}

static inline bool sdlSetWindow(SDL_Window* _window)
{
	bgfx::PlatformData pd;
	pd.ndt = nullptr;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
	if (sdlUseWayland()) {
		pd.ndt =
			SDL_GetPointerProperty(SDL_GetWindowProperties(_window), SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, nullptr);
	}
	else {
		pd.ndt = SDL_GetPointerProperty(SDL_GetWindowProperties(_window), SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr);
	}
#endif // BX_PLATFORM_LINUX || BX_PLATFORM_BSD
	pd.nwh = sdlNativeWindowHandle(_window);

	pd.context = nullptr;
	pd.backBuffer = nullptr;
	pd.backBufferDS = nullptr;
	Graphics::SetPlatformData(&pd);

	return true;
}

static void sdlDestroyWindow(SDL_Window* _window)
{
	if (!_window) return;
	SDL_DestroyWindow(_window);
}

} // namespace entry

void Window::CreateWindow()
{
	int windowWidth = Engine::Instance()->GetWindowWidth();
	int windowHeight = Engine::Instance()->GetWindowHeight();
	String title = Engine::Instance()->GetTitle();

#ifdef ANDROID
	Engine::Instance()->m_windowResizable = false;
	Engine::Instance()->m_fullscreen = true;

	const SDL_DisplayMode* mode = SDL_GetDesktopDisplayMode(SDL_GetPrimaryDisplay());
	if (mode != nullptr) {
		windowWidth = mode->w;
		windowHeight = mode->h;
	}
	m_mainWindow = SDL_CreateWindow(title.c_str(), windowWidth, windowHeight, SDL_WINDOW_FULLSCREEN);
	Engine::Instance()->SetWindowSize(windowWidth, windowHeight);
#else
	bool resizable = Engine::Instance()->m_windowResizable;
	bool fullscreen = Engine::Instance()->m_fullscreen;
	SDL_WindowFlags windowFlag = SDL_WINDOW_HIDDEN;
	if (resizable) windowFlag |= SDL_WINDOW_RESIZABLE;
	if (fullscreen) windowFlag |= SDL_WINDOW_FULLSCREEN;
	int viewWidth = Engine::Instance()->GetWidth();
	int viewHeight = Engine::Instance()->GetHeight();
#ifndef __EMSCRIPTEN__
	// SDL3 takes physical pixels on Windows while the engine's sizes are meant
	// as logical points, so multiply by the display content scale to keep the
	// intended physical size under OS display scaling (macOS/Wayland report
	// 1.0 and are unaffected). See SDL's README-highdpi.
	float contentScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
	const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(SDL_GetPrimaryDisplay());
	if (mode != nullptr && contentScale > 1.0f &&
		(windowWidth * contentScale > mode->w || windowHeight * contentScale > mode->h)) {
		contentScale = 1.0f;
	}
	windowWidth = (int)SDL_ceilf(windowWidth * contentScale);
	windowHeight = (int)SDL_ceilf(windowHeight * contentScale);
	viewWidth = (int)SDL_ceilf(viewWidth * contentScale);
	viewHeight = (int)SDL_ceilf(viewHeight * contentScale);
	Engine::Instance()->m_contentScale = contentScale;
#endif
	m_mainWindow = SDL_CreateWindow(title.c_str(), windowWidth, windowHeight, windowFlag);
	Engine::Instance()->SetWindowSize(windowWidth, windowHeight);
	Engine::Instance()->SetViewSize(viewWidth, viewHeight);
#endif

	InitializeCursor();

	bool showCursor = Engine::Instance()->m_showCursor;
	ShowCursor(showCursor);

	entry::sdlSetWindow(m_mainWindow);
}

void Window::DestroyWindow()
{
	TerminateCursor();

	entry::sdlDestroyWindow(m_mainWindow);
}

void Window::SetSize(int width, int height)
{
	SDL_SetWindowSize(m_mainWindow, width, height);
}

void Window::SetTitle(const String& title)
{
	SDL_SetWindowTitle(m_mainWindow, title.c_str());
}

void Window::SetFullscreen(bool fullscreen)
{
	SDL_SetWindowFullscreen(m_mainWindow, fullscreen);
}

bool Window::IsFullscreen()
{
	return (SDL_GetWindowFlags(m_mainWindow) & SDL_WINDOW_FULLSCREEN) != 0;
}

void Window::GetPosition(int* x, int* y)
{
	SDL_GetWindowPosition(m_mainWindow, x, y);
}

void Window::SetMousePosition(int x, int y)
{
	SDL_WarpMouseInWindow(m_mainWindow, (float)x, (float)y);
}

void Window::Hide()
{
	SDL_HideWindow(m_mainWindow);
}

void Window::Show()
{
	SDL_ShowWindow(m_mainWindow);
	// SDL 3 no longer activates the app at launch on macOS 14+, and showing the
	// window alone doesn't bring the process to the foreground.
	SDL_RaiseWindow(m_mainWindow);
}

#define CHERRYSODA_SWITCH_WEB_CURSOR(CURSOR_X) EM_ASM(document.getElementById("canvas").style.cursor = (CURSOR_X));

void Window::ShowCursor(bool show)
{
	static bool s_showCache = true;
	if (show == s_showCache) return;
	if (show) {
		SDL_ShowCursor();
	}
	else {
		SDL_HideCursor();
	}
#ifdef __EMSCRIPTEN__
	if (show) {
		CHERRYSODA_SWITCH_WEB_CURSOR('auto');
	}
	else {
		CHERRYSODA_SWITCH_WEB_CURSOR('none');
	}
#endif // __EMSCRIPTEN__
	s_showCache = show;
}

void Window::SetCursor(CursorTypes cursor)
{
	SDL_SetCursor(s_mouseCursors[(int)cursor] ? s_mouseCursors[(int)cursor] : s_mouseCursors[(int)CursorTypes::Arrow]);
#ifdef __EMSCRIPTEN__
	switch (cursor) {
	case CursorTypes::Arrow:
		CHERRYSODA_SWITCH_WEB_CURSOR('auto');
		break;
	case CursorTypes::TextInput:
		CHERRYSODA_SWITCH_WEB_CURSOR('text');
		break;
	case CursorTypes::ResizeAll:
		CHERRYSODA_SWITCH_WEB_CURSOR('move');
		break;
	case CursorTypes::ResizeNS:
		CHERRYSODA_SWITCH_WEB_CURSOR('ns-resize');
		break;
	case CursorTypes::ResizeEW:
		CHERRYSODA_SWITCH_WEB_CURSOR('ew-resize');
		break;
	case CursorTypes::ResizeNESW:
		CHERRYSODA_SWITCH_WEB_CURSOR('nesw-resize');
		break;
	case CursorTypes::ResizeNWSE:
		CHERRYSODA_SWITCH_WEB_CURSOR('nwse-resize');
		break;
	case CursorTypes::Hand:
		CHERRYSODA_SWITCH_WEB_CURSOR('pointer');
		break;
	case CursorTypes::NotAllowed:
		CHERRYSODA_SWITCH_WEB_CURSOR('not-allowed');
		break;
	default:
		CHERRYSODA_SWITCH_WEB_CURSOR('auto');
		break;
	}
#endif // __EMSCRIPTEN__
}

#undef CHERRYSODA_SWITCH_WEB_CURSOR

void Window::InitializeCursor()
{
	s_mouseCursors[(int)CursorTypes::Arrow] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
	s_mouseCursors[(int)CursorTypes::TextInput] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_TEXT);
	s_mouseCursors[(int)CursorTypes::ResizeAll] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_MOVE);
	s_mouseCursors[(int)CursorTypes::ResizeNS] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NS_RESIZE);
	s_mouseCursors[(int)CursorTypes::ResizeEW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_EW_RESIZE);
	s_mouseCursors[(int)CursorTypes::ResizeNESW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NESW_RESIZE);
	s_mouseCursors[(int)CursorTypes::ResizeNWSE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NWSE_RESIZE);
	s_mouseCursors[(int)CursorTypes::Hand] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);
	s_mouseCursors[(int)CursorTypes::NotAllowed] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NOT_ALLOWED);
}

void Window::TerminateCursor()
{
	SDL_DestroyCursor(s_mouseCursors[(int)CursorTypes::Arrow]);
	SDL_DestroyCursor(s_mouseCursors[(int)CursorTypes::TextInput]);
	SDL_DestroyCursor(s_mouseCursors[(int)CursorTypes::ResizeAll]);
	SDL_DestroyCursor(s_mouseCursors[(int)CursorTypes::ResizeNS]);
	SDL_DestroyCursor(s_mouseCursors[(int)CursorTypes::ResizeEW]);
	SDL_DestroyCursor(s_mouseCursors[(int)CursorTypes::ResizeNESW]);
	SDL_DestroyCursor(s_mouseCursors[(int)CursorTypes::ResizeNWSE]);
	SDL_DestroyCursor(s_mouseCursors[(int)CursorTypes::Hand]);
	SDL_DestroyCursor(s_mouseCursors[(int)CursorTypes::NotAllowed]);
}

void Window::Resizable(bool resizable)
{
	SDL_SetWindowResizable(m_mainWindow, resizable);
}

void Window::SetTextInputEnabled(bool enable)
{
	if (m_mainWindow == nullptr) return;
	// SDL3 only delivers SDL_EVENT_TEXT_INPUT while text input is enabled for the window
	if (enable) {
		if (!SDL_TextInputActive(m_mainWindow)) {
			SDL_StartTextInput(m_mainWindow);
		}
	}
	else if (SDL_TextInputActive(m_mainWindow)) {
		SDL_StopTextInput(m_mainWindow);
	}
}

void Window::PollEvents()
{
	SDL_Event event;
	Keys key = Keys::None;
#ifdef CHERRYSODA_ENABLE_DEBUG
	bool first = true;
#endif // CHERRYSODA_ENABLE_DEBUG
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
		case SDL_EVENT_QUIT:
			Engine::Instance()->Exit();
			break;

		case SDL_EVENT_KEY_DOWN: {
			if (STL::TryGetValue(s_scancodeToKeys, (int)event.key.scancode, key) &&
				!STL::Contains(s_keyboardKeys, key)) {
				STL::Add(s_keyboardKeys, key);
			}

			break;
		}
		case SDL_EVENT_KEY_UP:
			if (STL::TryGetValue(s_scancodeToKeys, (int)event.key.scancode, key)) {
				STL::Remove(s_keyboardKeys, key);
			}
			break;

		case SDL_EVENT_MOUSE_WHEEL:
			MInput::ms_internalMouseWheel += (int)(event.wheel.y * 120);
			break;

		case SDL_EVENT_FINGER_DOWN:
		case SDL_EVENT_FINGER_MOTION:
		case SDL_EVENT_FINGER_UP:
			break;

		case SDL_EVENT_GAMEPAD_ADDED:
			MInput::AddControllerInstance((int)event.gdevice.which);
			break;
		case SDL_EVENT_GAMEPAD_REMOVED:
			MInput::RemoveControllerInstance((int)event.gdevice.which);
			break;

		case SDL_EVENT_WINDOW_FOCUS_GAINED:
			Engine::Instance()->IsActive(true);
			break;

		case SDL_EVENT_WINDOW_FOCUS_LOST:
			Engine::Instance()->IsActive(false);
			break;

		case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
			Engine::Instance()->OnClientSizeChanged(event.window.data1, event.window.data2);
			break;

		case SDL_EVENT_TEXT_INPUT: {
			const SDL_TextInputEvent& tev = event.text;
			Engine::Instance()->OnTextInput(tev.text);
		} break;

		default:
			break;
		}
	}
	MInput::SetKeyboardKeys(s_keyboardKeys);
}

bool Window::Initialize()
{
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		return false;
	}

	SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight Portrait");

	return true;
}

void Window::Terminate()
{
	SDL_Quit();
}

} // namespace cherrysoda
