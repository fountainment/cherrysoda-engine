#include <CherrySoda/Input/MInput.h>

#include <CherrySoda/Input/VirtualInput.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Util/GUI.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/Profile.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

#include <SDL3/SDL.h>

namespace cherrysoda {

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
	type::UInt32 mask = (type::UInt32)1 << ((int)key & 0x1f);
	type::UInt32 element;
	switch (((int)key) >> 5) {
	case 0:
		element = keys0;
		break;
	case 1:
		element = keys1;
		break;
	case 2:
		element = keys2;
		break;
	case 3:
		element = keys3;
		break;
	case 4:
		element = keys4;
		break;
	case 5:
		element = keys5;
		break;
	case 6:
		element = keys6;
		break;
	case 7:
		element = keys7;
		break;
	default:
		element = 0;
		break;
	};
	return (element & mask) != 0;
}

void MInput::KeyboardState::InternalSetKey(Keys key)
{
	type::UInt32 mask = (type::UInt32)1 << ((int)key & 0x1f);
	switch (((int)key) >> 5) {
	case 0:
		keys0 |= mask;
		break;
	case 1:
		keys1 |= mask;
		break;
	case 2:
		keys2 |= mask;
		break;
	case 3:
		keys3 |= mask;
		break;
	case 4:
		keys4 |= mask;
		break;
	case 5:
		keys5 |= mask;
		break;
	case 6:
		keys6 |= mask;
		break;
	case 7:
		keys7 |= mask;
		break;
	}
}

const Math::IVec2 MInput::MouseData::Position() const
{
	return Math::IVec2(m_currentState.m_x, Engine::Instance()->GetWindowHeight() - m_currentState.m_y);
}

void MInput::MouseData::Position(const Math::IVec2& pos)
{
	SetMousePosition(Math::IVec2(pos.x, Engine::Instance()->GetWindowHeight() - pos.y));
}

void MInput::Initialize()
{
	SDL_AddGamepadMappingsFromFile("gamecontrollerdb.txt");

	SDL_InitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMEPAD);

	const char* hint = SDL_GetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS);
	if (hint == nullptr) {
		SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
	}

	SDL_PumpEvents();
	SDL_Event evt[1];
	while (SDL_PeepEvents(evt, 1, SDL_GETEVENT, SDL_EVENT_GAMEPAD_ADDED, SDL_EVENT_GAMEPAD_ADDED) == 1) {
		AddControllerInstance((int)evt[0].gdevice.which);
	}

	ms_keyboard = new KeyboardData();
	ms_mouse = new MouseData();
	for (int i = 0; i < 4; ++i) {
		ms_gamePads[i] = new GamePadData((PlayerIndex)i);
	}

	StringID platform = SDL_GetPlatform();
	if (platform == StringID("Windows") || platform == StringID("macOS") || platform == StringID("Linux") ||
		platform == StringID("OpenBSD") || platform == StringID("FreeBSD") || platform == StringID("NetBSD")) {
		ms_supportsGlobalMouse = true;
	}
	else {
		ms_supportsGlobalMouse = false;
	}
}

void MInput::Terminate()
{
	delete ms_keyboard;
	ms_keyboard = nullptr;
	delete ms_mouse;
	ms_mouse = nullptr;
	for (int i = 0; i < 4; ++i) {
		delete ms_gamePads[i];
		ms_gamePads[i] = nullptr;
	}
	SDL_QuitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMEPAD);
}

void MInput::Update()
{
	CHERRYSODA_PROFILE_FUNCTION();

	if (Engine::Instance()->IsActive()) {
		if (Engine::Instance()->ConsoleOpened() && GUI::ConsoleWindowFocused()) {
			ms_keyboard->UpdateNull();
		}
		else {
			ms_keyboard->Update();
		}
		if (Engine::Instance()->ConsoleOpened() && (GUI::ConsoleWindowFocused() || GUI::SliderWindowFocused())) {
			ms_mouse->UpdateNull();
		}
		else {
			ms_mouse->Update();
		}
		for (int i = 0; i < 4; ++i) {
			ms_gamePads[i]->Update();
		}
	}
	else {
		ms_keyboard->UpdateNull();
		ms_mouse->UpdateNull();
		for (int i = 0; i < 4; ++i) {
			ms_gamePads[i]->UpdateNull();
		}
	}

	VirtualInput::UpdateVirtualInputs();
}

void MInput::UpdateNull()
{
	ms_keyboard->UpdateNull();
	ms_mouse->UpdateNull();
	for (int i = 0; i < 4; ++i) {
		ms_gamePads[i]->UpdateNull();
	}
}

