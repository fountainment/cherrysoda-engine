#ifndef _CHERRYSODA_INTERNALUTILITIES_COMPONENTLIST_H_
#define _CHERRYSODA_INTERNALUTILITIES_COMPONENTLIST_H_

#include <CherrySoda/Util/STL.h>

#include <list>
#include <unordered_set>

namespace cherrysoda {

class Camera;
class Component;
class Entity;

class ComponentList
{
public:
	typedef std::list<Component*> IterableComponents;
	typedef std::unordered_set<Component*> HashSetComponents;

	enum class LockModes { Open, Locked, Error };

	inline Entity* GetEntity() { return m_entity; }
	
private:
	friend class Entity;

	ComponentList(Entity* entity);

	inline LockModes LockMode() { return m_lockMode; }
	void LockMode(LockModes lockMode);

	void Add(Component* component);
	void Remove(Component* component);
	void Add(IterableComponents& component);
	void Remove(IterableComponents& component);

	void Update();
	void Render();

	void DebugRender(Camera* camera);

	LockModes m_lockMode;

	IterableComponents m_components;
	IterableComponents m_toAdd;
	IterableComponents m_toRemove;

	HashSetComponents m_current;
	HashSetComponents m_adding;
	HashSetComponents m_removing;

	Entity* m_entity;

	CHERRYSODA_ITERABLE(m_components);
};

} // namespace cherrysoda

#endif // _CHERRYSODA_INTERNALUTILITIES_COMPONENTLIST_H_ 