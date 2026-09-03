#include "Test.h"

#include <CherrySoda/Input/VirtualAxis.h>
#include <CherrySoda/Input/VirtualButton.h>
#include <CherrySoda/Input/VirtualInput.h>
#include <CherrySoda/Input/VirtualIntegerAxis.h>
#include <CherrySoda/Input/VirtualJoystick.h>

namespace {

// A node whose state is driven by the test instead of real hardware
class FakeButtonNode : public VirtualButton::Node
{
public:
	FakeButtonNode() = default;

	bool Check() const override { return m_check; }
	bool Pressed() const override { return m_pressed; }
	bool Released() const override { return m_released; }

	bool m_check = false;
	bool m_pressed = false;
	bool m_released = false;
};

class FakeAxisNode : public VirtualAxis::Node
{
public:
	FakeAxisNode(float value = 0.f) : m_value(value) {}

	float Value() const override { return m_value; }

	float m_value;
};

class FakeJoystickNode : public VirtualJoystick::Node
{
public:
	FakeJoystickNode(const Math::Vec2& value = Vec2_Zero) : m_value(value) {}

	Math::Vec2 Value() const override { return m_value; }

	Math::Vec2 m_value;
};

TEST(VirtualInputTest, RegistersAndDeregisters)
{
	VirtualAxis axis;
	// Constructed inputs are registered for per-frame updates
	axis.Deregister();

	// Destruction also deregisters
	{
		VirtualButton button;
	}
}

TEST(VirtualButtonTest, QueriesNodes)
{
	FakeButtonNode* node = new FakeButtonNode();
	VirtualButton button({node});

	EXPECT_FALSE(button.Check());
	EXPECT_FALSE(button.Pressed());
	EXPECT_FALSE(button.Released());

	node->m_check = true;
	EXPECT_TRUE(button.Check());
	EXPECT_FALSE(button.Pressed());

	node->m_check = false;
	node->m_pressed = true;
	EXPECT_TRUE(button.Pressed());

	node->m_pressed = false;
	node->m_released = true;
	EXPECT_TRUE(button.Released());

	// Disabled shuts every query off
	MInput::Disabled(true);
	node->m_check = node->m_pressed = node->m_released = true;
	EXPECT_FALSE(button.Check());
	EXPECT_FALSE(button.Pressed());
	EXPECT_FALSE(button.Released());
	MInput::Disabled(false);
}

TEST(VirtualButtonTest, ConsumePressEndsPressedForTheFrame)
{
	FakeButtonNode* node = new FakeButtonNode();
	VirtualButton button({node});

	node->m_pressed = true;
	EXPECT_TRUE(button.Pressed());

	button.ConsumePress();
	EXPECT_FALSE(button.Pressed());
	// Check is unaffected
	EXPECT_FALSE(button.Check());

	// Consumed presses stay consumed until the next Update resets them
	node->m_check = true;
	node->m_pressed = true;
	EXPECT_TRUE(button.Check());
	EXPECT_FALSE(button.Pressed());
}

TEST(VirtualAxisTest, TakesFirstNonZeroNode)
{
	FakeAxisNode* idle = new FakeAxisNode(0.f);
	FakeAxisNode* active = new FakeAxisNode(-0.5f);
	VirtualAxis axis({idle, active});

	axis.Update();
	EXPECT_FLOAT_EQ(-0.5f, axis.Value());
	EXPECT_FLOAT_EQ(0.f, axis.PreviousValue());

	axis.Update();
	EXPECT_FLOAT_EQ(-0.5f, axis.PreviousValue());
	EXPECT_FLOAT_EQ(-0.5f, axis.Value());
}

TEST(VirtualIntegerAxisTest, SnapsNodeValueToSign)
{
	FakeAxisNode* node = new FakeAxisNode(0.25f);
	VirtualIntegerAxis axis({node});

	axis.Update();
	EXPECT_EQ(1, axis.Value());

	node->m_value = -0.25f;
	axis.Update();
	EXPECT_EQ(1, axis.PreviousValue());
	EXPECT_EQ(-1, axis.Value());
}

TEST(VirtualJoystickTest, FirstNonZeroNodeWins)
{
	FakeJoystickNode* idle = new FakeJoystickNode(Vec2_Zero);
	FakeJoystickNode* dpad = new FakeJoystickNode(Math::Vec2(-1.f, 1.f));
	VirtualJoystick joystick(false, {idle, dpad});

	joystick.Update();
	EXPECT_EQ(Math::Vec2(-1.f, 1.f), joystick.Value());
}

TEST(VirtualJoystickTest, NormalizesAndSnaps)
{
	FakeJoystickNode* node = new FakeJoystickNode(Math::Vec2(3.f, 4.f));

	VirtualJoystick normalized(true);
	normalized.Nodes().push_back(node);
	normalized.Update();
	const Math::Vec2 unit = normalized.Value();
	EXPECT_FLOAT_EQ(1.f, Math_Length(unit));

	VirtualJoystick snapped(true);
	snapped.Nodes().push_back(new FakeJoystickNode(Math::Vec2(0.866f, 0.5f)));
	snapped.SnapSlices(4.f); // quarter-circle slices
	snapped.Update();
	EXPECT_FLOAT_EQ(1.f, Math_Length(snapped.Value()));
	// Snapped onto the nearest of 4 directions: mostly-x rounds to pure x
	EXPECT_NEAR(0.f, Math_Abs(snapped.Value().y), 0.01f);
}

} // namespace
