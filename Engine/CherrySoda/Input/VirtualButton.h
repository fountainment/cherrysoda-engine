#ifndef _CHERRYSODA_INPUT_VIRTUALBUTTON_H_
#define _CHERRYSODA_INPUT_VIRTUALBUTTON_H_

#include <CherrySoda/Input/MInput.h>
#include <CherrySoda/Input/VirtualAxis.h>
#include <CherrySoda/Input/VirtualInput.h>
#include <CherrySoda/Input/VirtualIntegerAxis.h>
#include <CherrySoda/Input/VirtualJoystick.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class VirtualButton;

// A virtual input represented as a boolean. As well as simply checking the
// current state, you can ask whether it was just pressed or released this
// frame, keep presses in a time buffer, and set up key repeat. Port of
// Monocle's VirtualButton.
class VirtualButton : public VirtualInput
{
public:
	class Node : public VirtualInputNode
	{
	public:
		virtual bool Check() const = 0;
		virtual bool Pressed() const = 0;
		virtual bool Released() const = 0;
	};

	VirtualButton() = default;
	VirtualButton(float bufferTime) { m_bufferTime = bufferTime; }
	VirtualButton(const STL::Vector<Node*>& nodes) { m_nodes = nodes; }
	VirtualButton(float bufferTime, const STL::Vector<Node*>& nodes)
	{
		m_bufferTime = bufferTime;
		m_nodes = nodes;
	}
	~VirtualButton() override;

	void Update() override;

	bool Check() const;
	bool Pressed() const;
	bool Released() const;

	void SetRepeat(float repeatTime);
	void SetRepeat(float firstRepeatTime, float multiRepeatTime);

	// Ends the press buffer for this button
	void ConsumeBuffer();

	// The button will not register a press for the rest of the current frame,
	// but otherwise keeps working normally; also ends the press buffer
	void ConsumePress();

	inline STL::Vector<Node*>& Nodes() { return m_nodes; }

	CHERRYSODA_GETTER_SETTER_OF_TYPE(float, BufferTime, m_bufferTime);
	inline bool Repeating() const { return m_repeating; }

	class KeyboardKey : public Node
	{
	public:
		KeyboardKey(Keys key) : m_key(key) {}

		bool Check() const override { return MInput::Keyboard()->Check(m_key); }
		bool Pressed() const override { return MInput::Keyboard()->Pressed(m_key); }
		bool Released() const override { return MInput::Keyboard()->Released(m_key); }

	private:
		Keys m_key;
	};

	class PadButton : public Node
	{
	public:
		PadButton(int gamepadIndex, Buttons button) : m_gamepadIndex(gamepadIndex), m_button(button) {}

		bool Check() const override { return MInput::GamePads(m_gamepadIndex)->Check(m_button); }
		bool Pressed() const override { return MInput::GamePads(m_gamepadIndex)->Pressed(m_button); }
		bool Released() const override { return MInput::GamePads(m_gamepadIndex)->Released(m_button); }

	private:
		int m_gamepadIndex;
		Buttons m_button;
	};

	class PadLeftStickLeft : public Node
	{
	public:
		PadLeftStickLeft(int gamepadIndex, float deadzone) : m_gamepadIndex(gamepadIndex), m_threshold(deadzone) {}

		bool Check() const override { return MInput::GamePads(m_gamepadIndex)->LeftStickLeftCheck(m_threshold); }
		bool Pressed() const override { return MInput::GamePads(m_gamepadIndex)->LeftStickLeftPressed(m_threshold); }
		bool Released() const override { return MInput::GamePads(m_gamepadIndex)->LeftStickLeftReleased(m_threshold); }

	private:
		int m_gamepadIndex;
		float m_threshold;
	};

	class PadLeftStickRight : public Node
	{
	public:
		PadLeftStickRight(int gamepadIndex, float deadzone) : m_gamepadIndex(gamepadIndex), m_threshold(deadzone) {}

		bool Check() const override { return MInput::GamePads(m_gamepadIndex)->LeftStickRightCheck(m_threshold); }
		bool Pressed() const override { return MInput::GamePads(m_gamepadIndex)->LeftStickRightPressed(m_threshold); }
		bool Released() const override { return MInput::GamePads(m_gamepadIndex)->LeftStickRightReleased(m_threshold); }

	private:
		int m_gamepadIndex;
		float m_threshold;
	};

	class PadLeftStickUp : public Node
	{
	public:
		PadLeftStickUp(int gamepadIndex, float deadzone) : m_gamepadIndex(gamepadIndex), m_threshold(deadzone) {}

		bool Check() const override { return MInput::GamePads(m_gamepadIndex)->LeftStickUpCheck(m_threshold); }
		bool Pressed() const override { return MInput::GamePads(m_gamepadIndex)->LeftStickUpPressed(m_threshold); }
		bool Released() const override { return MInput::GamePads(m_gamepadIndex)->LeftStickUpReleased(m_threshold); }

	private:
		int m_gamepadIndex;
		float m_threshold;
	};

