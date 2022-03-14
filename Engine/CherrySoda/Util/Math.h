#ifndef _CHERRYSODA_UTIL_MATH_H_
#define _CHERRYSODA_UTIL_MATH_H_

#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_INTRINSICS
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/common.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#include <cmath>
#include <limits>
#include <type_traits>


#define CHERRYSODA_NONE_OP ((void)0)

#define CHERRYSODA_GETTER_SETTER(NAME,VALUE) \
	CHERRYSODA_GETTER_SETTER_EX(NAME,VALUE,CHERRYSODA_NONE_OP,CHERRYSODA_NONE_OP)

#define CHERRYSODA_GETTER_SETTER_OF_TYPE(TYPE,NAME,VALUE) \
	CHERRYSODA_GETTER_SETTER_EX_OF_TYPE(TYPE,NAME,VALUE,CHERRYSODA_NONE_OP,CHERRYSODA_NONE_OP)

#define CHERRYSODA_GETTER_SETTER_EX(NAME,VALUE,EXTRA_GET_OP,EXTRA_SET_OP) \
	inline const decltype(VALUE) NAME() const { EXTRA_GET_OP; return VALUE; } \
	inline void NAME(const decltype(VALUE)& v) { EXTRA_SET_OP; VALUE = v; }

#define CHERRYSODA_GETTER_SETTER_EX_OF_TYPE(TYPE,NAME,VALUE,EXTRA_GET_OP,EXTRA_SET_OP) \
	inline const TYPE NAME() const { EXTRA_GET_OP; return VALUE; } \
	inline void NAME(const TYPE& v) { EXTRA_SET_OP; VALUE = v; }

#define CHERRYSODA_GETTER_SETTER_OF_VEC3(NAME,VALUE) \
	CHERRYSODA_GETTER_SETTER_EX_OF_VEC3(NAME,VALUE,CHERRYSODA_NONE_OP,CHERRYSODA_NONE_OP)

#define CHERRYSODA_GETTER_SETTER_EX_OF_VEC3(NAME,VALUE,EXTRA_GET_OP,EXTRA_SET_OP) \
	inline const cherrysoda::Math::Vec3 NAME() const { EXTRA_GET_OP; return VALUE; } \
	inline cherrysoda::Math::Vec3& NAME##Ref() { return VALUE; } \
	inline const cherrysoda::Math::Vec2 NAME##2D() const { return cherrysoda::Math::Vec2(NAME()); } \
	inline void NAME(const cherrysoda::Math::Vec3& v) { EXTRA_SET_OP; VALUE = v; } \
	inline void NAME(const cherrysoda::Math::Vec2& v) { NAME(cherrysoda::Math::Vec3(v, NAME().z)); } \
	inline void NAME##2D(const cherrysoda::Math::Vec2& v) { NAME(cherrysoda::Math::Vec3(v, NAME().z)); } \
	inline void Move##NAME(const cherrysoda::Math::Vec3& v) { NAME(VALUE + v); } \
	inline void Move##NAME(const cherrysoda::Math::Vec2& v) { NAME(VALUE + cherrysoda::Math::Vec3(v, 0.f)); } \
	inline void Move##NAME##2D(const cherrysoda::Math::Vec2& v) { NAME(VALUE + cherrysoda::Math::Vec3(v, 0.f)); } \
	inline void Move##NAME##X(float x) { NAME##X(VALUE.x + x); } \
	inline void Move##NAME##Y(float y) { NAME##Y(VALUE.y + y); } \
	inline void Move##NAME##Z(float z) { NAME##Z(VALUE.z + z); } \
	inline float NAME##X() const { EXTRA_GET_OP; return VALUE.x; } \
	inline float NAME##Y() const { EXTRA_GET_OP; return VALUE.y; } \
	inline float NAME##Z() const { EXTRA_GET_OP; return VALUE.z; } \
	inline void NAME##X(float x) { NAME(cherrysoda::Math::Vec3(x, VALUE.y, VALUE.z)); } \
	inline void NAME##Y(float y) { NAME(cherrysoda::Math::Vec3(VALUE.x, y, VALUE.z)); } \
	inline void NAME##Z(float z) { NAME(cherrysoda::Math::Vec3(VALUE.x, VALUE.y, z)); }

#define CHERRYSODA_GETTER_SETTER_OF_VEC2(NAME,VALUE) \
	CHERRYSODA_GETTER_SETTER_EX_OF_VEC2(NAME,VALUE,CHERRYSODA_NONE_OP,CHERRYSODA_NONE_OP)

