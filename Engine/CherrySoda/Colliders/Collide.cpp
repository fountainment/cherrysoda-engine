#include <CherrySoda/Colliders/Collide.h>

#include <CherrySoda/Colliders/Collider.h>
#include <CherrySoda/Entity.h>
#include <CherrySoda/Util/STL.h>

using cherrysoda::Collide;

using cherrysoda::Entity;
using cherrysoda::STL;

bool Collide::Check(const Entity* a, const Entity* b)
{
	if (a->GetCollider() == nullptr || b->GetCollider() == nullptr) {
		return false;
	}
	else {
		return a != b && b->Collidable() && a->GetCollider()->Collide(b);
	}
}

bool Collide::Check(const Entity* a, const STL::List<Entity*> b)
{
	for (auto e : b) {
		if (Check(a, e)) {
			return true;
		}
	}	
	return false;
}

Entity* Collide::First(const Entity* a, const STL::List<Entity*> b)
{
	for (auto e : b) {
		if (Check(a, e)) {
			return e;
		}
	}
	return nullptr;
}

const STL::List<Entity*> Collide::All(const Entity* a, const STL::List<Entity*> b)
{
	STL::List<Entity*> ret;
	for (auto e : b) {
		if (Check(a, e)) {
			STL::Add(ret, e);
		}
	}
	return ret;
}
