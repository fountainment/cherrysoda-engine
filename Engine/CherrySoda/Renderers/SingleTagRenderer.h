#ifndef _CHERRYSODA_RENDERERS_SINGLETAGRENDERER_H_
#define _CHERRYSODA_RENDERERS_SINGLETAGRENDERER_H_

#include <CherrySoda/Renderers/RendererBase.h>
#include <CherrySoda/Util/BitTag.h>

namespace cherrysoda {

class Scene;

class SingleTagRenderer : public RendererBase
{
public:
	SingleTagRenderer(const BitTag& tag)
	: m_tag(tag)
	{
	}

	void Render(Scene* scene) override;

private:
	BitTag m_tag;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_RENDERERS_SINGLETAGRENDERER_H_ 
