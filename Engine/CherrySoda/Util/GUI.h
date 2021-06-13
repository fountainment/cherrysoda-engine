#ifndef _CHERRYSODA_UTIL_GUI_H_
#define _CHERRYSODA_UTIL_GUI_H_

#include <CherrySoda/Graphics/Effect.h>
#include <CherrySoda/Graphics/Texture.h>
#include <CherrySoda/Util/NumType.h>

namespace cherrysoda {

class GUI
{
public:
	static void SetEffect(const Effect& effect) { ms_guiEffect = effect; }

	static void Disable() { ms_disable = true; }
	static void Enable() { ms_disable = false; }

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

	static Effect ms_guiEffect;
	static Texture2D ms_fontTexture;
	static type::UInt16 ms_guiRenderPass;
	static bool ms_disable;
	static bool ms_frameStarted;
	static bool ms_consoleFocused;
	static bool ms_sliderFocused;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_GUI_H_
