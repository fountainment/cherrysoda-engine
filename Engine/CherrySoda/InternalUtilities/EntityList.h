#ifndef _CHERRYSODA_INTERNALUTILITIES_ENTITYLIST_H_
#define _CHERRYSODA_INTERNALUTILITIES_ENTITYLIST_H_

#include <CherrySoda/Entity.h>

#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class Camera;
class Scene;

class EntityList
{
public:
	typedef STL::List<Entity*> IterableEntities;
	typedef STL::HashSet<Entity*> HashSetEntities;

	void UpdateLists();

	void Add(Entity* entity);
	void Remove(Entity* entity);

	void Render();
	void DebugRender(Camera* camera);

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

	CHERRYSODA_ITERABLE(m_entities);
};

} // namespace cherrysoda

#endif // _CHERRYSODA_INTERNALUTILITIES_ENTITYLIST_H_