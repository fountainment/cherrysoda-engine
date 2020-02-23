#ifndef _CHERRYSODA_RENDERERS_SINGLETAGRENDERER_H_
#define _CHERRYSODA_RENDERERS_SINGLETAGRENDERER_H_

#include <CherrySoda/Graphics/Effect.h>
#include <CherrySoda/Renderers/Renderer.h>
#include <CherrySoda/Util/BitTag.h>
#include <CherrySoda/Util/Camera.h>

namespace cherrysoda {

class Scene;

class SingleTagRenderer : public Renderer
{
public:
	SingleTagRenderer(const BitTag& tag)
	: m_tag(tag)
	{
	}

	void Render(Scene* scene) override;

	inline Camera* GetCamera() { return &m_camera; }
	inline Effect* GetEffect() { return &m_effect; }

	inline void SetEffect(const Effect& effect) { *GetEffect() = effect; }

private:
	BitTag m_tag;
	Camera m_camera;
	Effect m_effect;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_RENDERERS_SINGLETAGRENDERER_H_ 
