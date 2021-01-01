#include <CherrySoda/Input/MInput.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/Profile.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

#include <SDL2/SDL.h>

using cherrysoda::MInput;

using cherrysoda::Buttons;
using cherrysoda::ButtonState;
using cherrysoda::Engine;
using cherrysoda::Keys;
using cherrysoda::Math;
using cherrysoda::STL;
using cherrysoda::StringUtil;

namespace type = cherrysoda::type;

void MInput::GamePadData::Update()
{
	m_previousState = m_currentState;
	m_currentState = MInput::GetGamePadState((int)m_playerIndex);
	m_attached = m_currentState.m_connected;

	if (m_rumbleTime > 0.f) {
		m_rumbleTime -= Engine::Instance()->DeltaTime();
		if (m_rumbleTime <= 0.f) {
			MInput::SetGamePadVibration((int)m_playerIndex, 0.f, 0.f);
		}
	}
}

void MInput::GamePadData::UpdateNull()
{
	m_previousState = m_currentState;
	m_currentState = MInput::GamePadState();
	m_attached = MInput::GetGamePadState((int)m_playerIndex).m_connected;

	if (m_rumbleTime > 0.f) {
		m_rumbleTime -= Engine::Instance()->DeltaTime();
		if (m_rumbleTime <= 0.f) {
			MInput::SetGamePadVibration((int)m_playerIndex, 0.f, 0.f);
		}
	}
}

void MInput::GamePadData::Rumble(float strength, float time)
{
	if (m_rumbleTime <= 0.f || strength > m_rumbleStrength || (strength == m_rumbleStrength && time > m_rumbleTime)) {
		MInput::SetGamePadVibration((int)m_playerIndex, strength, strength);
		m_rumbleStrength = strength;
		m_rumbleTime = time;
	}
}

void MInput::GamePadData::StopRumble()
{
	MInput::SetGamePadVibration((int)m_playerIndex, 0.f, 0.f);
	m_rumbleTime = 0.f;
}

bool MInput::KeyboardState::InternalGetKey(Keys key) const
{
	type::UInt32 mask = (type::UInt32) 1 << ((int)key & 0x1f);
	type::UInt32 element;
	switch (((int)key) >> 5) {
		case 0: element = keys0; break;
		case 1: element = keys1; break;
		case 2: element = keys2; break;
		case 3: element = keys3; break;
		case 4: element = keys4; break;
		case 5: element = keys5; break;
		case 6: element = keys6; break;
		case 7: element = keys7; break;
		default: element = 0; break;
	};
	return (element & mask) != 0;
}

void MInput::KeyboardState::InternalSetKey(Keys key)
{
	type::UInt32 mask = (type::UInt32) 1 << ((int)key & 0x1f);
	switch (((int)key) >> 5) {
		case 0: keys0 |= mask; break;
		case 1: keys1 |= mask; break;
		case 2: keys2 |= mask; break;
		case 3: keys3 |= mask; break;
		case 4: keys4 |= mask; break;
		case 5: keys5 |= mask; break;
		case 6: keys6 |= mask; break;
		case 7: keys7 |= mask; break;
	}
}

void MInput::Initialize()
{
	SDL_InitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC);
	SDL_JoystickEventState(SDL_ENABLE);
	SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt");

	// TODO: Remove temporary code
	SDL_GameController *controller = nullptr;
	for (int i = 0; i < SDL_NumJoysticks(); ++i) {
		if (SDL_IsGameController(i)) {
			controller = SDL_GameControllerOpen(i);
			if (controller) {
				CHERRYSODA_DEBUG_FORMAT("Successfully open gamecontroller %i\n", i);
				ms_internalDevices[0] = (void*)controller;
				break;
			} else {
				CHERRYSODA_DEBUG_FORMAT("Could not open gamecontroller %i: %s\n", i, SDL_GetError());
			}
		}
	}

	ms_keyboard = new KeyboardData();
	ms_mouse = new MouseData();
	for (int i = 0; i < 4; ++i) {
		ms_gamePads[i] = new GamePadData((PlayerIndex)i);
	}

	StringID platform = SDL_GetPlatform();
	if (platform == StringID("Windows") ||
		platform == StringID("Mac OS X") ||
		platform == StringID("Linux") ||
		platform == StringID("OpenBSD") ||
		platform == StringID("FreeBSD") ||
		platform == StringID("NetBSD")) {
		ms_supportsGlobalMouse = true;
	}
	else {
		ms_supportsGlobalMouse = false;
	}

	// TODO: VirtualInputs
}

