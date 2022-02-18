#ifndef _CHERRYSODA_COLLIDERS_COLLIDERLIST_H_
#define _CHERRYSODA_COLLIDERS_COLLIDERLIST_H_

#include <CherrySoda/Colliders/Collider.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/Log.h>

namespace cherrysoda {

class Camera;
class Circle;
class Color;
class Component;
class Entity;
class Grid;
class Hitbox;

class ColliderList : public Collider
{
public:
	CHERRYSODA_DECLARE_COLLIDER(ColliderList, Collider);
	CHERRYSODA_ITERABLE(m_colliders);

	typedef STL::List<Collider*> IterableColliders;

	ColliderList(const IterableColliders& colliders)
	{
#ifdef CHERRYSODA_ENABLE_DEBUG
		for (auto c : colliders) {
			if (c == nullptr) {
				CHERRYSODA_ASSERT(false, "Cannot add a null Collider to a ColliderList.\n");
			}
		}
#endif // CHERRYSODA_ENABLE_DEBUG
		m_colliders = colliders;
	}

	~ColliderList()
	{
		if (m_ownColliders) {
			for (auto c : m_colliders) {
				delete c;
			}
		}
		STL::Clear(m_colliders);
	}

	void Add(const IterableColliders& toAdd);
	void Remove(const IterableColliders& toRemove);

	bool Collide(const Circle* circle) const override;
	bool Collide(const ColliderList* list) const override;
	bool Collide(const Hitbox* hitbox) const override;
	bool Collide(const Grid* grid) const override;
	bool Collide(const Math::Vec2& point) const override;
	bool Collide(const Math::Rectangle& rect) const override;
	bool Collide(const Math::Vec2& from, const Math::Vec2& to) const override;
	void Render(const Camera* camera, const Color& color) const override;

	float Left() const override;
	float Right() const override;
	float Bottom() const override;
	float Top() const override;

	float Width() const override { return Right() - Left(); }
	float Height() const override { return Top() - Bottom(); }

	inline void SetOwnColliders(bool own) { m_ownColliders = own; }

private:
	void Added(Entity* entity) override;
	void Added(Component* component) override;
	void Removed() override;

	IterableColliders m_colliders;
	bool m_ownColliders = false;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COLLIDERS_COLLIDERLIST_H_
