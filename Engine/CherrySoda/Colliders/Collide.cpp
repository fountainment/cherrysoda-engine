#include <CherrySoda/Colliders/Collide.h>

#include <CherrySoda/Colliders/Collider.h>
#include <CherrySoda/Components/CollidableComponent.h>
#include <CherrySoda/Entity.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/Profile.h>
#include <CherrySoda/Util/STL.h>

using cherrysoda::Collide;

using cherrysoda::CollidableComponent;
using cherrysoda::Entity;
using cherrysoda::Math;
using cherrysoda::PointSectors;
using cherrysoda::STL;

bool Collide::Check(const Entity* a, const Entity* b)
{
	if (a->GetCollider() == nullptr || b->GetCollider() == nullptr) {
		return false;
	}
	return a != b && b->Collidable() && a->GetCollider()->Collide(b);
}

bool Collide::Check(Entity* a, const Entity* b, const Math::Vec2& at)
{
	Math::Vec2 old = a->Position2D();
	a->Position2D(at);
	bool ret = Check(a, b);
	a->Position2D(old);
	return ret;
}

bool Collide::Check(const Entity* a, const CollidableComponent* b)
{
	if (a->GetCollider() == nullptr && b->GetCollider() == nullptr) {
		return false;
	}
	else {
		return b->Collidable() && b->GetEntity()->Collidable() && a->GetCollider()->Collide(b);
	}
}

bool Collide::Check(Entity* a, const CollidableComponent* b, const Math::Vec2& at)
{
	Math::Vec2 old = a->Position2D();
	a->Position2D(at);
	bool ret = Check(a, b);
	a->Position2D(old);
	return ret;
}

bool Collide::Check(const Entity* a, const STL::List<Entity*>& b)
{
	CHERRYSODA_PROFILE_FUNCTION();
	for (auto e : b) {
		if (Check(a, e)) {
			return true;
		}
	}
	return false;
}

bool Collide::Check(Entity* a, const STL::List<Entity*>& b, const Math::Vec2& at)
{
	CHERRYSODA_PROFILE_FUNCTION();
	Math::Vec2 position = a->Position2D();
	a->Position2D(at);
	bool result = Check(a, b);
	a->Position2D(position);
	return result;
}

bool Collide::CheckPoint(const Entity* a, const Math::Vec2& point)
{
	if (a->GetCollider() == nullptr) {
		return false;
	}
	return a->GetCollider()->Collide(point);
}

bool Collide::CheckLine(const Entity* a, const Math::Vec2& from, const Math::Vec2& to)
{
	if (a->GetCollider() == nullptr) {
		return false;
	}
	return a->GetCollider()->Collide(from, to);
}

int Collide::Count(const Entity* a, const STL::List<Entity*>& b)
{
	CHERRYSODA_PROFILE_FUNCTION();
	int count = 0;
	for (auto e : b) {
		if (Check(a, e)) {
			++count;
		}
	}
	return count;
}

Entity* Collide::First(const Entity* a, const STL::List<Entity*>& b)
{
	CHERRYSODA_PROFILE_FUNCTION();
	for (auto e : b) {
		if (Check(a, e)) {
			return e;
		}
	}
	return nullptr;
}

const STL::List<Entity*> Collide::All(const Entity* a, const STL::List<Entity*>& b)
{
	CHERRYSODA_PROFILE_FUNCTION();
	STL::List<Entity*> ret;
	for (auto e : b) {
		if (Check(a, e)) {
			STL::Add(ret, e);
		}
	}
	return ret;
}

bool Collide::LineCheck(const Math::Vec2& a1, const Math::Vec2& a2, const Math::Vec2& b1, const Math::Vec2& b2)
{
	Math::Vec2 b = a2 - a1;
	Math::Vec2 d = b2 - b1;
	float bDotDPerp = b.x * d.y - b.y * d.x;

	// if b dot d == 0, it means the lines are parallel so have infinite intersection points
	if (bDotDPerp == 0.f)
		return false;

	Math::Vec2 c = b1 - a1;
	float t = (c.x * d.y - c.y * d.x) / bDotDPerp;
	if (t < 0.f || t > 1.f)
		return false;

	float u = (c.x * b.y - c.y * b.x) / bDotDPerp;
	if (u < 0.f || u > 1.f)
		return false;

	return true;
}

