#ifndef _CHERRYSODA_INPUT_MINPUT_H_
#define _CHERRYSODA_INPUT_MINPUT_H_

#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>

namespace cherrysoda {

enum class PlayerIndex : type::UInt8
{
	One = 0,
	Two = 1,
	Three = 2,
	Four = 3
};

enum class ButtonState : type::UInt8
{
	Released = 0,
	Pressed = 1
};

enum class Buttons : type::UInt32
{
	None = 0,
	DPadUp = 1,
	DPadDown = 2,
	DPadLeft = 4,
	DPadRight = 8,
	Start = 16,
	Back = 32,
	LeftStick = 64,
	RightStick = 128,
	LeftShoulder = 256,
	RightShoulder = 512,
	BigButton = 2048,
	A = 4096,
	B = 8192,
	X = 16384,
	Y = 32768,
	LeftThumbstickLeft = 2097152,
	RightTrigger = 4194304,
	LeftTrigger = 8388608,
	RightThumbstickUp = 16777216,
	RightThumbstickDown = 33554432,
	RightThumbstickRight = 67108864,
	RightThumbstickLeft = 134217728,
	LeftThumbstickUp = 268435456,
	LeftThumbstickDown = 536870912,
	LeftThumbstickRight = 1073741824
};

DECLARE_ENUM_FLAG(Buttons);

class MInput
{
public:
	friend class Engine;

	struct GamePadThumbSticks
	{
		inline GamePadThumbSticks() = default;

		inline GamePadThumbSticks(const Math::Vec2& leftThumbstick, const Math::Vec2& rightThumbStick)
		: m_left(leftThumbstick)
		, m_right(rightThumbStick)
		{}

		Math::Vec2 m_left  = Vec2_Zero;
		Math::Vec2 m_right = Vec2_Zero;
	};

	struct GamePadTriggers
	{
		inline GamePadTriggers() = default;

		inline GamePadTriggers(float left, float right)
		: m_left(left)
		, m_right(right)
		{}

		float m_left  = 0.f;
		float m_right = 0.f;
	};

	struct GamePadButtons
	{
		inline GamePadButtons() = default;

		inline GamePadButtons(Buttons buttons)
		: m_buttons(buttons)
		{}

		Buttons m_buttons = Buttons::None;
	};

	struct GamePadDPad
	{
		inline GamePadDPad() = default;

		inline GamePadDPad(ButtonState dpadUp, ButtonState dpadDown, ButtonState dpadLeft, ButtonState dpadRight)
		: m_up(dpadUp)
		, m_down(dpadDown)
		, m_left(dpadLeft)
		, m_right(dpadRight)
		{}

		ButtonState m_up    = ButtonState::Released;
		ButtonState m_down  = ButtonState::Released;
		ButtonState m_left  = ButtonState::Released;
		ButtonState m_right = ButtonState::Released;
	};

	struct GamePadState
	{
		inline GamePadState() = default;

		inline GamePadState(const GamePadThumbSticks& thumSticks, const GamePadTriggers& triggers, const GamePadButtons& buttons, const GamePadDPad& dpad)
		: m_thumbSticks(thumSticks)
		, m_triggers(triggers)
		, m_buttons(buttons)
		, m_dpad(dpad)
		{}

		inline bool IsButtonDown(Buttons buttons) const
		{
			return (m_buttons.m_buttons & buttons) == buttons;
		}

		inline bool IsButtonUp(Buttons buttons) const
		{
			return (m_buttons.m_buttons & buttons) != buttons;
		}

		GamePadThumbSticks m_thumbSticks;
		GamePadTriggers m_triggers;
		GamePadButtons m_buttons;
		GamePadDPad m_dpad;
		bool m_connected = false;
	};

	class GamePadData
	{
	public:
		friend class MInput;

		void Update();
		void UpdateNull();

		void Rumble(float strength, float time);
		void StopRumble();

		// Buttons
		inline bool Check(Buttons button)    const { return m_currentState.IsButtonDown(button); }
		inline bool Pressed(Buttons button)  const { return m_currentState.IsButtonDown(button) && m_previousState.IsButtonUp(button); }
		inline bool Released(Buttons button) const { return m_currentState.IsButtonUp(button) && m_previousState.IsButtonDown(button); }

		inline bool Check(Buttons buttonA, Buttons buttonB)    const { return Check(buttonA) || Check(buttonB); }
		inline bool Pressed(Buttons buttonA, Buttons buttonB)  const { return Pressed(buttonA) || Pressed(buttonB); }
		inline bool Released(Buttons buttonA, Buttons buttonB) const { return Released(buttonA) || Released(buttonB); }
		inline bool Check(Buttons buttonA, Buttons buttonB, Buttons buttonC)    const { return Check(buttonA) || Check(buttonB) || Check(buttonC); }
		inline bool Pressed(Buttons buttonA, Buttons buttonB, Buttons buttonC)  const { return Pressed(buttonA) || Pressed(buttonB) || Pressed(buttonC); }
		inline bool Released(Buttons buttonA, Buttons buttonB, Buttons buttonC) const { return Released(buttonA) || Released(buttonB) || Released(buttonC); }