	class PadLeftStickDown : public Node
	{
	public:
		PadLeftStickDown(int gamepadIndex, float deadzone) : m_gamepadIndex(gamepadIndex), m_threshold(deadzone) {}

		bool Check() const override { return MInput::GamePads(m_gamepadIndex)->LeftStickDownCheck(m_threshold); }
		bool Pressed() const override { return MInput::GamePads(m_gamepadIndex)->LeftStickDownPressed(m_threshold); }
		bool Released() const override { return MInput::GamePads(m_gamepadIndex)->LeftStickDownReleased(m_threshold); }

	private:
		int m_gamepadIndex;
		float m_threshold;
	};

	class PadRightStickLeft : public Node
	{
	public:
		PadRightStickLeft(int gamepadIndex, float deadzone) : m_gamepadIndex(gamepadIndex), m_threshold(deadzone) {}

		bool Check() const override { return MInput::GamePads(m_gamepadIndex)->RightStickLeftCheck(m_threshold); }
		bool Pressed() const override { return MInput::GamePads(m_gamepadIndex)->RightStickLeftPressed(m_threshold); }
		bool Released() const override { return MInput::GamePads(m_gamepadIndex)->RightStickLeftReleased(m_threshold); }

	private:
		int m_gamepadIndex;
		float m_threshold;
	};

	class PadRightStickRight : public Node
	{
	public:
		PadRightStickRight(int gamepadIndex, float deadzone) : m_gamepadIndex(gamepadIndex), m_threshold(deadzone) {}

		bool Check() const override { return MInput::GamePads(m_gamepadIndex)->RightStickRightCheck(m_threshold); }
		bool Pressed() const override { return MInput::GamePads(m_gamepadIndex)->RightStickRightPressed(m_threshold); }
		bool Released() const override
		{
			return MInput::GamePads(m_gamepadIndex)->RightStickRightReleased(m_threshold);
		}

	private:
		int m_gamepadIndex;
		float m_threshold;
	};

	class PadRightStickUp : public Node
	{
	public:
		PadRightStickUp(int gamepadIndex, float deadzone) : m_gamepadIndex(gamepadIndex), m_threshold(deadzone) {}

		bool Check() const override { return MInput::GamePads(m_gamepadIndex)->RightStickUpCheck(m_threshold); }
		bool Pressed() const override { return MInput::GamePads(m_gamepadIndex)->RightStickUpPressed(m_threshold); }
		bool Released() const override { return MInput::GamePads(m_gamepadIndex)->RightStickUpReleased(m_threshold); }

	private:
		int m_gamepadIndex;
		float m_threshold;
	};

	class PadRightStickDown : public Node
	{
	public:
		PadRightStickDown(int gamepadIndex, float deadzone) : m_gamepadIndex(gamepadIndex), m_threshold(deadzone) {}

		bool Check() const override { return MInput::GamePads(m_gamepadIndex)->RightStickDownCheck(m_threshold); }
		bool Pressed() const override { return MInput::GamePads(m_gamepadIndex)->RightStickDownPressed(m_threshold); }
		bool Released() const override { return MInput::GamePads(m_gamepadIndex)->RightStickDownReleased(m_threshold); }

	private:
		int m_gamepadIndex;
		float m_threshold;
	};

	class PadLeftTrigger : public Node
	{
	public:
		PadLeftTrigger(int gamepadIndex, float threshold) : m_gamepadIndex(gamepadIndex), m_threshold(threshold) {}

		bool Check() const override { return MInput::GamePads(m_gamepadIndex)->LeftTriggerCheck(m_threshold); }
		bool Pressed() const override { return MInput::GamePads(m_gamepadIndex)->LeftTriggerPressed(m_threshold); }
		bool Released() const override { return MInput::GamePads(m_gamepadIndex)->LeftTriggerReleased(m_threshold); }

	private:
		int m_gamepadIndex;
		float m_threshold;
	};

	class PadRightTrigger : public Node
	{
	public:
		PadRightTrigger(int gamepadIndex, float threshold) : m_gamepadIndex(gamepadIndex), m_threshold(threshold) {}

		bool Check() const override { return MInput::GamePads(m_gamepadIndex)->RightTriggerCheck(m_threshold); }
		bool Pressed() const override { return MInput::GamePads(m_gamepadIndex)->RightTriggerPressed(m_threshold); }
		bool Released() const override { return MInput::GamePads(m_gamepadIndex)->RightTriggerReleased(m_threshold); }

	private:
		int m_gamepadIndex;
		float m_threshold;
	};

	class PadDPadLeft : public Node
	{
	public:
		PadDPadLeft(int gamepadIndex) : m_gamepadIndex(gamepadIndex) {}

		bool Check() const override { return MInput::GamePads(m_gamepadIndex)->DPadLeftCheck(); }
		bool Pressed() const override { return MInput::GamePads(m_gamepadIndex)->DPadLeftPressed(); }
		bool Released() const override { return MInput::GamePads(m_gamepadIndex)->DPadLeftReleased(); }