void MInput::Update()
{
	CHERRYSODA_PROFILE_FUNCTION();

	// TODO: Use gamepad event to detect gamepad connection
	if (ms_internalDevices[0] == nullptr && SDL_NumJoysticks() > 0) {
		ms_internalDevices[0] = (void*)SDL_GameControllerOpen(0);
	}

	ms_keyboard->Update();
	ms_mouse->Update();
	for (int i = 0; i < 4; ++i) {
		ms_gamePads[i]->Update();
	}
	// TODO: UpdateNull when !IsActive
	// TODO: VirtualInputs
}

void MInput::UpdateNull()
{
	ms_keyboard->UpdateNull();
	ms_mouse->UpdateNull();
	for (int i = 0; i < 4; ++i) {
		ms_gamePads[i]->UpdateNull();
	}
	// TODO: VirtualInputs
}

const MInput::KeyboardState MInput::GetKeyboardState()
{
	return MInput::KeyboardState(ms_keyboardKeys);
}

const MInput::MouseState MInput::GetMouseState()
{
	int x = 0, y = 0;
	ButtonState left, middle, right;
	ButtonState x1, x2;
	type::UInt32 flags = 0;
	if (GetRelativeMouseMode()) {
		flags = SDL_GetRelativeMouseState(&x, &y);	
	}
#if SDL_VERSION_ATLEAST(2,0,4)
	else if (ms_supportsGlobalMouse) {
		flags = SDL_GetGlobalMouseState(&x, &y);
		auto pos = Engine::Instance()->GetWindowPosition();
		x -= pos.x;
		y -= pos.y;
	}
#endif // SDL_VERSION_ATLEAST(2,0,4)
	else {
		flags = SDL_GetMouseState(&x, &y);	
	}
	left = (ButtonState)(flags & SDL_BUTTON_LMASK);
	middle = (ButtonState)((flags & SDL_BUTTON_MMASK) >> 1);
	right = (ButtonState)((flags & SDL_BUTTON_RMASK) >> 2);
	x1 = (ButtonState)((flags & SDL_BUTTON_X1MASK) >> 3);
	x2 = (ButtonState)((flags & SDL_BUTTON_X2MASK) >> 4);
	return MInput::MouseState(x, y, ms_internalMouseWheel, left, middle, right, x1, x2);
}

