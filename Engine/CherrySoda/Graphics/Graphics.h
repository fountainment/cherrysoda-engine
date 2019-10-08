#ifndef _CHERRYSODA_GRAPHICS_H_
#define _CHERRYSODA_GRAPHICS_H_

#include <CherrySoda/Utility/Color.h>

namespace cherrysoda {

class Graphics
{
public:
	static void LoadGraphicsAPI();
	static void SetClearColor(const Color& color);
	static void SetViewport(int x, int y, int w, int h);
	static void ClearColorAndDepth();

#ifdef CHERRYSODA_GLES2
	static void TriangleGLES2();
#endif
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_H_