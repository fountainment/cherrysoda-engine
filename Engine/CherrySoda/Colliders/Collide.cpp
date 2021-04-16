#include <CherrySoda/Colliders/Collide.h>

#include <CherrySoda/Colliders/Collider.h>
#include <CherrySoda/Entity.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/Profile.h>
#include <CherrySoda/Util/STL.h>

using cherrysoda::Collide;

using cherrysoda::Entity;
using cherrysoda::Math;
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
	Math::Vec2 position = a->Position2D();
	a->Position2D(at);
	bool result = Check(a, b);
	a->Position2D(position);
	return result;
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
