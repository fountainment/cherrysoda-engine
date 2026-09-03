#ifndef _CHERRYSODA_INPUT_VIRTUALAXIS_H_
#define _CHERRYSODA_INPUT_VIRTUALAXIS_H_

#include <CherrySoda/Input/MInput.h>
#include <CherrySoda/Input/VirtualInput.h>
#include <CherrySoda/Util/Calc.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

// A virtual input represented as a float between -1 and 1. Port of Monocle's
// VirtualAxis.
class VirtualAxis : public VirtualInput
{
public:
	class Node : public VirtualInputNode
	{
	public:
		virtual float Value() const = 0;
	};

	VirtualAxis() = default;
	VirtualAxis(const STL::Vector<Node*>& nodes) { m_nodes = nodes; }
	~VirtualAxis() override;

	void Update() override;

	inline float Value() const { return m_value; }
	inline float PreviousValue() const { return m_previousValue; }

	inline STL::Vector<Node*>& Nodes() { return m_nodes; }

	class PadLeftStickX : public Node
	{
	public:
		PadLeftStickX(int gamepadIndex, float deadzone) : m_gamepadIndex(gamepadIndex), m_deadzone(deadzone) {}

		float Value() const override
		{
			return Calc::SignThreshold(MInput::GamePads(m_gamepadIndex)->GetLeftStick().x, m_deadzone);
		}

	private:
		int m_gamepadIndex;
		float m_deadzone;
	};

	class PadLeftStickY : public Node
	{
	public:
		PadLeftStickY(int gamepadIndex, float deadzone) : m_gamepadIndex(gamepadIndex), m_deadzone(deadzone) {}

		float Value() const override
		{
			return Calc::SignThreshold(MInput::GamePads(m_gamepadIndex)->GetLeftStick().y, m_deadzone);
		}

	private:
		int m_gamepadIndex;
		float m_deadzone;
	};

	class PadRightStickX : public Node
	{
	public:
		PadRightStickX(int gamepadIndex, float deadzone) : m_gamepadIndex(gamepadIndex), m_deadzone(deadzone) {}

		float Value() const override
		{
			return Calc::SignThreshold(MInput::GamePads(m_gamepadIndex)->GetRightStick().x, m_deadzone);
		}

	private:
		int m_gamepadIndex;
		float m_deadzone;
	};

	class PadRightStickY : public Node
	{
	public:
		PadRightStickY(int gamepadIndex, float deadzone) : m_gamepadIndex(gamepadIndex), m_deadzone(deadzone) {}

		float Value() const override
		{
			return Calc::SignThreshold(MInput::GamePads(m_gamepadIndex)->GetRightStick().y, m_deadzone);
		}

	private:
		int m_gamepadIndex;
		float m_deadzone;
	};

	class PadDpadLeftRight : public Node
	{
	public:
		PadDpadLeftRight(int gamepadIndex) : m_gamepadIndex(gamepadIndex) {}

		float Value() const override { return MInput::GamePads(m_gamepadIndex)->DPadHorizontal(); }

	private:
		int m_gamepadIndex;
	};

	class PadDpadUpDown : public Node
	{
	public:
		PadDpadUpDown(int gamepadIndex) : m_gamepadIndex(gamepadIndex) {}

		float Value() const override { return MInput::GamePads(m_gamepadIndex)->DPadVertical(); }

	private:
		int m_gamepadIndex;
	};

	class KeyboardKeys : public Node
	{
	public:
		KeyboardKeys(OverlapBehaviors overlapBehavior, Keys negative, Keys positive)
			: m_overlapBehavior(overlapBehavior), m_negative(negative), m_positive(positive)
		{
		}

		void Update() override;

		inline float Value() const override { return m_value; }

	private:
		OverlapBehaviors m_overlapBehavior;
		Keys m_negative;
		Keys m_positive;

		float m_value = 0.f;
		bool m_turned = false;
	};

private:
	STL::Vector<Node*> m_nodes;
	float m_value = 0.f;
	float m_previousValue = 0.f;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_INPUT_VIRTUALAXIS_H_