#define CHERRYSODA_GETTER_SETTER_EX_OF_VEC2(NAME,VALUE,EXTRA_GET_OP,EXTRA_SET_OP) \
	inline const cherrysoda::Math::Vec2 NAME() const { EXTRA_GET_OP; return VALUE; } \
	inline cherrysoda::Math::Vec2& NAME##Ref() { return VALUE; } \
	inline void NAME(const cherrysoda::Math::Vec2& v) { EXTRA_SET_OP; VALUE = v; } \
	inline void Move##NAME(const cherrysoda::Math::Vec2& v) { NAME(VALUE + v); } \
	inline void Move##NAME##X(float x) { NAME##X(VALUE.x + x); } \
	inline void Move##NAME##Y(float y) { NAME##Y(VALUE.y + y); } \
	inline float NAME##X() const { EXTRA_GET_OP; return VALUE.x; } \
	inline float NAME##Y() const { EXTRA_GET_OP; return VALUE.y; } \
	inline void NAME##X(float x) { NAME(cherrysoda::Math::Vec2(x, VALUE.y)); } \
	inline void NAME##Y(float y) { NAME(cherrysoda::Math::Vec2(VALUE.x, y)); }

#define CHERRYSODA_GETTER_SETTER_OF_BOOL(NAME,VALUE) \
	CHERRYSODA_GETTER_SETTER_EX_OF_BOOL(NAME,VALUE,CHERRYSODA_NONE_OP,CHERRYSODA_NONE_OP)

#define CHERRYSODA_GETTER_SETTER_EX_OF_BOOL(NAME,VALUE,EXTRA_GET_OP,EXTRA_SET_OP) \
	inline bool NAME() const { EXTRA_GET_OP; return VALUE; } \
	inline void NAME(bool v) { EXTRA_SET_OP; VALUE = v; } \
	inline void Toggle##NAME() { NAME(!VALUE); }

#define CHERRYSODA_DECLARE_ENUM_FLAG(ENUM_T) \
inline ENUM_T operator | (ENUM_T lhs, ENUM_T rhs) \
{ \
    using T = std::underlying_type_t<ENUM_T>; \
    return static_cast<ENUM_T>(static_cast<T>(lhs) | static_cast<T>(rhs)); \
} \
inline ENUM_T& operator |= (ENUM_T& lhs, ENUM_T rhs) \
{ \
    lhs = lhs | rhs; \
    return lhs; \
} \
inline ENUM_T operator & (ENUM_T lhs, ENUM_T rhs) \
{ \
    using T = std::underlying_type_t<ENUM_T>; \
    return static_cast<ENUM_T>(static_cast<T>(lhs) & static_cast<T>(rhs)); \
} \
inline ENUM_T& operator &= (ENUM_T& lhs, ENUM_T rhs) \
{ \
    lhs = lhs & rhs; \
    return lhs; \
} \
inline ENUM_T operator - (ENUM_T lhs, ENUM_T rhs) \
{ \
    using T = std::underlying_type_t<ENUM_T>; \
    return static_cast<ENUM_T>(static_cast<T>(lhs) & ~static_cast<T>(rhs)); \
}


#define Vec2_Zero       cherrysoda::Math::Vec2(0.f)
#define Vec2_One        cherrysoda::Math::Vec2(1.f)
#define Vec2_XUp        cherrysoda::Math::Vec2(1.f,0.f)
#define Vec2_YUp        cherrysoda::Math::Vec2(0.f,1.f)

#define Vec3_Zero       cherrysoda::Math::Vec3(0.f)
#define Vec3_One        cherrysoda::Math::Vec3(1.f)
#define Vec3_XUp        cherrysoda::Math::Vec3(1.f,0.f,0.f)
#define Vec3_YUp        cherrysoda::Math::Vec3(0.f,1.f,0.f)
#define Vec3_ZUp        cherrysoda::Math::Vec3(0.f,0.f,1.f)

#define Vec4_Zero       cherrysoda::Math::Vec4(0.f)
#define Vec4_One        cherrysoda::Math::Vec4(1.f)
#define Vec4_XUp        cherrysoda::Math::Vec4(1.f,0.f,0.f,0.f)
#define Vec4_YUp        cherrysoda::Math::Vec4(0.f,1.f,0.f,0.f)
#define Vec4_ZUp        cherrysoda::Math::Vec4(0.f,0.f,1.f,0.f)
#define Vec4_WUp        cherrysoda::Math::Vec4(0.f,0.f,0.f,1.f)

#define IVec2_Zero      cherrysoda::Math::IVec2(0)
#define IVec2_One       cherrysoda::Math::IVec2(1)
#define IVec2_XUp       cherrysoda::Math::IVec2(1,0)
#define IVec2_YUp       cherrysoda::Math::IVec2(0,1)

#define IVec3_Zero      cherrysoda::Math::IVec3(0)
#define IVec3_One       cherrysoda::Math::IVec3(1)
#define IVec3_XUp       cherrysoda::Math::IVec3(1,0,0)
#define IVec3_YUp       cherrysoda::Math::IVec3(0,1,0)
#define IVec3_ZUp       cherrysoda::Math::IVec3(0,0,1)

#define Math_Atan2      glm::atan
#define Math_Abs        glm::abs
#define Math_Ceiling    glm::ceil
#define Math_Cross      glm::cross
#define Math_Cos        glm::cos
#define Math_Clamp      glm::clamp
#define Math_Dot        glm::dot
#define Math_Degrees    glm::degrees
#define Math_Floor      glm::floor
#define Math_Length     glm::length
#define Math_LengthSq   glm::length2
#define Math_Max        glm::max
#define Math_Min        glm::min
#define Math_Mod        glm::mod
#define Math_Normalize  glm::normalize
#define Math_Pow        glm::pow
#define Math_Round      glm::round
#define Math_Rotate     glm::rotate
#define Math_Radians    glm::radians
#define Math_Sign       glm::sign
#define Math_Sin        glm::sin

