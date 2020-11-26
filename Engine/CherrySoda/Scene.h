#ifndef _CHERRYSODA_SCENE_H_
#define _CHERRYSODA_SCENE_H_

#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class BitTag;
class Entity;
class EntityList;
class Renderer;
class RendererList;
class TagLists;

class Scene
{
public:
	Scene();
	virtual ~Scene();

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
	
	const STL::List<Entity*> GetEntitiesByTagMask(int mask) const;
	const STL::List<Entity*> GetEntitiesExcludingTagMask(int mask) const;

	void Add(Renderer* renderer);
	void Remove(Renderer* renderer);

	void _SetActualDepth(Entity* entity);

	EntityList* Entities() { return m_entities; }
	TagLists* Tags() { return m_tagLists; }
	RendererList* Renderers() { return m_rendererList; }
	Entity* HelperEntity() { return m_helperEntity; }
	Renderer* FirstRenderer();

	const STL::List<Entity*>& operator [] (const BitTag& tag) const;

	void AddActionOnEndOfFrame(STL::Action<> func);

private:
	EntityList* m_entities = nullptr;
	TagLists* m_tagLists = nullptr;
	RendererList* m_rendererList = nullptr;

	Entity* m_helperEntity = nullptr;

	float m_timeActive = 0.f;
	float m_rawTimeActive = 0.f;

	bool m_focused = false;
	bool m_paused = false;

	STL::Vector<STL::Action<>> m_onEndOfFrame;
	STL::HashMap<int,double> m_actualDepthLookup;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_SCENE_H_
