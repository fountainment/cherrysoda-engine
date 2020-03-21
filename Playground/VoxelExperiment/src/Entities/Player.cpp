#include "Player.h"

#include <CherrySoda/CherrySoda.h>

using namespace cherrysoda;

void PlayerControl::Update()
{
	auto player = EntityAs<Player>();
	CHERRYSODA_ASSERT(player, "PlayerControl can only be used on Player!\n");

	float deltaTime = Engine::Instance()->DeltaTime();
	Math::Vec2 leftStick = MInput::GamePads(0)->GetLeftStick(0.2f);
	Math::Vec2 rightStick = MInput::GamePads(0)->GetRightStick(0.2f);
	leftStick *= Math_Length(leftStick) * 40.0f;
	rightStick *= Math_Length(rightStick) * Math::Pi * Math::Vec2(1.2f, 1.f);

	player->RotateXY(deltaTime * rightStick);
	player->MoveXY(deltaTime * leftStick);
}

void PlayerCamera::Update()
{
	auto player = EntityAs<Player>();
	CHERRYSODA_ASSERT(player, "PlayerCamera can only be used on Player!\n");

	GetCamera()->Direction(player->FacingDirection());
	GetCamera()->Position(player->Position());
}