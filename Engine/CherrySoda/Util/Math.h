#ifndef _CHERRYSODA_UTIL_MATH_H_
#define _CHERRYSODA_UTIL_MATH_H_

#define GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace cherrysoda {

class Math
{
public:
	using Vec2 = glm::vec2;
	using Vec3 = glm::vec3;
	using Mat4 = glm::mat4;

	static constexpr auto LookAt = glm::lookAt<glm::f32, glm::qualifier::defaultp>;
	static constexpr auto Perspective = glm::perspective<glm::f32>;
	static constexpr auto InverseMatrix = glm::inverse<4, 4, glm::f32, glm::qualifier::defaultp>;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_MATH_H_