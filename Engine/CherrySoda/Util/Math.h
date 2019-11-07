#ifndef _CHERRYSODA_UTIL_MATH_H_
#define _CHERRYSODA_UTIL_MATH_H_

#define GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>

namespace cherrysoda {

class Math
{
public:
	using Vec2 = glm::vec2;
	using Vec3 = glm::vec3;
	using Mat4 = glm::mat4;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_MATH_H_