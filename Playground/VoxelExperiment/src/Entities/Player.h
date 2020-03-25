#ifndef _VOXELEXPERIMENT_PLAYER_H_
#define _VOXELEXPERIMENT_PLAYER_H_

#include <CherrySoda/Entity.h>
#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Util/Math.h>

namespace cherrysoda
{
	class Camera;
} // namespace cherrysoda

class PlayerControl : public cherrysoda::Component
{
public:
	typedef cherrysoda::Component base;

	PlayerControl() : base(true, false) {}

	void Update() override;
};

class PlayerCamera : public cherrysoda::Component
{
public:
	typedef cherrysoda::Component base;

	PlayerCamera(cherrysoda::Camera* camera) : base(true, false)
	, m_camera(camera)
	{
	}

	cherrysoda::Camera* GetCamera() { return m_camera; }

	void Update() override;

private:
	cherrysoda::Camera* m_camera;
};

class Player : public cherrysoda::Entity
{
public:
	Player(cherrysoda::Camera* camera)
	{
		Add(new PlayerControl());
		Add(new PlayerCamera(camera));
	}

	inline void Move(const cherrysoda::Math::Vec3& delta) { Position(Position() + delta); }
	inline void MoveXY(const cherrysoda::Math::Vec2& xy)
	{
		Move(FacingDirection() * xy.y);
		Move(LeftDirection() * -xy.x);
	}
	inline void MoveZ(float z)
	{
		Move(UpDirection() * z);
	}
	inline void RotateXY(const cherrysoda::Math::Vec2& xy)
	{
		m_leftDirection = Math_RotateVector(LeftDirection(), xy.x, UpDirection());
		m_direction = Math_RotateVector(FacingDirection(), xy.x, UpDirection());
		auto newDirection = Math_RotateVector(FacingDirection(), xy.y, LeftDirection());
		if (Math_Dot(Math_Normalize(Math_Cross(newDirection, LeftDirection())), UpDirection()) > 0.f) {
			m_direction = newDirection;
		}
	}
	inline cherrysoda::Math::Vec3 FacingDirection() const { return m_direction; }
	inline cherrysoda::Math::Vec3 LeftDirection() const { return m_leftDirection; }
	inline cherrysoda::Math::Vec3 UpDirection() const { return m_upDirection; }

private:
	cherrysoda::Math::Vec3 m_direction = -Vec3_ZUp;
	cherrysoda::Math::Vec3 m_leftDirection = -Vec3_XUp;
	cherrysoda::Math::Vec3 m_upDirection = Vec3_YUp;
};

#endif // _VOXELEXPERIMENT_PLAYER_H_
