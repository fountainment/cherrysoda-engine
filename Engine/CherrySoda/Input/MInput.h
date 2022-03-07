#ifndef _CHERRYSODA_INPUT_MINPUT_H_
#define _CHERRYSODA_INPUT_MINPUT_H_

#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>

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
	None = 0x00000000,
	DPadUp = 0x00000001,
	DPadDown = 0x00000002,
	DPadLeft = 0x00000004,
	DPadRight = 0x00000008,
	Start = 0x00000010,
	Back = 0x00000020,
	LeftStick = 0x00000040,
	RightStick = 0x00000080,
	LeftShoulder = 0x00000100,
	RightShoulder = 0x00000200,
	BigButton = 0x00000800,
	A = 0x00001000,
	B = 0x00002000,
	X = 0x00004000,
	Y = 0x00008000,
	LeftThumbstickLeft = 0x00200000,
	RightTrigger = 0x00400000,
	LeftTrigger = 0x00800000,
	RightThumbstickUp = 0x01000000,
	RightThumbstickDown = 0x02000000,
	RightThumbstickRight = 0x04000000,
	RightThumbstickLeft = 0x08000000,
	LeftThumbstickUp = 0x10000000,
	LeftThumbstickDown = 0x20000000,
	LeftThumbstickRight = 0x40000000,

	// Extensions
	Misc1EXT = 0x00000400,
	Paddle1EXT = 0x00010000,
	Paddle2EXT = 0x00020000,
	Paddle3EXT = 0x00040000,
	Paddle4EXT = 0x00080000,
	TouchPadEXT = 0x00100000

	// Unused bitflags: 0x80000000
};

CHERRYSODA_DECLARE_ENUM_FLAG(Buttons);

enum class KeyState : type::UInt8
{
	Up,
	Down
};

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
	LeftSuper = 91,
	RightSuper = 92,
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

class MInput
{
public:
	friend class Engine;
	friend class GUI;
	friend class Window;

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

		ButtonState m_up = ButtonState::Released;
		ButtonState m_down = ButtonState::Released;
		ButtonState m_left = ButtonState::Released;
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

		bool Attached() const { return m_attached; }

		// Buttons
		inline bool Check(Buttons button) const { return m_currentState.IsButtonDown(button); }
		inline bool Pressed(Buttons button) const { return m_currentState.IsButtonDown(button) && m_previousState.IsButtonUp(button); }
		inline bool Released(Buttons button) const { return m_currentState.IsButtonUp(button) && m_previousState.IsButtonDown(button); }

		inline bool Check(Buttons buttonA, Buttons buttonB) const { return Check(buttonA) || Check(buttonB); }
		inline bool Pressed(Buttons buttonA, Buttons buttonB) const { return Pressed(buttonA) || Pressed(buttonB); }
		inline bool Released(Buttons buttonA, Buttons buttonB) const { return Released(buttonA) || Released(buttonB); }

		inline bool Check(Buttons buttonA, Buttons buttonB, Buttons buttonC) const { return Check(buttonA) || Check(buttonB) || Check(buttonC); }
		inline bool Pressed(Buttons buttonA, Buttons buttonB, Buttons buttonC) const { return Pressed(buttonA) || Pressed(buttonB) || Pressed(buttonC); }
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

