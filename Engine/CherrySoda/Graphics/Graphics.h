#ifndef _CHERRYSODA_GRAPHICS_GRAPHICS_H_
#define _CHERRYSODA_GRAPHICS_GRAPHICS_H_

#include <CherrySoda/Utility/Color.h>

namespace cherrysoda {

class Graphics
{
public:
	static void Init();
	static void Terminate();

	void RenderFrame();
	void UpdateView();
	void SetClearColor(const Color& color);
	void SetViewport(int x, int y, int w, int h);

	static Graphics* GetInstance() { return ms_instance; };

private:
	friend class Engine;

	Graphics();

	bool m_vsyncEnabled = true;

	static Graphics* ms_instance;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_GRAPHICS_H_