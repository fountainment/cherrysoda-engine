#ifndef _CHERRYSODA_COLLIDERS_COLLIDER_H_
#define _CHERRYSODA_COLLIDERS_COLLIDER_H_

#include <CherrySoda/Entity.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/String.h>

#ifndef NDEBUG
#define CHERRYSODA_COLLIDER_TYPE_STR(NAME) \
virtual cherrysoda::String TypeStr() const override \
{ return cherrysoda::StringID(""#NAME).GetStr(); }
#else
#define CHERRYSODA_COLLIDER_TYPE_STR(NAME)
#endif // NDEBUG

#define CHERRYSODA_DECLARE_COLLIDER(COLL,BASE) \
typedef BASE base; \
virtual cherrysoda::type::Int32 TypeID() const override \
{ return COLL::ColliderTypeID(); } \
CHERRYSODA_COLLIDER_TYPE_STR(COLL) \
static inline cherrysoda::type::Int32 ColliderTypeID() \
{ return cherrysoda::StringID(""#COLL).GetID(); }

namespace cherrysoda {

class Circle;
class Color;
class Camera;
class Component;
class Entity;

class Collider
{
public:
	Collider() = default;
	virtual ~Collider() = default;

	inline bool Collide(const Entity* entity) const
	{
		return Collide(entity->GetCollider());
	}

	bool Collide(const Collider* collider) const;
	virtual bool Collide(const Circle* circle) const = 0;
	virtual bool Collide(const Math::Vec2& point) const = 0;

	Math::Vec2 AbsolutePosition2D() const;
	Math::Vec3 AbsolutePosition() const;

	void Render(const Camera* camera) const;
	virtual void Render(const Camera* camera, const Color& color) const = 0;

	inline Entity* GetEntity() const { return m_entity; }
	inline Component* GetComponent() const { return m_component; }

	virtual type::Int32 TypeID() const = 0;

#ifndef NDEBUG
	virtual String TypeStr() const = 0;
#endif

	CHERRYSODA_GETTER_SETTER_OF_VEC3(Position, m_position);

private:
	friend class Entity;

	void Removed();
	virtual void Added(Entity* entity);

	Math::Vec3 m_position = Vec3_Zero;

	Component* m_component = nullptr;
	Entity* m_entity = nullptr;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COLLIDERS_COLLIDER_H_