		// Left Stick Directions
		inline bool LeftStickLeftCheck(float deadzone) const { return m_currentState.m_thumbSticks.m_left.x <= -deadzone; }
		inline bool LeftStickLeftPressed(float deadzone) const { return m_currentState.m_thumbSticks.m_left.x <= -deadzone && m_previousState.m_thumbSticks.m_left.x > -deadzone; }
		inline bool LeftStickLeftReleased(float deadzone) const { return m_currentState.m_thumbSticks.m_left.x > -deadzone && m_previousState.m_thumbSticks.m_left.x <= -deadzone; }
		inline bool LeftStickRightCheck(float deadzone) const { return m_currentState.m_thumbSticks.m_left.x >= deadzone; }
		inline bool LeftStickRightPressed(float deadzone) const { return m_currentState.m_thumbSticks.m_left.x >= deadzone && m_previousState.m_thumbSticks.m_left.x < deadzone; }
		inline bool LeftStickRightReleased(float deadzone) const { return m_currentState.m_thumbSticks.m_left.x < deadzone && m_previousState.m_thumbSticks.m_left.x >= deadzone; }
		inline bool LeftStickDownCheck(float deadzone) const { return m_currentState.m_thumbSticks.m_left.y <= -deadzone; }
		inline bool LeftStickDownPressed(float deadzone) const { return m_currentState.m_thumbSticks.m_left.y <= -deadzone && m_previousState.m_thumbSticks.m_left.y > -deadzone; }
		inline bool LeftStickDownReleased(float deadzone) const { return m_currentState.m_thumbSticks.m_left.y > -deadzone && m_previousState.m_thumbSticks.m_left.y <= -deadzone; }
		inline bool LeftStickUpCheck(float deadzone) const { return m_currentState.m_thumbSticks.m_left.y >= deadzone; }
		inline bool LeftStickUpPressed(float deadzone) const { return m_currentState.m_thumbSticks.m_left.y >= deadzone && m_previousState.m_thumbSticks.m_left.y < deadzone; }
		inline bool LeftStickUpReleased(float deadzone) const { return m_currentState.m_thumbSticks.m_left.y < deadzone && m_previousState.m_thumbSticks.m_left.y >= deadzone; }
		inline float LeftStickHorizontal(float deadzone) const
		{
			float h = m_currentState.m_thumbSticks.m_left.x;
			return Math_Abs(h) < deadzone ? 0.f : h;
		}
		inline float LeftStickVertical(float deadzone) const
		{
			float v = m_currentState.m_thumbSticks.m_left.y;
			return Math_Abs(v) < deadzone ? 0.f : v;
		}

		// Right Stick Directions
		inline bool RightStickLeftCheck(float deadzone) const { return m_currentState.m_thumbSticks.m_right.x <= -deadzone; }
		inline bool RightStickLeftPressed(float deadzone) const { return m_currentState.m_thumbSticks.m_right.x <= -deadzone && m_previousState.m_thumbSticks.m_right.x > -deadzone; }
		inline bool RightStickLeftReleased(float deadzone) const { return m_currentState.m_thumbSticks.m_right.x > -deadzone && m_previousState.m_thumbSticks.m_right.x <= -deadzone; }
		inline bool RightStickRightCheck(float deadzone) const { return m_currentState.m_thumbSticks.m_right.x >= deadzone; }
		inline bool RightStickRightPressed(float deadzone) const { return m_currentState.m_thumbSticks.m_right.x >= deadzone && m_previousState.m_thumbSticks.m_right.x < deadzone; }
		inline bool RightStickRightReleased(float deadzone) const { return m_currentState.m_thumbSticks.m_right.x < deadzone && m_previousState.m_thumbSticks.m_right.x >= deadzone; }
		inline bool RightStickDownCheck(float deadzone) const { return m_currentState.m_thumbSticks.m_right.y <= -deadzone; }
		inline bool RightStickDownPressed(float deadzone) const { return m_currentState.m_thumbSticks.m_right.y <= -deadzone && m_previousState.m_thumbSticks.m_right.y > -deadzone; }
		inline bool RightStickDownReleased(float deadzone) const { return m_currentState.m_thumbSticks.m_right.y > -deadzone && m_previousState.m_thumbSticks.m_right.y <= -deadzone; }
		inline bool RightStickUpCheck(float deadzone) const { return m_currentState.m_thumbSticks.m_right.y >= deadzone; }
		inline bool RightStickUpPressed(float deadzone) const { return m_currentState.m_thumbSticks.m_right.y >= deadzone && m_previousState.m_thumbSticks.m_right.y < deadzone; }
		inline bool RightStickUpReleased(float deadzone) const { return m_currentState.m_thumbSticks.m_right.y < deadzone && m_previousState.m_thumbSticks.m_right.y >= deadzone; }
		inline float RightStickHorizontal(float deadzone) const
		{
			float h = m_currentState.m_thumbSticks.m_right.x;
			return Math_Abs(h) < deadzone ? 0.f : h;
		}
		inline float RightStickVertical(float deadzone) const
		{
			float v = m_currentState.m_thumbSticks.m_right.y;
			return Math_Abs(v) < deadzone ? 0.f : v;
		}

