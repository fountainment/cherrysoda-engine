#include <CherrySoda/Input/MInput.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Util/Log.h>
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
	SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt");

	// TODO: Remove temporary code
	SDL_GameController *controller = nullptr;
	for (int i = 0; i < SDL_NumJoysticks(); ++i) {
		if (SDL_IsGameController(i)) {
			controller = SDL_GameControllerOpen(i);
			if (controller) {
				CHERRYSODA_DEBUG(StringUtil::Format("Successfully open gamecontroller %i", i));
				ms_internalDevices[0] = (void*)controller;
				break;
			} else {
				CHERRYSODA_DEBUG(StringUtil::Format("Could not open gamecontroller %i: %s", i, SDL_GetError()));
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

	GamePadState builtState;
	builtState.m_thumbSticks = GamePadThumbSticks(stickLeft, stickRight);
	builtState.m_connected = true;
	return builtState;
}

bool MInput::SetGamePadVibration(int index, float leftMotor, float rightMotor)
{
	// TODO: Implement MInput::SetGamePadVibration
	return true;
}

MInput::GamePadData* MInput::ms_gamePads[4] = { nullptr, nullptr, nullptr, nullptr };
void* MInput::ms_internalDevices[4] = { nullptr, nullptr, nullptr, nullptr };