	private:
		int m_gamepadIndex;
	};

	class PadDPadRight : public Node
	{
	public:
		PadDPadRight(int gamepadIndex) : m_gamepadIndex(gamepadIndex) {}

		bool Check() const override { return MInput::GamePads(m_gamepadIndex)->DPadRightCheck(); }
		bool Pressed() const override { return MInput::GamePads(m_gamepadIndex)->DPadRightPressed(); }
		bool Released() const override { return MInput::GamePads(m_gamepadIndex)->DPadRightReleased(); }

	private:
		int m_gamepadIndex;
	};

	class PadDPadUp : public Node
	{
	public:
		PadDPadUp(int gamepadIndex) : m_gamepadIndex(gamepadIndex) {}

		bool Check() const override { return MInput::GamePads(m_gamepadIndex)->DPadUpCheck(); }
		bool Pressed() const override { return MInput::GamePads(m_gamepadIndex)->DPadUpPressed(); }
		bool Released() const override { return MInput::GamePads(m_gamepadIndex)->DPadUpReleased(); }

	private:
		int m_gamepadIndex;
	};

	class PadDPadDown : public Node
	{
	public:
		PadDPadDown(int gamepadIndex) : m_gamepadIndex(gamepadIndex) {}

		bool Check() const override { return MInput::GamePads(m_gamepadIndex)->DPadDownCheck(); }
		bool Pressed() const override { return MInput::GamePads(m_gamepadIndex)->DPadDownPressed(); }
		bool Released() const override { return MInput::GamePads(m_gamepadIndex)->DPadDownReleased(); }

	private:
		int m_gamepadIndex;
	};

	class MouseLeftButton : public Node
	{
	public:
		bool Check() const override { return MInput::Mouse()->CheckLeftButton(); }
		bool Pressed() const override { return MInput::Mouse()->PressedLeftButton(); }
		bool Released() const override { return MInput::Mouse()->RleasedLeftButton(); }
	};

	class MouseRightButton : public Node
	{
	public:
		bool Check() const override { return MInput::Mouse()->CheckRightButton(); }
		bool Pressed() const override { return MInput::Mouse()->PressedRightButton(); }
		bool Released() const override { return MInput::Mouse()->RleasedRightButton(); }
	};

	class MouseMiddleButton : public Node
	{
	public:
		bool Check() const override { return MInput::Mouse()->CheckMiddleButton(); }
		bool Pressed() const override { return MInput::Mouse()->PressedMiddleButton(); }
		bool Released() const override { return MInput::Mouse()->RleasedMiddleButton(); }
	};

	// Threshold triggers over other virtual inputs; the pointed-to virtual
	// input is not owned by the node
	class VirtualAxisTrigger : public Node
	{
	public:
		VirtualAxisTrigger(const VirtualAxis* axis, ThresholdModes mode, float threshold)
			: m_axis(axis), m_mode(mode), m_threshold(threshold)
		{
		}

		bool Check() const override;
		bool Pressed() const override;
		bool Released() const override;

	private:
		const VirtualAxis* m_axis;
		ThresholdModes m_mode;
		float m_threshold;
	};

	class VirtualIntegerAxisTrigger : public Node
	{
	public:
		VirtualIntegerAxisTrigger(const VirtualIntegerAxis* axis, ThresholdModes mode, int threshold)
			: m_axis(axis), m_mode(mode), m_threshold(threshold)
		{
		}

		bool Check() const override;
		bool Pressed() const override;
		bool Released() const override;

	private:
		const VirtualIntegerAxis* m_axis;
		ThresholdModes m_mode;
		int m_threshold;
	};

	class VirtualJoystickXTrigger : public Node
	{
	public:
		VirtualJoystickXTrigger(const VirtualJoystick* joystick, ThresholdModes mode, float threshold)
			: m_joystick(joystick), m_mode(mode), m_threshold(threshold)
		{
		}

		bool Check() const override;
		bool Pressed() const override;
		bool Released() const override;

	private:
		const VirtualJoystick* m_joystick;
		ThresholdModes m_mode;
		float m_threshold;
	};

	class VirtualJoystickYTrigger : public Node
	{
	public:
		VirtualJoystickYTrigger(const VirtualJoystick* joystick, ThresholdModes mode, float threshold)
			: m_joystick(joystick), m_mode(mode), m_threshold(threshold)
		{
		}

		bool Check() const override;
		bool Pressed() const override;
		bool Released() const override;

	private:
		const VirtualJoystick* m_joystick;
		ThresholdModes m_mode;
		float m_threshold;
	};

private:
	STL::Vector<Node*> m_nodes;
	float m_bufferTime = 0.f;
	float m_firstRepeatTime = 0.f;
	float m_multiRepeatTime = 0.f;
	float m_bufferCounter = 0.f;
	float m_repeatCounter = 0.f;
	bool m_canRepeat = false;
	bool m_repeating = false;
	bool m_consumed = false;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_INPUT_VIRTUALBUTTON_H_
