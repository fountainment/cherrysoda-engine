#ifndef _CHERRYSODA_UTIL_MATH_H_
#define _CHERRYSODA_UTIL_MATH_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define CHERRYSODA_NONE_OP

#define CHERRYSODA_GETTER_SETTER(NAME,VALUE) \
	CHERRYSODA_GETTER_SETTER_EX(NAME,VALUE,CHERRYSODA_NONE_OP,CHERRYSODA_NONE_OP)

#define CHERRYSODA_GETTER_SETTER_EX(NAME,VALUE,EXTRA_GET_OP,EXTRA_SET_OP) \
	inline const decltype(VALUE) NAME() const { EXTRA_GET_OP; return VALUE; } \
	inline void NAME(const decltype(VALUE)& v) { EXTRA_SET_OP; VALUE = v; } \

#define CHERRYSODA_MATH_VEC3_GETTER_SETTER(NAME,VALUE) \
	CHERRYSODA_MATH_VEC3_GETTER_SETTER_EX(NAME,VALUE,CHERRYSODA_NONE_OP,CHERRYSODA_NONE_OP)

#define CHERRYSODA_MATH_VEC3_GETTER_SETTER_EX(NAME,VALUE,EXTRA_GET_OP,EXTRA_SET_OP) \
	inline const Math::Vec3 NAME() const { EXTRA_GET_OP; return VALUE; } \
	inline void NAME(const Math::Vec3& v3) { EXTRA_SET_OP; VALUE = v3; } \
	inline void NAME(const Math::Vec2& v2) { NAME(Math::Vec3(v2, NAME()[2])); }

#define Vec2_Zero     Math::Vec2(0.f)
#define Vec2_One      Math::Vec2(1.f)
#define Vec2_XUp      Math::Vec2(1.f,0.f)
#define Vec2_YUp      Math::Vec2(0.f,1.f)

#define Vec3_Zero     Math::Vec3(0.f)
#define Vec3_One      Math::Vec3(1.f)
#define Vec3_XUp      Math::Vec3(1.f,0.f,0.f)
#define Vec3_YUp      Math::Vec3(0.f,1.f,0.f)
#define Vec3_ZUp      Math::Vec3(0.f,0.f,1.f)

#define Mat4_Identity Math::Mat4(1.f)

#define IVec3_Zero    Math::IVec3(0)
#define IVec3_One     Math::IVec3(1)
#define IVec3_XUp     Math::IVec3(1,0,0)
#define IVec3_YUp     Math::IVec3(0,1,0)
#define IVec3_ZUp     Math::IVec3(0,0,1)

namespace cherrysoda {

class Math
{
public:
	using Vec2 = glm::vec2;
	using Vec3 = glm::vec3;
	using Vec4 = glm::vec4;
	using Mat4 = glm::mat4;

	using IVec3 = glm::i32vec3;

	static constexpr auto LookAt = glm::lookAt<glm::f32, glm::qualifier::defaultp>;
	static constexpr auto Perspective = glm::perspective<glm::f32>;

	static constexpr auto InverseMat4 = glm::inverse<4, 4, glm::f32, glm::qualifier::defaultp>;
	static constexpr auto TranslateMat4 = glm::translate<float,glm::qualifier::defaultp>;
	static constexpr auto RotateMat4 = glm::rotate<float,glm::qualifier::defaultp>;
	static constexpr auto ScaleMat4 = glm::scale<float,glm::qualifier::defaultp>;

	static const Vec3 RotateVector(const Vec3& v3, float angle, const Vec3& axis)
	{
		return Vec3(Vec4(v3, 1.f) * RotateMat4(Mat4_Identity, angle, axis));
	}
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_MATH_H_