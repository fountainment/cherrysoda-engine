#ifndef _CHERRYSODA_GRAPHIC_H_
#define _CHERRYSODA_GRAPHIC_H_

#include <CherrySoda/Utility/Color.h>

namespace cherrysoda {

class Graphic
{
public:
	static void LoadGraphicAPI();
	static void SetClearColor(const Color& color);
	static void SetViewport(int x, int y, int w, int h);
	static void ClearColorAndDepth();
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHIC_H_