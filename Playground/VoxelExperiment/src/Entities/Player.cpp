#include "Player.h"

#include <CherrySoda/CherrySoda.h>

using namespace cherrysoda;

void PlayerControl::Update()
{
	auto player = GetEntityAs<Player>();

	float deltaTime = Engine::Instance()->DeltaTime();

	Math::Vec2 leftStick = MInput::GamePads(0)->GetLeftStick(0.2f);
	Math::Vec2 rightStick = MInput::GamePads(0)->GetRightStick(0.2f);
	float leftTrigger = MInput::GamePads(0)->GetLeftTrigger();
	float rightTrigger = MInput::GamePads(0)->GetRightTrigger();

	leftStick += MInput::Keyboard()->GetAxis(Keys::A, Keys::D, Keys::S, Keys::W);
	rightStick += MInput::Keyboard()->GetAxis(Keys::Left, Keys::Right, Keys::Down, Keys::Up);
	leftTrigger += MInput::Keyboard()->Check(Keys::X) ? 1.0f : 0;
	rightTrigger += MInput::Keyboard()->Check(Keys::C) ? 1.0f : 0;

	leftStick *= Math_Length(leftStick) * 40.0f;
	rightStick *= Math_Length(rightStick) * Math::Pi * Math::Vec2(1.2f, 1.f);
	leftTrigger *= 40.f;
	rightTrigger *= 40.f;

	player->RotateXY(deltaTime * rightStick);
	player->MoveXY(deltaTime * leftStick);
	player->MoveZ(deltaTime * (rightTrigger - leftTrigger));
}

void PlayerCamera::Update()
{
	auto player = GetEntityAs<Player>();

	GetCamera()->Direction(player->FacingDirection());
	GetCamera()->Position(player->Position());
}
