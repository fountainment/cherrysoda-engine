#include <CherrySoda/Colliders/ColliderList.h>

#include <CherrySoda/Colliders/Circle.h>
#include <CherrySoda/Colliders/Grid.h>
#include <CherrySoda/Colliders/Hitbox.h>
#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Entity.h>
#include <CherrySoda/Util/Camera.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>
#include <algorithm>

namespace cherrysoda {

void ColliderList::Add(const ColliderList::IterableColliders& toAdd)
{
#ifdef CHERRYSODA_ENABLE_DEBUG
	for (auto* c : toAdd) {
		if (STL::Contains(m_colliders, c))
			CHERRYSODA_ASSERT(false, "Adding a Collider to a ColliderList that already contains it!\n");
		else if (c == nullptr)
			CHERRYSODA_ASSERT(false, "Cannot add a null Collider to a ColliderList.\n");
	}
#endif // CHERRYSODA_ENABLE_DEBUG

	for (auto* c : toAdd) {
		STL::Add(m_colliders, c);
		c->Added(GetEntity());
	}
}

void ColliderList::Remove(const ColliderList::IterableColliders& toRemove)
{
#ifdef CHERRYSODA_ENABLE_DEBUG
	for (auto* c : toRemove) {
		if (!STL::Contains(m_colliders, c))
			CHERRYSODA_ASSERT(false, "Removing a Collider from a ColliderList that does not contain it!\n");
		else if (c == nullptr)
			CHERRYSODA_ASSERT(false, "Cannot remove a null Collider from a ColliderList.\n");
	}
#endif // CHERRYSODA_ENABLE_DEBUG

	for (auto* c : toRemove) {
		STL::Remove(m_colliders, c);
	}
}

bool ColliderList::Collide(const Circle* circle) const
{
	return std::ranges::any_of(m_colliders, [&](const auto* c) { return c->Collide(circle); });
}

bool ColliderList::Collide(const ColliderList* list) const
{
	return std::ranges::any_of(m_colliders, [&](const auto* c) { return c->Collide(list); });
}

bool ColliderList::Collide(const Hitbox* hitbox) const
{
	return std::ranges::any_of(m_colliders, [&](const auto* c) { return c->Collide(hitbox); });
}

bool ColliderList::Collide(const Grid* grid) const
{
	return std::ranges::any_of(m_colliders, [&](const auto* c) { return c->Collide(grid); });
}

bool ColliderList::Collide(const Math::Vec2& point) const
{
	return std::ranges::any_of(m_colliders, [&](const auto* c) { return c->Collide(point); });
}

bool ColliderList::Collide(const Math::Rectangle& rect) const
{
	return std::ranges::any_of(m_colliders, [&](const auto* c) { return c->Collide(rect); });
}

bool ColliderList::Collide(const Math::Vec2& from, const Math::Vec2& to) const
{
	return std::ranges::any_of(m_colliders, [&](const auto* c) { return c->Collide(from, to); });
}

void ColliderList::Render(const Camera* camera, const Color& color) const
{
	for (auto* c : m_colliders) {
		c->Render(camera, color);
	}
}

float ColliderList::Left() const
{
	if (STL::IsEmpty(m_colliders)) return Math::NaNf();
	float left = Math::FloatMax;
	for (auto* c : m_colliders) {
		left = Math_Min(left, c->Left());
	}
	return left;
}

float ColliderList::Right() const
{
	if (STL::IsEmpty(m_colliders)) return Math::NaNf();
	float right = -Math::FloatMax;
	for (auto* c : m_colliders) {
		right = Math_Max(right, c->Right());
	}
	return right;
}

float ColliderList::Bottom() const
{
	if (STL::IsEmpty(m_colliders)) return Math::NaNf();
	float bottom = Math::FloatMax;
	for (auto* c : m_colliders) {
		bottom = Math_Min(bottom, c->Bottom());
	}
	return bottom;
}

float ColliderList::Top() const
{
	if (STL::IsEmpty(m_colliders)) return Math::NaNf();
	float top = -Math::FloatMax;
	for (auto* c : m_colliders) {
		top = Math_Max(top, c->Top());
	}
	return top;
}

void ColliderList::Added(Entity* entity)
{
	base::Added(entity);
	for (auto* c : m_colliders) {
		c->Added(entity);
	}
}

void ColliderList::Added(Component* component)
{
	base::Added(component);
	for (auto* c : m_colliders) {
		c->Added(component);
	}
}

void ColliderList::Removed()
{
	base::Removed();
	for (auto* c : m_colliders) {
		c->Removed();
	}
}

} // namespace cherrysoda
