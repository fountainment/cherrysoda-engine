#ifndef _CHERRYSODA_INTERNALUTILITIES_ENTITYLIST_H_
#define _CHERRYSODA_INTERNALUTILITIES_ENTITYLIST_H_

#include <CherrySoda/Entity.h>

#include <CherrySoda/Util/BitTag.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class Camera;
class Scene;

class EntityList
{
public:
	CHERRYSODA_ITERABLE(m_entities);

	typedef STL::List<Entity*> IterableEntities;
	typedef STL::HashSet<Entity*> HashSetEntities;

	void UpdateLists();

	void Add(Entity* entity);
	void Remove(Entity* entity);

	inline void MarkUnsorted() { m_unsorted = true; }

	void Render();
	void RenderOnly(BitTagValueType matchTags);
	void RenderOnlyFullMatch(BitTagValueType matchTags);
	void RenderExcept(BitTagValueType excludeTags);
	void DebugRender(Camera* camera);

	Entity* First();

	static bool CompareDepth(Entity* a, Entity* b) { return a->m_actualDepth > b->m_actualDepth; }

private:
	friend class Scene;

	EntityList(Scene* scene);

	void Update();

	void HandleGraphicsReset();
	void HandleGraphicsCreate();

	IterableEntities m_entities;
	IterableEntities m_toAdd;
	IterableEntities m_toAwake;
	IterableEntities m_toRemove;

	HashSetEntities m_current;
	HashSetEntities m_adding;
	HashSetEntities m_removing;

	bool m_unsorted = false;

	Scene* m_scene = nullptr;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_INTERNALUTILITIES_ENTITYLIST_H_
