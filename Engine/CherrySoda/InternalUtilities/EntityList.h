#ifndef _CHERRYSODA_INTERNALUTILITIES_ENTITYLIST_H_
#define _CHERRYSODA_INTERNALUTILITIES_ENTITYLIST_H_

#include <CherrySoda/Entity.h>

#include <CherrySoda/Util/STL.h>

#include <list>
#include <unordered_set>

namespace cherrysoda {

class Scene;

class EntityList
{
public:
	typedef std::list<Entity*> IterableEntities;
	typedef std::unordered_set<Entity*> HashSetEntities;

	void UpdateLists();

	void Add(Entity* entity);
	void Remove(Entity* entity);

	void Render();

private:
	friend class Scene;

	EntityList(Scene* scene);

	void Update();

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