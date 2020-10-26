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

private:
	friend class Engine;

	static void Initialize();
	static void Update();
	static void Render();

	static Effect ms_guiEffect;
	static Texture2D ms_fontTexture;
	static type::UInt16 ms_guiRenderPass;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_GUI_H_
