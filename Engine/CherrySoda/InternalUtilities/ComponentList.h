#ifndef _CHERRYSODA_INTERNALUTILITIES_COMPONENTLIST_H_
#define _CHERRYSODA_INTERNALUTILITIES_COMPONENTLIST_H_

#include <vector>

namespace cherrysoda {

class Camera;
class Component;
class Entity;

class ComponentList
{
private:
	friend class Entity;

	ComponentList(Entity* entity);

	void Add(Component* component);
	void Remove(Component* component);

	void Update();
	void Render();

	void DebugRender(const Camera* camera);

	std::vector<Component*> m_components;
	std::vector<Component*> m_toAdd;
	std::vector<Component*> m_toRemove;

	Entity* m_entity;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_INTERNALUTILITIES_COMPONENTLIST_H_ 