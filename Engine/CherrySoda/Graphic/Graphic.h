#ifndef _CHERRYSODA_GRAPHIC_H_
#define _CHERRYSODA_GRAPHIC_H_

#include <CherrySoda/Utility/Color.h>

namespace cherrysoda {

class Graphic
{
public:
	static void LoadGL();
	static void SetClearColor(const Color& color);
	static void SetViewport(int x, int y, int w, int h);
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHIC_H_