const MInput::KeyboardState MInput::GetKeyboardState()
{
	return MInput::KeyboardState(ms_keyboardKeys);
}

const MInput::MouseState MInput::GetMouseState()
{
	float x = 0.f, y = 0.f;
	ButtonState left, middle, right;
	ButtonState x1, x2;
	type::UInt32 flags = 0;
	if (GetRelativeMouseMode()) {
		flags = SDL_GetRelativeMouseState(&x, &y);
	}
	else if (ms_supportsGlobalMouse) {
		flags = SDL_GetGlobalMouseState(&x, &y);
		auto pos = Engine::Instance()->GetWindowPosition();
		x -= pos.x;
		y -= pos.y;
	}
	else {
		flags = SDL_GetMouseState(&x, &y);
	}
	left = (ButtonState)(flags & SDL_BUTTON_LMASK);
	middle = (ButtonState)((flags & SDL_BUTTON_MMASK) >> 1);
	right = (ButtonState)((flags & SDL_BUTTON_RMASK) >> 2);
	x1 = (ButtonState)((flags & SDL_BUTTON_X1MASK) >> 3);
	x2 = (ButtonState)((flags & SDL_BUTTON_X2MASK) >> 4);
	return MInput::MouseState((int)x, (int)y, ms_internalMouseWheel, left, middle, right, x1, x2);
}

const MInput::GamePadState MInput::GetGamePadState(int index)
{
	SDL_Gamepad* device = (SDL_Gamepad*)ms_internalDevices[index];
	if (device == nullptr) {
		return GamePadState();
	}

	// Sticks
	Math::Vec2 stickLeft((float)SDL_GetGamepadAxis(device, SDL_GAMEPAD_AXIS_LEFTX) / 32767.0f,
						 (float)SDL_GetGamepadAxis(device, SDL_GAMEPAD_AXIS_LEFTY) / -32767.0f);
	Math::Vec2 stickRight((float)SDL_GetGamepadAxis(device, SDL_GAMEPAD_AXIS_RIGHTX) / 32767.0f,
						  (float)SDL_GetGamepadAxis(device, SDL_GAMEPAD_AXIS_RIGHTY) / -32767.0f);

	// Triggers
	float triggerLeft = (float)SDL_GetGamepadAxis(device, SDL_GAMEPAD_AXIS_LEFT_TRIGGER) / 32767.0f;
	float triggerRight = (float)SDL_GetGamepadAxis(device, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) / 32767.0f;

	// Buttons
	Buttons buttonState = Buttons::None;
	if (SDL_GetGamepadButton(device, SDL_GAMEPAD_BUTTON_SOUTH)) {
		buttonState |= Buttons::A;
	}
	if (SDL_GetGamepadButton(device, SDL_GAMEPAD_BUTTON_EAST)) {
		buttonState |= Buttons::B;
	}
	if (SDL_GetGamepadButton(device, SDL_GAMEPAD_BUTTON_WEST)) {
		buttonState |= Buttons::X;
	}
	if (SDL_GetGamepadButton(device, SDL_GAMEPAD_BUTTON_NORTH)) {
		buttonState |= Buttons::Y;
	}
	if (SDL_GetGamepadButton(device, SDL_GAMEPAD_BUTTON_BACK)) {
		buttonState |= Buttons::Back;
	}
	if (SDL_GetGamepadButton(device, SDL_GAMEPAD_BUTTON_GUIDE)) {
		buttonState |= Buttons::BigButton;
	}
	if (SDL_GetGamepadButton(device, SDL_GAMEPAD_BUTTON_START)) {
		buttonState |= Buttons::Start;
	}
	if (SDL_GetGamepadButton(device, SDL_GAMEPAD_BUTTON_LEFT_STICK)) {
		buttonState |= Buttons::LeftStick;
	}
	if (SDL_GetGamepadButton(device, SDL_GAMEPAD_BUTTON_RIGHT_STICK)) {
		buttonState |= Buttons::RightStick;
	}
	if (SDL_GetGamepadButton(device, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER)) {
		buttonState |= Buttons::LeftShoulder;
	}
	if (SDL_GetGamepadButton(device, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER)) {
		buttonState |= Buttons::RightShoulder;
	}

	// DPad
	ButtonState dpadUp = ButtonState::Released;
	ButtonState dpadDown = ButtonState::Released;
	ButtonState dpadLeft = ButtonState::Released;
	ButtonState dpadRight = ButtonState::Released;

	if (SDL_GetGamepadButton(device, SDL_GAMEPAD_BUTTON_DPAD_UP)) {
		buttonState |= Buttons::DPadUp;
		dpadUp = ButtonState::Pressed;
	}
	if (SDL_GetGamepadButton(device, SDL_GAMEPAD_BUTTON_DPAD_DOWN)) {
		buttonState |= Buttons::DPadDown;
		dpadDown = ButtonState::Pressed;
	}
	if (SDL_GetGamepadButton(device, SDL_GAMEPAD_BUTTON_DPAD_LEFT)) {
		buttonState |= Buttons::DPadLeft;
		dpadLeft = ButtonState::Pressed;
	}
	if (SDL_GetGamepadButton(device, SDL_GAMEPAD_BUTTON_DPAD_RIGHT)) {
		buttonState |= Buttons::DPadRight;
		dpadRight = ButtonState::Pressed;
	}

	// Extensions
	if (SDL_GetGamepadButton(device, SDL_GAMEPAD_BUTTON_MISC1)) {
		buttonState |= Buttons::Misc1EXT;
	}
	if (SDL_GetGamepadButton(device, SDL_GAMEPAD_BUTTON_LEFT_PADDLE1)) {
		buttonState |= Buttons::Paddle1EXT;
	}
	if (SDL_GetGamepadButton(device, SDL_GAMEPAD_BUTTON_RIGHT_PADDLE1)) {
		buttonState |= Buttons::Paddle2EXT;
	}
	if (SDL_GetGamepadButton(device, SDL_GAMEPAD_BUTTON_LEFT_PADDLE2)) {
		buttonState |= Buttons::Paddle3EXT;
	}
	if (SDL_GetGamepadButton(device, SDL_GAMEPAD_BUTTON_RIGHT_PADDLE2)) {
		buttonState |= Buttons::Paddle4EXT;
	}
	if (SDL_GetGamepadButton(device, SDL_GAMEPAD_BUTTON_TOUCHPAD)) {
		buttonState |= Buttons::TouchPadEXT;
	}

	GamePadState builtState(GamePadThumbSticks(stickLeft, stickRight), GamePadTriggers(triggerLeft, triggerRight),
							GamePadButtons(buttonState), GamePadDPad(dpadUp, dpadDown, dpadLeft, dpadRight));
	builtState.m_connected = true;
	return builtState;
}

