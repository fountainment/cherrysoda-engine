#ifndef _CHERRYSODA_COLLIDERS_COLLIDE_H_
#define _CHERRYSODA_COLLIDERS_COLLIDE_H_

#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class Entity;

class Collide
{
public:
	static bool Check(const Entity* a, const Entity* b);

	static bool Check(const Entity* a, const STL::List<Entity*> b);

	static Entity* First(const Entity* a, const STL::List<Entity*> b);

	static const STL::List<Entity*> All(const Entity* a, const STL::List<Entity*> b);
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COLLIDERS_COLLIDE_H_
