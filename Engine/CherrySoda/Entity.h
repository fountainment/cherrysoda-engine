#ifndef _CHERRYSODA_ENTITY_H_
#define _CHERRYSODA_ENTITY_H_

#include <CherrySoda/Components/Component.h>

#include <glm/vec2.hpp>
#include <memory>

namespace cherrysoda {

class Entity
{
public:
	Entity() : Entity(glm::vec2(0.f)) {}
	Entity(const glm::vec2& position)
	{
		m_position = position;
		// m_components = new ComponentList;
	}
	
	virtual void SceneBegin(/*Scene scene*/) {};
	virtual void SceneEnd(/*Scene scene*/) {};

	virtual void Update() {}
	virtual void Render() {}

	virtual void DebugRender(/*const Camera& camera*/) {}

private:
	bool m_active = true;
	bool m_visible = true;
	bool m_collidable = false;
	glm::vec2 m_position = glm::vec2(0.f);

	// std::weak_ptr<Scene> m_scene = nullptr;
	// std::shared_ptr<ComponentList> m_components = nullptr;

	int m_tag = 0;

	// Collider* collider;

	int m_depth = 0;
	double m_actualDepth = 0.f;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_ENTITY_H_
