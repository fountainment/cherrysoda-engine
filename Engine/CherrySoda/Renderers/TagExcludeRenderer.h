#ifndef _CHERRYSODA_RENDERERS_TAGEXCLUDERENDERER_H_
#define _CHERRYSODA_RENDERERS_TAGEXCLUDERENDERER_H_

#include <CherrySoda/Renderers/RendererBase.h>

namespace cherrysoda {

class Scene;

class TagExcludeRenderer : public RendererBase
{
public:
	TagExcludeRenderer(int excludeTag)
	: m_excludeTag(excludeTag)
	{}

	void Render(Scene* scene) override;

private:
	int m_excludeTag;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_RENDERERS_TAGEXCLUDERENDERER_H_
