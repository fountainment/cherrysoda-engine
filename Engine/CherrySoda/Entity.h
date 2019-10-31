#ifndef _CHERRYSODA_ENTITY_H_
#define _CHERRYSODA_ENTITY_H_

#include <glm/vec2.hpp>

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
	
	void SceneBegin(Scene* scene) {};
	void SceneEnd(Scene* scene) {};

	void Update();
	void Render();

	void DebugRender(const Camera* camera);

	void RemoveSelf();

	void Add(Component* component); 
	void Remove(Component* component);

private:
	friend class EntityList;

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
	double m_actualDepth = 0.f;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_ENTITY_H_