#define Math_Identity     glm::identity
#define Math_Inverse      glm::inverse
#define Math_LookAt       glm::lookAt
#define Math_Ortho        glm::ortho
#define Math_Perspective  glm::perspective
#define Math_Scale        glm::scale
#define Math_Translate    glm::translate

#define Math_BitCount             cherrysoda::Math::BitCount_
#define Math_RotateVector         cherrysoda::Math::RotateVector_
#define Math_GetOrientationMatrix cherrysoda::Math::GetOrientationMatrix_
#define Math_RaycastAABB          cherrysoda::Math::RaycastAABB_


namespace cherrysoda {

class Math
{
public:
	static constexpr float Pi     = 3.14159265358979323846f;
	static constexpr float Pi2    = Pi * 2.0f;
	static constexpr float PiHalf = Pi * 0.5f;
	static constexpr float PiQuarter = Pi * 0.25f;
	static constexpr float epsf   = 1E-06f;

	static constexpr float FloatMin = std::numeric_limits<float>::min();
	static constexpr float FloatMax = std::numeric_limits<float>::max();

	static constexpr int IntMin = std::numeric_limits<int>::min();
	static constexpr int IntMax = std::numeric_limits<int>::max();

	static const double NaN()
	{
		static double nan = std::nan("");
		return nan;
	}

	static const float NaNf()
	{
		static float nanf = std::nanf("");
		return nanf;
	}

	using Vec2 = glm::vec2;
	using Vec3 = glm::vec3;
	using Vec4 = glm::vec4;
	using Mat4 = glm::mat4;
	using Quat = glm::quat;

	using IVec2 = glm::i32vec2;
	using IVec3 = glm::i32vec3;
	using IVec4 = glm::i32vec4;

	struct AABB
	{
		Vec3 m_min;
		Vec3 m_max;
	};

	struct Rectangle
	{
		Vec2 m_coord;
		Vec2 m_size;

		inline float X() const { return m_coord.x; }
		inline float Y() const { return m_coord.y; }
		inline Vec2 Size() const { return m_size; }
		inline float Width() const { return m_size.x; }
		inline float Height() const { return m_size.y; }
		inline float Left() const { return X(); }
		inline float Right() const { return X() + Width(); }
		inline float Top() const { return Y() + Height(); }
		inline float Bottom() const { return Y(); }

		inline void Move(const Vec2& delta) { m_coord += delta; }
		inline void Size(const Vec2& size) { m_size = size; }
		inline Vec2 Clamp(const Vec2& pos) const { return Vec2(Math_Clamp(pos.x, Left(), Right()), Math_Clamp(pos.y, Bottom(), Top())); }

		inline bool Intersects(const Rectangle& rect) const
		{
			return Left() < rect.Right() && Right() > rect.Left() && Bottom() < rect.Top() && Top() > rect.Bottom();
		}
	};

	struct IRectangle
	{
		IVec2 m_coord;
		IVec2 m_size;

		inline int X() const { return m_coord.x; }
		inline int Y() const { return m_coord.y; }
		inline IVec2 Size() const { return m_size; }
		inline int Width() const { return m_size.x; }
		inline int Height() const { return m_size.y; }
		inline int Left() const { return X(); }
		inline int Right() const { return X() + Width(); }
		inline int Top() const { return Y() + Height(); }
		inline int Bottom() const { return Y(); }

		inline void Move(const IVec2& delta) { m_coord += delta; }
		inline IVec2 Clamp(const IVec2& pos) const { return IVec2(Math_Clamp(pos.x, Left(), Right()), Math_Clamp(pos.y, Bottom(), Top())); }

		inline bool operator == (const IRectangle& rect) const { return m_coord == rect.m_coord && m_size == rect.m_size; }
		inline bool operator != (const IRectangle& rect) const { return m_coord != rect.m_coord || m_size != rect.m_size; }
	};

	static inline Vec3 RotateVector_(const Vec3& v3, float angle, const Vec3& axis = Vec3_ZUp)
	{
		return Vec3(Math_Rotate(Math_Identity<Mat4>(), angle, axis) * Vec4(v3, 1.f));
	}

	static inline Vec2 RotateVector_(const Vec2& v2, float angle)
	{
		return Vec2(RotateVector_(Vec3(v2, 0.f), angle));
	}

	static const Mat4 GetOrientationMatrix_(const Mat4& matrix);

	static bool RaycastAABB_(const Vec3& start, const Vec3& direction, const AABB& aabb, float* t1 = nullptr, float* t2 = nullptr);

	template <typename T>
	static inline int BitCount_(T x)
	{
		int ret = 0;
		while (x) { x &= ~(x & (-x)); ++ret; }
		return ret;
	}
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_MATH_H_
