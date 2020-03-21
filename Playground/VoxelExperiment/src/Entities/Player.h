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
	inline cherrysoda::Math::Vec3 FacingDirection() { return m_direction; }
	inline cherrysoda::Math::Vec3 RightDirection() { return Vec3_XUp; }

private:
	cherrysoda::Math::Vec3 m_direction = -Vec3_ZUp;
};

#endif // _VOXELEXPERIMENT_PLAYER_H_