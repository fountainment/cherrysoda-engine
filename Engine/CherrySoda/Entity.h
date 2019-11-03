#ifndef _CHERRYSODA_ENTITY_H_
#define _CHERRYSODA_ENTITY_H_

#include <CherrySoda/InternalUtilities/ComponentList.h>

#include <glm/vec2.hpp>
#include <vector>

namespace cherrysoda {

class Camera;
class Component;
class ComponentList;
class Scene;

class Entity
{
public:
	Entity() : Entity(glm::vec2(0.f)) {}
	Entity(const glm::vec2& position);
	
	virtual void SceneBegin(Scene* scene);
	virtual void SceneEnd(Scene* scene);
	virtual void Awake(Scene* scene);

	virtual void Update();
	virtual void Render();

	virtual void DebugRender(Camera* camera);

	virtual void HandleGraphicsReset();
	virtual void HandleGraphicsCreate();

	void RemoveSelf();

	void Add(Component* component); 
	void Remove(Component* component);
	void Add(ComponentList::IterableComponents& components); 
	void Remove(ComponentList::IterableComponents& components);

	template<class T>
	T Get()
	{
		return m_components != nullptr ? m_components->Get<T>() : nullptr;
	}

	inline Scene* GetScene() { return m_scene; }

private:
	friend class EntityList;

	virtual void Added(Scene* scene);
	virtual void Removed(Scene* scene);

	bool m_active = true;
	bool m_visible = true;
	bool m_collidable = false;
	glm::vec2 m_position = glm::vec2(0.f);

	Scene* m_scene = nullptr;
	ComponentList* m_components = nullptr;

	int m_id = 0;
	int m_tag = 0;

	// Collider* collider;

	int m_depth = 0;
	double m_actualDepth = 0.0;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_ENTITY_H_
