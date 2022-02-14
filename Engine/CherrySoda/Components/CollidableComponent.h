#ifndef _CHERRYSODA_COMPONENTS_COLLIDABLECOMPONENT_H_
#define _CHERRYSODA_COMPONENTS_COLLIDABLECOMPONENT_H_

#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Util/Math.h>

namespace cherrysoda {

class Camera;
class Collider;
class Entity;

class CollidableComponent : public Component
{
public:
	CHERRYSODA_DECLARE_COMPONENT(CollidableComponent, Component);

	CollidableComponent(bool active, bool visible, bool colllidable)
		: base(active, visible)
		, m_collidable(colllidable)
	{}

	CHERRYSODA_GETTER_SETTER_OF_BOOL(Collidable, m_collidable);

	void Added(Entity* entity) override;
	void Removed(Entity* entity) override;

	void DebugRender(Camera* camera) override;

	void SetCollider(Collider* collider);
	Collider* GetCollider() const;

private:
	bool m_collidable = true;
	Collider* m_collider = nullptr;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_COLLIDABLECOMPONENT_H_
