#include <CherrySoda/Input/MInput.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/Profile.h>
#include <CherrySoda/Util/String.h>

#include <SDL2/SDL.h>

using cherrysoda::MInput;

using cherrysoda::StringUtil;

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

	for (int i = 0; i < 4; ++i) {
		ms_gamePads[i] = new GamePadData((PlayerIndex)i);
	}

	// TODO: Keyboard, Mouse, VirtualInputs
}

void MInput::Update()
{
	CHERRYSODA_PROFILE_FUNCTION();

	// TODO: Use gamepad event to detect gamepad connection
	if (ms_internalDevices[0] == nullptr && SDL_NumJoysticks() > 0) {
		ms_internalDevices[0] = (void*)SDL_GameControllerOpen(0);
	}

	for (int i = 0; i < 4; ++i) {
		ms_gamePads[i]->Update();
	}
	// TODO: UpdateNull when !IsActive
	// TODO: Keyboard, Mouse, VirtualInputs
}

void MInput::UpdateNull()
{
	for (int i = 0; i < 4; ++i) {
		ms_gamePads[i]->UpdateNull();
	}	
	// TODO: Keyboard, Mouse, VirtualInputs
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

bool MInput::SetGamePadVibration(int index, float leftMotor, float rightMotor)
{
	SDL_GameController* device = (SDL_GameController*)ms_internalDevices[index];
	if (device == nullptr)
	{
		return false;
	}

#ifdef _WIN32
	return SDL_GameControllerRumble(
		device,
		(type::UInt16)((Math_Clamp(leftMotor, 0.0f, 1.0f) * 0xFFFF)),
		(type::UInt16)((Math_Clamp(rightMotor, 0.0f, 1.0f) * 0xFFFF)),
		0
	) == 0;
#endif // _WIN32
	return true;
}

MInput::GamePadData* MInput::ms_gamePads[4] = { nullptr, nullptr, nullptr, nullptr };
void* MInput::ms_internalDevices[4] = { nullptr, nullptr, nullptr, nullptr };
