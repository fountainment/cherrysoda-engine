#ifndef _CHERRYSODA_COMPONENTS_COMPONENT_H_
#define _CHERRYSODA_COMPONENTS_COMPONENT_H_

#include <CherrySoda/Entity.h>
#include <CherrySoda/Util/Math.h>

namespace cherrysoda {

class Camera;
class Entity;
class Scene;

class Component
{
public:
	friend class ComponentList;

	Component(bool active, bool visible);

	virtual void Added(Entity* entity);
	virtual void Removed(Entity* entity);

	virtual void EntityAdded(Scene* scene);
	virtual void EntityRemoved(Scene* scene);

	virtual void SceneEnd(Scene* scene);
	virtual void EntityAwake();

	virtual void Update();
	virtual void Render();

	virtual void DebugRender(Camera* camera); 

	virtual void HandleGraphicsReset();
	virtual void HandleGraphicsCreate();

	void RemoveSelf();	

	template <class T>
	T* SceneAs() { return typeid(T*) == typeid(GetScene()) ? static_cast<T*>(GetScene()) : nullptr; }

	template <class T>
	T* EntityAs() { return typeid(T*) == typeid(GetEntity()) ? static_cast<T*>(GetEntity()) : nullptr; }

	inline Entity* GetEntity() { return m_entity; };
	inline const Entity* GetEntity() const { return m_entity; };
	inline Scene* GetScene() const { return m_entity != nullptr ? m_entity->GetScene() : nullptr; }

	CHERRYSODA_GETTER_SETTER_OF_BOOL(Active, m_active);
	CHERRYSODA_GETTER_SETTER_OF_BOOL(Visible, m_visible);

private:
	Entity* m_entity = nullptr;

	bool m_active;
	bool m_visible;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_COMPONENT_H_
