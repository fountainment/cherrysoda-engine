#ifndef _CHERRYSODA_INTERNALUTILITIES_COMPONENTLIST_H_
#define _CHERRYSODA_INTERNALUTILITIES_COMPONENTLIST_H_

#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class Camera;
class Entity;

class ComponentList
{
public:
	typedef STL::List<Component*> IterableComponents;
	typedef STL::HashSet<Component*> HashSetComponents;

	enum class LockModes { Open, Locked, Error };

	template<class T>
	T* Get()
	{
		for (auto component : m_components) {
			if (T::ComponentTypeID() == component->TypeID()) {
				return static_cast<T*>(component);
			}
		}
		return nullptr;
	}

	inline Entity* GetEntity() { return m_entity; }

private:
	CHERRYSODA_ITERABLE(m_components);

	friend class Entity;

	ComponentList(Entity* entity);

	inline LockModes LockMode() const { return m_lockMode; }
	void LockMode(LockModes lockMode);

	void Add(Component* component);
	void Remove(Component* component);
	void Add(const IterableComponents& component);
	void Remove(const IterableComponents& component);

	void Update();
	void Render();

	void DebugRender(Camera* camera);

	void HandleGraphicsReset();
	void HandleGraphicsCreate();

	LockModes m_lockMode;

	IterableComponents m_components;
	IterableComponents m_toAdd;
	IterableComponents m_toRemove;

	HashSetComponents m_current;
	HashSetComponents m_adding;
	HashSetComponents m_removing;

	Entity* m_entity;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_INTERNALUTILITIES_COMPONENTLIST_H_
