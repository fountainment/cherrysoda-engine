#ifndef _CHERRYSODA_COLLIDERS_COLLIDER_H_
#define _CHERRYSODA_COLLIDERS_COLLIDER_H_

#include <CherrySoda/Entity.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/Pool.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>


#define CHERRYSODA_DECLARE_COLLIDER(COLL,BASE) \
typedef BASE base; \
virtual cherrysoda::type::Int32 TypeID() const override \
{ return COLL::ColliderTypeID(); } \
virtual const char* TypeCStr() const override \
{ return #COLL; } \
static CHERRYSODA_STRINGID_CONSTEXPR cherrysoda::type::Int32 ColliderTypeID() \
{ return cherrysoda::StringID(#COLL).GetID(); }


namespace cherrysoda {

class Circle;
class Color;
class Camera;
class CollidableComponent;
class ColliderList;
class Component;
class Grid;
class Hitbox;

class Collider
{
public:
	Collider() = default;
	virtual ~Collider() = default;

	CHERRYSODA_GETTER_SETTER_OF_VEC3(Position, m_position);

	inline bool Collide(const Entity* entity) const { return Collide(entity->GetCollider()); }
	bool Collide(const CollidableComponent* component) const;
	bool Collide(const Collider* collider) const;

	virtual bool Collide(const Circle* circle) const = 0;
	virtual bool Collide(const Hitbox* hitbox) const = 0;
	virtual bool Collide(const Grid* grid) const = 0;
	virtual bool Collide(const ColliderList* list) const = 0;
	virtual bool Collide(const Math::Vec2& point) const = 0;
	virtual bool Collide(const Math::Rectangle& rect) const = 0;
	virtual bool Collide(const Math::Vec2& from, const Math::Vec2& to) const = 0;

	virtual float Left() const = 0;
	virtual float Right() const = 0;
	virtual float Bottom() const = 0;
	virtual float Top() const = 0;

	virtual float Width() const = 0;
	virtual float Height() const = 0;

	inline Math::Vec2 AbsolutePosition2D() const { return m_entity != nullptr ? m_entity->Position2D() + Position2D() : Position2D(); }
	inline Math::Vec3 AbsolutePosition() const { return m_entity != nullptr ? m_entity->Position() + Position() : Position(); }
	inline float AbsoluteTop() const { return m_entity != nullptr ? Top() + m_entity->PositionY() : Top(); }
	inline float AbsoluteBottom() const { return m_entity != nullptr ? Bottom() + m_entity->PositionY() : Bottom(); }
	inline float AbsoluteLeft() const { return m_entity != nullptr ? Left() + m_entity->PositionX() : Left(); }
	inline float AbsoluteRight() const { return m_entity != nullptr ? Right() + m_entity->PositionX() : Right(); }

	inline Math::Rectangle Bounds() const
	{
		return Math::Rectangle{ Math::Vec2(AbsoluteLeft(), AbsoluteBottom()), Math::Vec2(Width(), Height()) };
	}

	void Render(const Camera* camera) const;
	virtual void Render(const Camera* camera, const Color& color) const = 0;

	inline Entity* GetEntity() const { return m_entity; }
	inline Component* GetComponent() const { return m_component; }

	virtual type::Int32 TypeID() const = 0;
	virtual const char* TypeCStr() const = 0;

	void AutoDeleteWhenRemoved() { if (m_onRemoved == nullptr) m_onRemoved = Collider::DeleteCollider; }
	void CancleAutoDelete() { m_onRemoved = nullptr; }

private:
	CHERRYSODA_FRIEND_CLASS_POOL;

	friend class CollidableComponent;
	friend class ColliderList;
	friend class Entity;

	void AutoDeleteWhenRemoved(PoolInterface* pool);

	static void DeleteCollider(Collider* collider, Entity* entity);

	virtual void Added(Entity* entity);
	virtual void Added(Component* component);
	virtual void Removed();

	Math::Vec3 m_position = Vec3_Zero;

	Component* m_component = nullptr;
	Entity* m_entity = nullptr;
	STL::Action<Collider*, Entity*> m_onRemoved = nullptr;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COLLIDERS_COLLIDER_H_
