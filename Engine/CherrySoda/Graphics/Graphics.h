#ifndef _CHERRYSODA_GRAPHICS_GRAPHICS_H_
#define _CHERRYSODA_GRAPHICS_GRAPHICS_H_

#include <CherrySoda/Utility/Color.h>

namespace cherrysoda {

class Graphics
{
public:
	static void Init();
	static void Terminate();
	static void RenderFrame();
	static void Reset(bool vsyncEnabled = true);
	static void SetClearColor(const Color& color);
	static void SetViewport(int x, int y, int w, int h);

	static Graphics* GetInstance() { return ms_instance; };

private:
	Graphics();

	static Graphics* ms_instance;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_GRAPHICS_H_