bool MInput::GetRelativeMouseMode()
{
	return SDL_GetWindowRelativeMouseMode(SDL_GetMouseFocus());
}

void MInput::SetMousePosition(const Math::IVec2& pos)
{
	Engine::Instance()->SetMousePosition(pos);
}

bool MInput::SetGamePadVibration(int index, float leftMotor, float rightMotor)
{
	SDL_Gamepad* device = (SDL_Gamepad*)ms_internalDevices[index];
	if (device == nullptr) {
		return false;
	}

	return SDL_RumbleGamepad(device, (type::UInt16)((Math_Clamp(leftMotor, 0.0f, 1.0f) * 0xFFFF)),
							 (type::UInt16)((Math_Clamp(rightMotor, 0.0f, 1.0f) * 0xFFFF)), 0);
}

void MInput::AddControllerInstance(int instanceID)
{
	int which = -1;

	for (int i = 0; i < 4; ++i) {
		if (ms_internalDevices[i] == nullptr) {
			which = i;
			break;
		}
	}

	if (which == -1) {
		return;
	}

	ms_internalDevices[which] = SDL_OpenGamepad((SDL_JoystickID)instanceID);
	ms_internalInstanceMap[instanceID] = which;
}

void MInput::RemoveControllerInstance(int instanceID)
{
	int output;
	if (!STL::TryGetValue(ms_internalInstanceMap, instanceID, output)) {
		return;
	}

	STL::RemoveKey(ms_internalInstanceMap, instanceID);
	SDL_CloseGamepad((SDL_Gamepad*)ms_internalDevices[output]);
	ms_internalDevices[output] = nullptr;
}

STL::List<Keys> MInput::ms_keyboardKeys;
MInput::KeyboardData* MInput::ms_keyboard = nullptr;
MInput::MouseData* MInput::ms_mouse = nullptr;
MInput::GamePadData* MInput::ms_gamePads[4] = {nullptr, nullptr, nullptr, nullptr};
void* MInput::ms_internalDevices[4] = {nullptr, nullptr, nullptr, nullptr};
STL::Map<int, int> MInput::ms_internalInstanceMap;

int MInput::ms_internalMouseWheel = 0;
bool MInput::ms_supportsGlobalMouse = true;

} // namespace cherrysoda
