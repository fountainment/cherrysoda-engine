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

	// Game Pad
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

	// Keyboard
	enum class Keys : type::UInt8
	{
		None = 0,
		Back = 8,
		Tab = 9,
		Enter = 13,
		CapsLock = 20,
		Escape = 27,
		Space = 32,
		PageUp = 33,
		PageDown = 34,
		End = 35,
		Home = 36,
		Left = 37,
		Up = 38,
		Right = 39,
		Down = 40,
		Select = 41,
		Print = 42,
		Execute = 43,
		PrintScreen = 44,
		Insert = 45,
		Delete = 46,
		Help = 47,
		D0 = 48,
		D1 = 49,
		D2 = 50,
		D3 = 51,
		D4 = 52,
		D5 = 53,
		D6 = 54,
		D7 = 55,
		D8 = 56,
		D9 = 57,
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,
		LeftWindows = 91,
		RightWindows = 92,
		Apps = 93,
		Sleep = 95,
		NumPad0 = 96,
		NumPad1 = 97,
		NumPad2 = 98,
		NumPad3 = 99,
		NumPad4 = 100,
		NumPad5 = 101,
		NumPad6 = 102,
		NumPad7 = 103,
		NumPad8 = 104,
		NumPad9 = 105,
		Multiply = 106,
		Add = 107,
		Separator = 108,
		Subtract = 109,
		Decimal = 110,
		Divide = 111,
		F1 = 112,
		F2 = 113,
		F3 = 114,
		F4 = 115,
		F5 = 116,
		F6 = 117,
		F7 = 118,
		F8 = 119,
		F9 = 120,
		F10 = 121,
		F11 = 122,
		F12 = 123,
		F13 = 124,
		F14 = 125,
		F15 = 126,
		F16 = 127,
		F17 = 128,
		F18 = 129,
		F19 = 130,
		F20 = 131,
		F21 = 132,
		F22 = 133,
		F23 = 134,
		F24 = 135,
		NumLock = 144,
		Scroll = 145,
		LeftShift = 160,
		RightShift = 161,
		LeftControl = 162,
		RightControl = 163,
		LeftAlt = 164,
		RightAlt = 165,
		BrowserBack = 166,
		BrowserForward = 167,
		BrowserRefresh = 168,
		BrowserStop = 169,
		BrowserSearch = 170,
		BrowserFavorites = 171,
		BrowserHome = 172,
		VolumeMute = 173,
		VolumeDown = 174,
		VolumeUp = 175,
		MediaNextTrack = 176,
		MediaPreviousTrack = 177,
		MediaStop = 178,
		MediaPlayPause = 179,
		LaunchMail = 180,
		SelectMedia = 181,
		LaunchApplication1 = 182,
		LaunchApplication2 = 183,
		OemSemicolon = 186,
		OemPlus = 187,
		OemComma = 188,
		OemMinus = 189,
		OemPeriod = 190,
		OemQuestion = 191,
		OemTilde = 192,
		OemOpenBrackets = 219,
		OemPipe = 220,
		OemCloseBrackets = 221,
		OemQuotes = 222,
		Oem8 = 223,
		OemBackslash = 226,
		ProcessKey = 229,
		Attn = 246,
		Crsel = 247,
		Exsel = 248,
		EraseEof = 249,
		Play = 250,
		Zoom = 251,
		Pa1 = 253,
		OemClear = 254,
		ChatPadGreen = 0xCA,
		ChatPadOrange = 0xCB,
		Pause = 0x13,
		ImeConvert = 0x1c,
		ImeNoConvert = 0x1d,
		Kana = 0x15,
		Kanji = 0x19,
		OemAuto = 0xf3,
		OemCopy = 0xf2,
		OemEnlW = 0xf4
	};

	enum class KeyState : type::UInt8
	{
		Up,
		Down
	};

	struct KeyboardState
	{

	};

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
