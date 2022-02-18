#include <CherrySoda/Colliders/ColliderList.h>

#include <CherrySoda/Colliders/Circle.h>
#include <CherrySoda/Colliders/Grid.h>
#include <CherrySoda/Colliders/Hitbox.h>
#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Util/Camera.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Entity.h>

using cherrysoda::ColliderList;

using cherrysoda::Camera;
using cherrysoda::Circle;
using cherrysoda::Color;
using cherrysoda::Component;
using cherrysoda::Entity;
using cherrysoda::Grid;
using cherrysoda::Hitbox;
using cherrysoda::Math;
using cherrysoda::STL;

void ColliderList::Add(const ColliderList::IterableColliders& toAdd)
{
#ifdef CHERRYSODA_ENABLE_DEBUG
	for (auto c : toAdd) {
		if (STL::Contains(m_colliders, c))
			CHERRYSODA_ASSERT(false, "Adding a Collider to a ColliderList that already contains it!\n");
		else if (c == nullptr)
			CHERRYSODA_ASSERT(false, "Cannot add a null Collider to a ColliderList.\n");
	}
#endif // CHERRYSODA_ENABLE_DEBUG

	for (auto c : toAdd) {
		STL::Add(m_colliders, c);
		c->Added(GetEntity());
	}
}

void ColliderList::Remove(const ColliderList::IterableColliders& toRemove)
{
#ifdef CHERRYSODA_ENABLE_DEBUG
	for (auto c : toRemove) {
		if (!STL::Contains(m_colliders, c))
			CHERRYSODA_ASSERT(false, "Removing a Collider from a ColliderList that does not contain it!\n");
		else if (c == nullptr)
			CHERRYSODA_ASSERT(false, "Cannot remove a null Collider from a ColliderList.\n");
	}
#endif // CHERRYSODA_ENABLE_DEBUG

	for (auto c : toRemove) {
		STL::Remove(m_colliders, c);
	}
}

bool ColliderList::Collide(const Circle* circle) const
{
	for (auto c : m_colliders)
		if (c->Collide(circle))
			return true;
	return false;
}

bool ColliderList::Collide(const ColliderList* list) const
{
	for (auto c : m_colliders)
		if (c->Collide(list))
			return true;
	return false;
}

bool ColliderList::Collide(const Hitbox* hitbox) const
{
	for (auto c : m_colliders)
		if (c->Collide(hitbox))
			return true;
	return false;
}

bool ColliderList::Collide(const Grid* grid) const
{
	for (auto c : m_colliders)
		if (c->Collide(grid))
			return true;
	return false;
}

bool ColliderList::Collide(const Math::Vec2& point) const
{
	for (auto c : m_colliders)
		if (c->Collide(point))
			return true;
	return false;
}

bool ColliderList::Collide(const Math::Rectangle& rect) const
{
	for (auto c : m_colliders)
		if (c->Collide(rect))
			return true;
	return false;
}

bool ColliderList::Collide(const Math::Vec2& from, const Math::Vec2& to) const
{
	for (auto c : m_colliders)
		if (c->Collide(from, to))
			return true;
	return false;
}

void ColliderList::Render(const Camera* camera, const Color& color) const
{
	for (auto c : m_colliders) {
		c->Render(camera, color);
	}
}

float ColliderList::Left() const
{
	if (STL::IsEmpty(m_colliders)) return Math::NaNf();
	float left = Math::FloatMax;
	for (auto c : m_colliders) {
		left = Math_Min(left, c->Left());
	}
	return left;
}

float ColliderList::Right() const
{
	if (STL::IsEmpty(m_colliders)) return Math::NaNf();
	float right = Math::FloatMin;
	for (auto c : m_colliders) {
		right = Math_Max(right, c->Right());
	}
	return right;
}

float ColliderList::Bottom() const
{
	if (STL::IsEmpty(m_colliders)) return Math::NaNf();
	float bottom = Math::FloatMax;
	for (auto c : m_colliders) {
		bottom = Math_Min(bottom, c->Bottom());
	}
	return bottom;
}

float ColliderList::Top() const
{
	if (STL::IsEmpty(m_colliders)) return Math::NaNf();
	float top = Math::FloatMin;
	for (auto c : m_colliders) {
		top = Math_Max(top, c->Top());
	}
	return top;
}

void ColliderList::Added(Entity* entity)
{
	base::Added(entity);
	for (auto c : m_colliders) {
		c->Added(entity);
	}
}

void ColliderList::Added(Component* component)
{
	base::Added(component);
	for (auto c : m_colliders) {
		c->Added(component);
	}
}

void ColliderList::Removed()
{
	base::Removed();
	for (auto c : m_colliders) {
		c->Removed();
	}
}
