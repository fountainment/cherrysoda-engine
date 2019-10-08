#ifndef _CHERRYSODA_ENTITY_H_
#define _CHERRYSODA_ENTITY_H_

#include <glm/vec2.hpp>

namespace cherrysoda {

class Entity
{
public:
	Entity() {}

private:
	bool m_active;
	bool m_visible;
	bool m_collidable;

	// Scene* m_scene = nullptr;
	// ComponentList m_components;

	int m_tag;

	// Collider* collider;

	int m_depth;
	double m_actualDepth;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_ENTITY_H_