bool Collide::LineCheck(const Math::Vec2& a1, const Math::Vec2& a2, const Math::Vec2& b1, const Math::Vec2& b2, Math::Vec2& intersection)
{
	Math::Vec2 b = a2 - a1;
	Math::Vec2 d = b2 - b1;
	float bDotDPerp = b.x * d.y - b.y * d.x;

	// if b dot d == 0, it means the lines are parallel so have infinite intersection points
	if (bDotDPerp == 0.f)
		return false;

	Math::Vec2 c = b1 - a1;
	float t = (c.x * d.y - c.y * d.x) / bDotDPerp;
	if (t < 0.f || t > 1.f)
		return false;

	float u = (c.x * b.y - c.y * b.x) / bDotDPerp;
	if (u < 0.f || u > 1.f)
		return false;

	intersection = a1 + t * b;

	return true;
}

bool Collide::RectToCircle(float rX, float rY, float rW, float rH, const Math::Vec2& cPosition, float cRadius)
{
	// Check if the rectangle contains the circle's center point
	if (RectToPoint(rX, rY, rW, rH, cPosition))
		return true;

	// Check the circle against the relevant edges
	Math::Vec2 edgeFrom;
	Math::Vec2 edgeTo;
	PointSectors sector = GetSector(rX, rY, rW, rH, cPosition);

	if (static_cast<int>(sector & PointSectors::Bottom) != 0) {
		edgeFrom = Math::Vec2(rX, rY);
		edgeTo = Math::Vec2(rX + rW, rY);
		if (CircleToLine(cPosition, cRadius, edgeFrom, edgeTo))
			return true;
	}

	if (static_cast<int>(sector & PointSectors::Top) != 0) {
		edgeFrom = Math::Vec2(rX, rY + rH);
		edgeTo = Math::Vec2(rX + rW, rY + rH);
		if (CircleToLine(cPosition, cRadius, edgeFrom, edgeTo))
			return true;
	}

	if (static_cast<int>(sector & PointSectors::Left) != 0) {
		edgeFrom = Math::Vec2(rX, rY);
		edgeTo = Math::Vec2(rX, rY + rH);
		if (CircleToLine(cPosition, cRadius, edgeFrom, edgeTo))
			return true;
	}

	if (static_cast<int>(sector & PointSectors::Right) != 0) {
		edgeFrom = Math::Vec2(rX + rW, rY);
		edgeTo = Math::Vec2(rX + rW, rY + rH);
		if (CircleToLine(cPosition, cRadius, edgeFrom, edgeTo))
			return true;
	}

	return false;
}

bool Collide::RectToLine(float rX, float rY, float rW, float rH, const Math::Vec2& lineFrom, const Math::Vec2& lineTo)
{
	PointSectors fromSector = GetSector(rX, rY, rW, rH, lineFrom);
	PointSectors toSector = GetSector(rX, rY, rW, rH, lineTo);

	if (fromSector == PointSectors::Center || toSector == PointSectors::Center)
		return true;
	else if (static_cast<int>(fromSector & toSector) != 0)
		return false;
	else {
		PointSectors both = fromSector | toSector;

		// Do line checks against the edges
		Math::Vec2 edgeFrom;
		Math::Vec2 edgeTo;

		if (static_cast<int>(both & PointSectors::Bottom) != 0) {
			edgeFrom = Math::Vec2(rX, rY);
			edgeTo = Math::Vec2(rX + rW, rY);
			if (LineCheck(edgeFrom, edgeTo, lineFrom, lineTo))
				return true;
		}

		if (static_cast<int>(both & PointSectors::Top) != 0) {
			edgeFrom = Math::Vec2(rX, rY + rH);
			edgeTo = Math::Vec2(rX + rW, rY + rH);
			if (LineCheck(edgeFrom, edgeTo, lineFrom, lineTo))
				return true;
		}

		if (static_cast<int>(both & PointSectors::Left) != 0) {
			edgeFrom = Math::Vec2(rX, rY);
			edgeTo = Math::Vec2(rX, rY + rH);
			if (LineCheck(edgeFrom, edgeTo, lineFrom, lineTo))
				return true;
		}

		if (static_cast<int>(both & PointSectors::Right) != 0) {
			edgeFrom = Math::Vec2(rX + rW, rY);
			edgeTo = Math::Vec2(rX + rW, rY + rH);
			if (LineCheck(edgeFrom, edgeTo, lineFrom, lineTo))
				return true;
		}
	}

	return false;
}

PointSectors Collide::GetSector(float rX, float rY, float rW, float rH, const Math::Vec2& point)
{
	PointSectors sector = PointSectors::Center;

	if (point.x < rX)
		sector |= PointSectors::Left;
	else if (point.x >= rX + rW)
		sector |= PointSectors::Right;

	if (point.y < rY)
		sector |= PointSectors::Bottom;
	else if (point.y >= rY + rH)
		sector |= PointSectors::Top;

	return sector;
}
