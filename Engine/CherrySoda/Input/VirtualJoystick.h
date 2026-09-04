#ifndef _CHERRYSODA_INPUT_VIRTUALJOYSTICK_H_
#define _CHERRYSODA_INPUT_VIRTUALJOYSTICK_H_

#include <CherrySoda/Input/MInput.h>
#include <CherrySoda/Input/VirtualInput.h>
#include <CherrySoda/Util/Calc.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

// A virtual input represented as a Vec2 with both components between -1 and
// 1. Port of Monocle's VirtualJoystick.
class VirtualJoystick : public VirtualInput
{
public:
	class Node : public VirtualInputNode
	{
	public:
		virtual Math::Vec2 Value() const = 0;
	};

	VirtualJoystick(bool normalized = false) { m_normalized = normalized; }
	VirtualJoystick(bool normalized, const STL::Vector<Node*>& nodes)
	{
		m_normalized = normalized;
		m_nodes = nodes;
	}
	~VirtualJoystick() override;

	void Update() override;

	inline Math::Vec2 Value() const { return m_value; }
	inline Math::Vec2 PreviousValue() const { return m_previousValue; }

	inline STL::Vector<Node*>& Nodes() { return m_nodes; }

	CHERRYSODA_GETTER_SETTER_OF_BOOL(Normalized, m_normalized);
	inline float SnapSlices() const { return m_snapSlices; }
	inline void SnapSlices(float slices) { m_snapSlices = slices; }
	inline void ClearSnapSlices() { m_snapSlices = 0.f; }

	class PadLeftStick : public Node
	{
	public:
		PadLeftStick(int gamepadIndex, float deadzone) : m_gamepadIndex(gamepadIndex), m_deadzone(deadzone) {}

		Math::Vec2 Value() const override { return MInput::GamePads(m_gamepadIndex)->GetLeftStick(m_deadzone); }

	private:
		int m_gamepadIndex;
		float m_deadzone;
	};

	class PadRightStick : public Node
	{
	public:
		PadRightStick(int gamepadIndex, float deadzone) : m_gamepadIndex(gamepadIndex), m_deadzone(deadzone) {}

		Math::Vec2 Value() const override { return MInput::GamePads(m_gamepadIndex)->GetRightStick(m_deadzone); }

	private:
		int m_gamepadIndex;
		float m_deadzone;
	};

	class PadDpad : public Node
	{
	public:
		PadDpad(int gamepadIndex) : m_gamepadIndex(gamepadIndex) {}

		Math::Vec2 Value() const override
		{
			auto value = Vec2_Zero;

			if (MInput::GamePads(m_gamepadIndex)->DPadRightCheck()) {
				value.x = 1.f;
			}
			else if (MInput::GamePads(m_gamepadIndex)->DPadLeftCheck()) {
				value.x = -1.f;
			}

			if (MInput::GamePads(m_gamepadIndex)->DPadDownCheck()) {
				value.y = 1.f;
			}
			else if (MInput::GamePads(m_gamepadIndex)->DPadUpCheck()) {
				value.y = -1.f;
			}

			return value;
		}

	private:
		int m_gamepadIndex;
	};

	class KeyboardKeys : public Node
	{
	public:
		KeyboardKeys(OverlapBehaviors overlapBehavior, Keys left, Keys right, Keys up, Keys down)
			: m_overlapBehavior(overlapBehavior), m_left(left), m_right(right), m_up(up), m_down(down)
		{
		}

		void Update() override;

		inline Math::Vec2 Value() const override { return m_value; }

	private:
		OverlapBehaviors m_overlapBehavior;
		Keys m_left;
		Keys m_right;
		Keys m_up;
		Keys m_down;

		bool m_turnedX = false;
		bool m_turnedY = false;
		Math::Vec2 m_value = Vec2_Zero;
	};

private:
	STL::Vector<Node*> m_nodes;
	bool m_normalized = false;
	float m_snapSlices = 0.f;
	Math::Vec2 m_value = Vec2_Zero;
	Math::Vec2 m_previousValue = Vec2_Zero;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_INPUT_VIRTUALJOYSTICK_H_