		// Triggers
		inline float GetLeftTrigger() const { return m_currentState.m_triggers.m_left; }
		inline float GetRightTrigger() const { return m_currentState.m_triggers.m_right; }

		inline bool LeftTriggerCheck(float threshold) const { return m_currentState.m_triggers.m_left >= threshold; }
		inline bool LeftTriggerPressed(float threshold) const { return m_currentState.m_triggers.m_left >= threshold && m_previousState.m_triggers.m_left < threshold; }
		inline bool LeftTriggerReleased(float threshold) const { return m_currentState.m_triggers.m_left < threshold && m_previousState.m_triggers.m_left >= threshold; }

		inline bool RightTriggerCheck(float threshold) const { return m_currentState.m_triggers.m_right >= threshold; }
		inline bool RightTriggerPressed(float threshold) const { return m_currentState.m_triggers.m_right >= threshold && m_previousState.m_triggers.m_right < threshold; }
		inline bool RightTriggerReleased(float threshold) const { return m_currentState.m_triggers.m_right < threshold && m_previousState.m_triggers.m_right >= threshold; }

		// DPad
		inline float DPadHorizontal() const { return m_currentState.m_dpad.m_right == ButtonState::Pressed ? 1.f : (m_currentState.m_dpad.m_left == ButtonState::Pressed ? -1.f : 0.f); }
		inline float DPadVertical() const { return m_currentState.m_dpad.m_up == ButtonState::Pressed ? 1.f : (m_currentState.m_dpad.m_down == ButtonState::Pressed ? -1.f : 0.f); }
		inline Math::Vec2 DPad() const { return Math::Vec2(DPadHorizontal(), DPadVertical()); }

		inline bool DPadLeftCheck() const { return m_currentState.m_dpad.m_left == ButtonState::Pressed; }
		inline bool DPadLeftPressed() const { return m_currentState.m_dpad.m_left == ButtonState::Pressed && m_previousState.m_dpad.m_left == ButtonState::Released; }
		inline bool DPadLeftReleased() const { return m_currentState.m_dpad.m_left == ButtonState::Released && m_previousState.m_dpad.m_left == ButtonState::Pressed; }

		inline bool DPadRightCheck() const { return m_currentState.m_dpad.m_right == ButtonState::Pressed; }
		inline bool DPadRightPressed() const { return m_currentState.m_dpad.m_right == ButtonState::Pressed && m_previousState.m_dpad.m_right == ButtonState::Released; }
		inline bool DPadRightReleased() const { return m_currentState.m_dpad.m_right == ButtonState::Released && m_previousState.m_dpad.m_right == ButtonState::Pressed; }

		inline bool DPadUpCheck() const { return m_currentState.m_dpad.m_up == ButtonState::Pressed; }
		inline bool DPadUpPressed() const { return m_currentState.m_dpad.m_up == ButtonState::Pressed && m_previousState.m_dpad.m_up == ButtonState::Released; }
		inline bool DPadUpReleased() const { return m_currentState.m_dpad.m_up == ButtonState::Released && m_previousState.m_dpad.m_up == ButtonState::Pressed; }

		inline bool DPadDownCheck() const { return m_currentState.m_dpad.m_down == ButtonState::Pressed; }
		inline bool DPadDownPressed() const { return m_currentState.m_dpad.m_down == ButtonState::Pressed && m_previousState.m_dpad.m_down == ButtonState::Released; }
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
	struct KeyboardState
	{
		KeyboardState() = default;
		KeyboardState(const STL::Vector<Keys>& keys)
		{
			for (Keys key : keys) InternalSetKey(key);
		}
		KeyboardState(const STL::List<Keys>& keys)
		{
			for (Keys key : keys) InternalSetKey(key);
		}

		inline bool IsKeyDown(Keys key) const { return InternalGetKey(key); }

		inline int GetHashCode() const { return (int)(keys0 ^ keys1 ^ keys2 ^ keys3 ^ keys4 ^ keys5 ^ keys6 ^ keys7); }