		// Sticks
		inline const Math::Vec2 GetLeftStick() const { return m_currentState.m_thumbSticks.m_left; }
		inline const Math::Vec2 GetLeftStick(float deadZone) const
		{
			Math::Vec2 ret = m_currentState.m_thumbSticks.m_left;
			if (Math_Dot(ret, ret) < deadZone * deadZone) {
				ret = Vec2_Zero;
			}
			return ret;
		}

		inline const Math::Vec2 GetRightStick() const { return m_currentState.m_thumbSticks.m_right; }
		inline const Math::Vec2 GetRightStick(float deadZone) const
		{
			Math::Vec2 ret = m_currentState.m_thumbSticks.m_right;
			if (Math_Dot(ret, ret) < deadZone * deadZone) {
				ret = Vec2_Zero;
			}
			return ret;
		}

		// Triggers
		inline float GetLeftTrigger()  const { return m_currentState.m_triggers.m_left; }
		inline float GetRightTrigger() const { return m_currentState.m_triggers.m_right; }
		inline bool LeftTriggerCheck(float threshold)    const { return m_currentState.m_triggers.m_left >= threshold; }
		inline bool LeftTriggerPressed(float threshold)  const { return m_currentState.m_triggers.m_left >= threshold && m_previousState.m_triggers.m_left < threshold; }
		inline bool LeftTriggerReleased(float threshold) const { return m_currentState.m_triggers.m_left < threshold && m_previousState.m_triggers.m_left >= threshold; }
		inline bool RightTriggerCheck(float threshold)    const { return m_currentState.m_triggers.m_right >= threshold; }
		inline bool RightTriggerPressed(float threshold)  const { return m_currentState.m_triggers.m_right >= threshold && m_previousState.m_triggers.m_right < threshold; }
		inline bool RightTriggerReleased(float threshold) const { return m_currentState.m_triggers.m_right < threshold && m_previousState.m_triggers.m_right >= threshold; }

		// DPad
		inline float DPadHorizontal() const { return m_currentState.m_dpad.m_right == ButtonState::Pressed ? 1.f : (m_currentState.m_dpad.m_left == ButtonState::Pressed ? -1.f : 0.f); }
		inline float DPadVertical()   const { return m_currentState.m_dpad.m_up == ButtonState::Pressed ? 1.f : (m_currentState.m_dpad.m_down == ButtonState::Pressed ? -1.f : 0.f); }
		inline Math::Vec2 DPad() const { return Math::Vec2(DPadHorizontal(), DPadVertical()); }
		inline bool DPadLeftCheck()    const { return m_currentState.m_dpad.m_left == ButtonState::Pressed; }
		inline bool DPadLeftPressed()  const { return m_currentState.m_dpad.m_left == ButtonState::Pressed && m_previousState.m_dpad.m_left == ButtonState::Released; }
		inline bool DPadLeftReleased() const { return m_currentState.m_dpad.m_left == ButtonState::Released && m_previousState.m_dpad.m_left == ButtonState::Pressed; }
		inline bool DPadRightCheck()    const { return m_currentState.m_dpad.m_right == ButtonState::Pressed; }
		inline bool DPadRightPressed()  const { return m_currentState.m_dpad.m_right == ButtonState::Pressed && m_previousState.m_dpad.m_right == ButtonState::Released; }
		inline bool DPadRightReleased() const { return m_currentState.m_dpad.m_right == ButtonState::Released && m_previousState.m_dpad.m_right == ButtonState::Pressed; }
		inline bool DPadUpCheck()    const { return m_currentState.m_dpad.m_up == ButtonState::Pressed; }
		inline bool DPadUpPressed()  const { return m_currentState.m_dpad.m_up == ButtonState::Pressed && m_previousState.m_dpad.m_up == ButtonState::Released; }
		inline bool DPadUpReleased() const { return m_currentState.m_dpad.m_up == ButtonState::Released && m_previousState.m_dpad.m_up == ButtonState::Pressed; }
		inline bool DPadDownCheck()    const { return m_currentState.m_dpad.m_down == ButtonState::Pressed; }
		inline bool DPadDownPressed()  const { return m_currentState.m_dpad.m_down == ButtonState::Pressed && m_previousState.m_dpad.m_down == ButtonState::Released; }
		inline bool DPadDownReleased() const { return m_currentState.m_dpad.m_down == ButtonState::Released && m_previousState.m_dpad.m_down == ButtonState::Pressed; }

	private:
		inline GamePadData(PlayerIndex playerIndex)
		: m_playerIndex(playerIndex)
		{}

		PlayerIndex m_playerIndex;
		GamePadState m_currentState;
		GamePadState m_previousState;
		bool m_attached = false;

		float m_rumbleStrength = 0.f;
		float m_rumbleTime = 0.f;
	};

	static GamePadData* GamePads(int index)
	{
		CHERRYSODA_ASSERT(index < 4, "GamePad index out of range!\n");
		return ms_gamePads[index];
	}

	static void Initialize();

private:
	static void Shutdown();
	static void Update();
	static void UpdateNull();

	static const GamePadState GetGamePadState(int index);
	static bool SetGamePadVibration(int index, float leftMotor, float rightMotor);

	static GamePadData* ms_gamePads[4];
	static void* ms_internalDevices[4];
};

} // namespace cherrysoda

#endif // _CHERRYSODA_INPUT_MINPUT_H_
