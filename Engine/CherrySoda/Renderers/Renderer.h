#ifndef _CHERRYSODA_RENDERERS_RENDERER_H_
#define _CHERRYSODA_RENDERERS_RENDERER_H_

namespace cherrysoda {

class Scene;

class Renderer
{
public:
	virtual void Update(Scene* scene) { }
	virtual void BeforeRender(Scene* scene) { }
	virtual void Render(Scene* scene) = 0;
	virtual void AfterRender(Scene* scene) { }

	inline bool Visible() { return m_visible; }
	inline void Visible(bool v) { m_visible = v; }
private:
	bool m_visible = true;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_RENDERERS_RENDERER_H_