		bool InternalGetKey(Keys key) const;
		void InternalSetKey(Keys key);

		type::UInt32 keys0 = 0, keys1 = 0, keys2 = 0, keys3 = 0;
		type::UInt32 keys4 = 0, keys5 = 0, keys6 = 0, keys7 = 0;
	};

	class KeyboardData
	{
	public:
		friend class MInput;

		void Update()
		{
			m_previousState = m_currentState;
			m_currentState = GetKeyboardState();
		}

		void UpdateNull()
		{
			m_previousState = m_currentState;
			m_currentState = KeyboardState();
		}

		inline bool Check(Keys key) const { return m_currentState.IsKeyDown(key); }
		inline bool Pressed(Keys key) const { return m_currentState.IsKeyDown(key) && !m_previousState.IsKeyDown(key); }
		inline bool Released(Keys key) const { return !m_currentState.IsKeyDown(key) && m_previousState.IsKeyDown(key); }

		inline bool Check(Keys keyA, Keys keyB) const { return Check(keyA) || Check(keyB); }
		inline bool Pressed(Keys keyA, Keys keyB) const { return Pressed(keyA) || Pressed(keyB); }
		inline bool Released(Keys keyA, Keys keyB) const { return Released(keyA) || Released(keyB); }

		inline bool Check(Keys keyA, Keys keyB, Keys keyC) const { return Check(keyA) || Check(keyB) || Check(keyC); }
		inline bool Pressed(Keys keyA, Keys keyB, Keys keyC) const { return Pressed(keyA) || Pressed(keyB) || Pressed(keyC); }
		inline bool Released(Keys keyA, Keys keyB, Keys keyC) const { return Released(keyA) || Released(keyB) || Released(keyC); }

		inline bool Check(Keys keyA, Keys keyB, Keys keyC, Keys keyD) const { return Check(keyA) || Check(keyB) || Check(keyC) || Check(keyD); }
		inline bool Pressed(Keys keyA, Keys keyB, Keys keyC, Keys keyD) const { return Pressed(keyA) || Pressed(keyB) || Pressed(keyC) || Pressed(keyD); }
		inline bool Released(Keys keyA, Keys keyB, Keys keyC, Keys keyD) const { return Released(keyA) || Released(keyB) || Released(keyC) || Released(keyD); }

		int AxisCheck(Keys negative, Keys positive) const
		{
			if (Check(negative)) return Check(positive) ? 0 : -1;
			else return Check(positive) ? 1 : 0;
		}

		Math::IVec2 GetAxis(Keys left, Keys right, Keys down, Keys up) const
		{
			return Math::IVec2(AxisCheck(left, right), AxisCheck(down, up));
		}

		int AxisCheck(Keys negative, Keys positive, int both) const
		{
			if (Check(negative)) return Check(positive) ? both : -1;
			else return Check(positive) ? 1 : 0;
		}

		inline int GetHashCode() const { return m_currentState.GetHashCode(); }

	private:
		KeyboardData() = default;

		KeyboardState m_currentState;
		KeyboardState m_previousState;
	};

	static KeyboardData* Keyboard() { return ms_keyboard; }

	// Mouse
	struct MouseState
	{
		MouseState() = default;
		MouseState(int x, int y, int scrollWheel,
			ButtonState leftButton, ButtonState middleButton, ButtonState rightButton,
			ButtonState xButton1, ButtonState xButton2)
			: m_x(x)
			, m_y(y)
			, m_scrollWheelValue(scrollWheel)
			, m_leftButton(leftButton)
			, m_middleButton(middleButton)
			, m_rightButton(rightButton)
			, m_xButton1(xButton1)
			, m_xButton2(xButton2)
		{}

		int m_x = 0;
		int m_y = 0;
		int m_scrollWheelValue = 0;
		ButtonState m_leftButton = ButtonState::Released;
		ButtonState m_middleButton = ButtonState::Released;
		ButtonState m_rightButton = ButtonState::Released;
		ButtonState m_xButton1 = ButtonState::Released;
		ButtonState m_xButton2 = ButtonState::Released;
	};

	class MouseData
	{
	public:
		friend class MInput;

