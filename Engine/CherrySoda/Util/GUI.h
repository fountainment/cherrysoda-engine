#ifndef _CHERRYSODA_UTIL_GUI_H_
#define _CHERRYSODA_UTIL_GUI_H_

#include <CherrySoda/Graphics/Effect.h>
#include <CherrySoda/Graphics/Texture.h>
#include <CherrySoda/Util/NumType.h>

namespace cherrysoda {

class GUI
{
public:
	static inline void SetEffect(const Effect& effect) { ms_guiEffect = effect; }

	static inline void Disable() { ms_enabled = false; }
	static inline void Enable() { ms_enabled = true; }
	static inline bool Enabled() { return ms_enabled;  }

	static inline void DisableInternalConsole() { ms_internalConsoleEnabled = false; }
	static inline void EnableInternalConsole() { ms_internalConsoleEnabled = true; }
	static inline bool InternalConsoleEnabled() { return ms_internalConsoleEnabled; }

	static void BuildFontTexture();

	static void TextInput(const char* text);

	static inline bool ConsoleWindowFocused() { return ms_consoleFocused; }
	static inline bool SliderWindowFocused() { return ms_sliderFocused; }

private:
	friend class Engine;

	static void Initialize();
	static void Terminate();

	static void Update();
	static void Render();

	static void UpdateConsole();

	static Effect ms_guiEffect;
	static Texture2D ms_fontTexture;
	static type::UInt16 ms_guiRenderPass;
	static bool ms_enabled;
	static bool ms_frameStarted;
	static bool ms_consoleFocused;
	static bool ms_sliderFocused;
	static bool ms_internalConsoleEnabled;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_GUI_H_
