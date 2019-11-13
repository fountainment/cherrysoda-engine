#ifndef _CHERRYSODA_GRAPHICS_GRAPHICS_H_
#define _CHERRYSODA_GRAPHICS_GRAPHICS_H_

#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/NumType.h>

namespace cherrysoda {

class Camera;

class Graphics
{
public:
	static void Init();
	static void Terminate();

	void RenderFrame();

	inline void RenderPass(type::UInt16 renderPassId) { m_renderPassId = renderPassId; }
	inline type::UInt16 RenderPass() { return m_renderPassId; }
	void UpdateView();
	void SetClearColor(const Color& color);
	void Touch();
	void SetVsyncEnabled(bool vsyncEnabled);
	void SetViewport(int x, int y, int w, int h);
	void SetCamera(Camera* camera);

	static Graphics* Instance() { return ms_instance; };

private:
	friend class Engine;

	Graphics();

	type::UInt16 m_renderPassId = 0;	
	bool m_vsyncEnabled = true;

	static Graphics* ms_instance;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_GRAPHICS_H_