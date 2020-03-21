#include <CherrySoda/Util/Math.h>

using cherrysoda::Math;

const Math::Mat4 Math::GetOrientationMatrix_(const Math::Mat4& matrix)
{
	Vec3 scale, translation, skew;
	Quat rotation;
	Vec4 perspective;
	glm::decompose(matrix, scale, rotation, translation, skew, perspective);
	rotation = glm::conjugate(rotation);
	return glm::toMat4(rotation);
}

bool Math::RaycastAABB_(const Math::Vec3& start, const Math::Vec3& direction, const Math::AABB& aabb, float* t1 /* = nullptr */, float* t2 /* = nullptr */)
{
	/*
	x = a + tx'
	y = b + ty'
	z = c + tz'
	*/

	Vec3 startBC = start - aabb.m_min;
	Vec3 size = aabb.m_max - aabb.m_min;
	float intervalT1 = 0.f;
	float intervalT2 = FLT_MAX;
	for (int i = 0; i < 3; ++i)
	{
		if (direction[i] == 0.f) {
			if ((0.f - startBC[i]) * (size[i] - startBC[i]) < 0.f) {
				continue;
			}
			return false;
		}
		float tmpT1 = (0.f - startBC[i]) / direction[i];
		float tmpT2 = (size[i] - startBC[i]) / direction[i];
		float inT1 = Math_Max(Math_Min(intervalT1, intervalT2), Math_Min(tmpT1, tmpT2));
		intervalT2 = Math_Min(Math_Max(intervalT1, intervalT2), Math_Max(tmpT1, tmpT2));
		intervalT1 = inT1;
		if (intervalT1 >= intervalT2) {
			return false;
		}
	}
	if (t1 != nullptr) {
		*t1 = intervalT1;
	}
	if (t2 != nullptr) {
		*t2 = intervalT2;
	}
	return true;
}