const MInput::GamePadState MInput::GetGamePadState(int index)
{
	// TODO: Implement MInput::GetGamePadState
	SDL_GameController* device = (SDL_GameController*)ms_internalDevices[index];
	if (device == nullptr)
	{
		return GamePadState();
	}

	// Sticks
	Math::Vec2 stickLeft(
		(float)SDL_GameControllerGetAxis(
			device,
			SDL_CONTROLLER_AXIS_LEFTX
		) / 32767.0f,
		(float)SDL_GameControllerGetAxis(
			device,
			SDL_CONTROLLER_AXIS_LEFTY
		) / -32767.0f
	);
	Math::Vec2 stickRight(
		(float)SDL_GameControllerGetAxis(
			device,
			SDL_CONTROLLER_AXIS_RIGHTX
		) / 32767.0f,
		(float)SDL_GameControllerGetAxis(
			device,
			SDL_CONTROLLER_AXIS_RIGHTY
		) / -32767.0f
	);

	// Triggers
	float triggerLeft = (float)SDL_GameControllerGetAxis(
		device,
		SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERLEFT
	) / 32767.0f;
	float triggerRight = (float)SDL_GameControllerGetAxis(
		device,
		SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERRIGHT
	) / 32767.0f;

	// Buttons
	Buttons buttonState = Buttons::None;
	if (SDL_GameControllerGetButton(device, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A) != 0) {
		buttonState |= Buttons::A;
	}
	if (SDL_GameControllerGetButton(device, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B) != 0) {
		buttonState |= Buttons::B;
	}
	if (SDL_GameControllerGetButton(device, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X) != 0) {
		buttonState |= Buttons::X;
	}
	if (SDL_GameControllerGetButton(device, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_Y) != 0) {
		buttonState |= Buttons::Y;
	}
	if (SDL_GameControllerGetButton(device, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_BACK) != 0) {
		buttonState |= Buttons::Back;
	}
	if (SDL_GameControllerGetButton(device, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_GUIDE) != 0) {
		buttonState |= Buttons::BigButton;
	}
	if (SDL_GameControllerGetButton(device, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START) != 0) {
		buttonState |= Buttons::Start;
	}
	if (SDL_GameControllerGetButton(device, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSTICK) != 0) {
		buttonState |= Buttons::LeftStick;
	}
	if (SDL_GameControllerGetButton(device, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSTICK) != 0) {
		buttonState |= Buttons::RightStick;
	}
	if (SDL_GameControllerGetButton(device, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSHOULDER) != 0) {
		buttonState |= Buttons::LeftShoulder;
	}
	if (SDL_GameControllerGetButton(device, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) != 0) {
		buttonState |= Buttons::RightShoulder;
	}

	// DPad
	ButtonState dpadUp = ButtonState::Released;
	ButtonState dpadDown = ButtonState::Released;
	ButtonState dpadLeft = ButtonState::Released;
	ButtonState dpadRight = ButtonState::Released;

	if (SDL_GameControllerGetButton(device, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP) != 0) {
		buttonState |= Buttons::DPadUp;
		dpadUp = ButtonState::Pressed;
	}
	if (SDL_GameControllerGetButton(device, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN) != 0) {
		buttonState |= Buttons::DPadDown;
		dpadDown = ButtonState::Pressed;
	}
	if (SDL_GameControllerGetButton(device, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT) != 0) {
		buttonState |= Buttons::DPadLeft;
		dpadLeft = ButtonState::Pressed;
	}
	if (SDL_GameControllerGetButton(device, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT) != 0) {
		buttonState |= Buttons::DPadRight;
		dpadRight = ButtonState::Pressed;
	}

	GamePadState builtState(
		GamePadThumbSticks(stickLeft, stickRight),
		GamePadTriggers(triggerLeft, triggerRight),
		GamePadButtons(buttonState),
		GamePadDPad(dpadUp, dpadDown, dpadLeft, dpadRight)
	);
	builtState.m_connected = true;
	return builtState;
}

bool MInput::GetRelativeMouseMode()
{
	return SDL_GetRelativeMouseMode() == SDL_TRUE;	
}

void MInput::SetMousePosition(const Math::IVec2& pos)
{
	Engine::Instance()->SetMousePosition(pos);	
}

bool MInput::SetGamePadVibration(int index, float leftMotor, float rightMotor)
{
	SDL_GameController* device = (SDL_GameController*)ms_internalDevices[index];
	if (device == nullptr)
	{
		return false;
	}

#if SDL_VERSION_ATLEAST(2,0,9)
	return SDL_GameControllerRumble(
		device,
		(type::UInt16)((Math_Clamp(leftMotor, 0.0f, 1.0f) * 0xFFFF)),
		(type::UInt16)((Math_Clamp(rightMotor, 0.0f, 1.0f) * 0xFFFF)),
		0
	) == 0;
#endif // SDL_VERSION_ATLEAST(2,0,9)
	return true;
}

STL::List<Keys> MInput::ms_keyboardKeys;
MInput::KeyboardData* MInput::ms_keyboard = nullptr;
MInput::MouseData* MInput::ms_mouse = nullptr;
MInput::GamePadData* MInput::ms_gamePads[4] = { nullptr, nullptr, nullptr, nullptr };
void* MInput::ms_internalDevices[4] = { nullptr, nullptr, nullptr, nullptr };

int MInput::ms_internalMouseWheel = 0;
bool MInput::ms_supportsGlobalMouse = true;
