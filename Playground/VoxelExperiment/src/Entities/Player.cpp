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
	rightStick *= Math_LengthSq(rightStick) * Math::Pi;

	player->Move(deltaTime * player->FacingDirection() * leftStick[1]);
	player->Move(deltaTime * player->RightDirection() * leftStick[0]);
}

void PlayerCamera::Update()
{
	auto player = EntityAs<Player>();
	CHERRYSODA_ASSERT(player, "PlayerCamera can only be used on Player!\n");
	GetCamera()->Direction(player->FacingDirection());
	GetCamera()->Position(GetEntity()->Position());
}