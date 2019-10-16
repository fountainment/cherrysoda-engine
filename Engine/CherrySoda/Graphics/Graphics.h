#ifndef _CHERRYSODA_GRAPHICS_GRAPHICS_H_
#define _CHERRYSODA_GRAPHICS_GRAPHICS_H_

#include <CherrySoda/Utility/Color.h>

namespace cherrysoda {

class Graphics
{
public:
	static void Init();
	static void LoadGraphicsAPI();
	static void SetClearColor(const Color& color);
	static void SetViewport(int x, int y, int w, int h);
	static void ClearColorAndDepth();

#ifdef CHERRYSODA_GLES2
	static void TriangleGLES2();
#endif

	static Graphics* GetInstance() { return ms_instance; };

private:
	Graphics();

	static Graphics* ms_instance;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_GRAPHICS_H_