#ifndef _CHERRYSODA_COMPONENTS_COMPONENT_H_
#define _CHERRYSODA_COMPONENTS_COMPONENT_H_

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
	T* SceneAs() { return dynamic_cast<T*>(GetScene()); }

	template <class T>
	T* EntityAs() { return dynamic_cast<T*>(GetEntity()); }

	inline Entity* GetEntity() { return m_entity; };
	inline const Entity* GetEntity() const { return m_entity; };
	Scene* GetScene();

	CHERRYSODA_GETTER_SETTER_OF_TYPE(bool, Active, m_active);
	CHERRYSODA_GETTER_SETTER_OF_TYPE(bool, Visible, m_visible);

private:
	Entity* m_entity = nullptr;

	bool m_active;
	bool m_visible;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_COMPONENT_H_
