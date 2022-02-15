#ifndef _CHERRYSODA_INTERNALUTILITIES_RENDERERLIST_H_
#define _CHERRYSODA_INTERNALUTILITIES_RENDERERLIST_H_

#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class Renderer;
class Scene;

class RendererList
{
public:
	CHERRYSODA_ITERABLE(m_renderers);

	typedef STL::List<Renderer*> IterableRenderers;

	void MoveToFront(Renderer* renderer);
	void Add(Renderer* renderer);
	void Remove(Renderer* renderer);

	Renderer* First();

private:
	friend class Scene;

	RendererList(Scene* scene);

	void UpdateLists();
	void Update();
	void BeforeRender();
	void Render();
	void AfterRender();

	IterableRenderers m_renderers;
	IterableRenderers m_adding;
	IterableRenderers m_removing;
	Scene* m_scene = nullptr;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_INTERNALUTILITIES_RENDERERLIST_H_
