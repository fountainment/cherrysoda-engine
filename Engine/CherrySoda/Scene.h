#ifndef _CHERRYSODA_SCENE_H_
#define _CHERRYSODA_SCENE_H_

#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class Entity;
class EntityList;
class Renderer;
class RendererList;

class Scene
{
public:
	Scene();

	virtual void Begin();
	virtual void End(); 

	virtual void BeforeUpdate();	
	virtual void Update();	
	virtual void AfterUpdate();	

	virtual void BeforeRender();
	virtual void Render();
	virtual void AfterRender();

	virtual void HandleGraphicsReset();
	virtual void HandleGraphicsCreate();

	void Add(Entity* entity);
	void Remove(Entity* entity);

	void Add(Renderer* renderer);
	void Remove(Renderer* renderer);

	EntityList* Entities() { return m_entities; }

	void OnEndOfFrame(STL::Action func);

private:
	EntityList* m_entities = nullptr;
	RendererList* m_rendererList = nullptr;

	float m_timeActive = 0.f;
	float m_rawTimeActive = 0.f;

	bool m_focused = false;
	bool m_paused = false;

	STL::Vector<STL::Action> m_onEndOfFrame;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_SCENE_H_
