#ifndef _CHERRYSODA_GRAPHICS_GRAPHICS_H_
#define _CHERRYSODA_GRAPHICS_GRAPHICS_H_

#include <CherrySoda/Util/Color.h>

namespace cherrysoda {

class Graphics
{
public:
	static void Init();
	static void Terminate();

	void RenderFrame();
	void UpdateView();
	void SetClearColor(const Color& color);
	void Touch();
	void SetVsyncEnabled(bool vsyncEnabled);
	void SetViewport(int x, int y, int w, int h);

	static Graphics* Instance() { return ms_instance; };

private:
	friend class Engine;

	Graphics();

	bool m_vsyncEnabled = true;

	static Graphics* ms_instance;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_GRAPHICS_H_