#ifndef _CHERRYSODA_INTERNALUTILITIES_ENTITYLIST_H_
#define _CHERRYSODA_INTERNALUTILITIES_ENTITYLIST_H_

#include <CherrySoda/Entity.h>

#include <unordered_set>
#include <vector>

namespace cherrysoda {

class Scene;

class EntityList
{
public:
	typedef std::vector<Entity*> IterableEntities;
	typedef std::unordered_set<Entity*> HashSetEntities;

	void UpdateLists();

	void Add(Entity* entity);
	void Remove(Entity* entity);

	void Render();

private:
	friend class Scene;

	EntityList(Scene* scene);

	void Update();

	IterableEntities::iterator begin() { return m_entities.begin(); }
	IterableEntities::iterator end() { return m_entities.end(); }

	IterableEntities m_entities;
	IterableEntities m_toAdd;
	IterableEntities m_toAwake;
	IterableEntities m_toRemove;

	HashSetEntities m_current;
	HashSetEntities m_adding;
	HashSetEntities m_removing;

	Scene* m_scene;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_INTERNALUTILITIES_ENTITYLIST_H_