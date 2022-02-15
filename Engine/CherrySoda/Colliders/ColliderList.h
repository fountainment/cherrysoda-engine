#ifndef _CHERRYSODA_COLLIDERS_COLLIDERLIST_H_
#define _CHERRYSODA_COLLIDERS_COLLIDERLIST_H_

#include <CherrySoda/Colliders/Collider.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class Component;
class Entity;

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

	void Add(const IterableColliders& toAdd);
	void Remove(const IterableColliders& toRemove);

	float Width() const { return Right() - Left(); }
	float Height() const { return Top() - Bottom(); }

	bool Collide(const Circle* circle) const override;
	bool Collide(const ColliderList* list) const override;
	bool Collide(const Hitbox* hitbox) const override;
	bool Collide(const Math::Vec2& point) const override;
	bool Collide(const Math::Rectangle& rect) const override;
	bool Collide(const Math::Vec2& from, const Math::Vec2& to) const override;
	void Render(const Camera* camera, const Color& color) const override;

	float Left() const override;
	float Right() const override;
	float Bottom() const override;
	float Top() const override;

private:
	void Added(Entity* entity) override;
	void Added(Component* component) override;
	void Removed() override;

	IterableColliders m_colliders;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COLLIDERS_COLLIDERLIST_H_