		void Update()
		{
			m_previousState = m_currentState;
			m_currentState = GetMouseState();
		}

		void UpdateNull()
		{
			m_previousState = m_currentState;
			m_currentState = MouseState();
		}

		bool CheckLeftButton() const { return m_currentState.m_leftButton == ButtonState::Pressed; }
		bool CheckRightButton() const { return m_currentState.m_rightButton == ButtonState::Pressed; }
		bool CheckMiddleButton() const { return m_currentState.m_middleButton == ButtonState::Pressed; }

		bool PressedLeftButton() const { return m_currentState.m_leftButton == ButtonState::Pressed && m_previousState.m_leftButton == ButtonState::Released; }
		bool PressedRightButton() const { return m_currentState.m_rightButton == ButtonState::Pressed && m_previousState.m_rightButton == ButtonState::Released; }
		bool PressedMiddleButton() const { return m_currentState.m_middleButton == ButtonState::Pressed && m_previousState.m_middleButton == ButtonState::Released; }

		bool RleasedLeftButton() const { return m_currentState.m_leftButton == ButtonState::Released && m_previousState.m_leftButton == ButtonState::Pressed; }
		bool RleasedRightButton() const { return m_currentState.m_rightButton == ButtonState::Released && m_previousState.m_rightButton == ButtonState::Pressed; }
		bool RleasedMiddleButton() const { return m_currentState.m_middleButton == ButtonState::Released && m_previousState.m_middleButton == ButtonState::Pressed; }

		int Wheel() const { return m_currentState.m_scrollWheelValue; }
		int WheelDelta() const { return m_currentState.m_scrollWheelValue - m_previousState.m_scrollWheelValue; }

		bool WasMoved() const { return m_currentState.m_x != m_previousState.m_x || m_currentState.m_y != m_previousState.m_y; }

		// const Math::IVec2 Position() const;
		// void Position(const Math::IVec2& pos);

		// int X() const { return Position().x; }
		// int Y() const { return Position().y; }

		inline const Math::IVec2 RawPosition() const { return Math::IVec2(m_currentState.m_x, m_currentState.m_y); }
		void RawPosition(const Math::IVec2& pos) { SetMousePosition(pos); }
		const Math::IVec2 Position() const;
		void Position(const Math::IVec2& pos);

		inline const Math::IVec2 RawPositionDelta() const { return Math::IVec2(m_currentState.m_x - m_previousState.m_x, m_currentState.m_y - m_previousState.m_y); }
		inline const Math::IVec2 PositionDelta() const { return Math::IVec2(m_currentState.m_x - m_previousState.m_x, m_previousState.m_y - m_currentState.m_y); }

		int RawX() const { return RawPosition().x; }
		int RawY() const { return RawPosition().y; }

	private:
		MouseData() = default;

		MouseState m_currentState;
		MouseState m_previousState;
	};

	static MouseData* Mouse() { return ms_mouse; }

	static void Initialize();
	static void Terminate();

private:
	static const STL::List<Keys>& GetCurrentKeyboardKeys() { return ms_keyboardKeys; } // Added for GUI, this won't be affected when Keyboard UpdateNull)

	static void Shutdown();
	static void Update();
	static void UpdateNull();

	static void SetKeyboardKeys(const STL::List<Keys>& keys) { ms_keyboardKeys = keys; }

	static const KeyboardState GetKeyboardState();
	static const MouseState GetMouseState();
	static const GamePadState GetGamePadState(int index);

	static bool GetRelativeMouseMode();
	static void SetMousePosition(const Math::IVec2& pos);
	static bool SetGamePadVibration(int index, float leftMotor, float rightMotor);

	static void AddControllerInstance(int dev);
	static void RemoveControllerInstance(int dev);

	static STL::List<Keys> ms_keyboardKeys;
	static KeyboardData* ms_keyboard;
	static MouseData* ms_mouse;
	static GamePadData* ms_gamePads[4];
	static void* ms_internalDevices[4];
	static STL::Map<int,int> ms_internalInstanceMap;

	static int ms_internalMouseWheel;
	static bool ms_supportsGlobalMouse;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_INPUT_MINPUT_H_
