#include <CherrySoda/Input/VirtualButton.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Util/STL.h>
#include <algorithm>

namespace cherrysoda {

VirtualButton::~VirtualButton()
{
	for (auto* node : m_nodes) {
		delete node;
	}
}

void VirtualButton::Update()
{
	m_consumed = false;
	m_bufferCounter -= Engine::Instance()->DeltaTime();

	bool check = false;
	for (auto* node : m_nodes) {
		node->Update();
		if (node->Pressed()) {
			m_bufferCounter = m_bufferTime;
			check = true;
		}
		else if (node->Check()) {
			check = true;
		}
	}

	if (!check) {
		m_repeating = false;
		m_repeatCounter = 0;
		m_bufferCounter = 0;
	}
	else if (m_canRepeat) {
		m_repeating = false;
		if (m_repeatCounter == 0) {
			m_repeatCounter = m_firstRepeatTime;
		}
		else {
			m_repeatCounter -= Engine::Instance()->DeltaTime();
			if (m_repeatCounter <= 0) {
				m_repeating = true;
				m_repeatCounter = m_multiRepeatTime;
			}
		}
	}
}

bool VirtualButton::Check() const
{
	if (MInput::Disabled()) {
		return false;
	}

	return std::ranges::any_of(m_nodes, [&](const auto* node) { return node->Check(); });
}

bool VirtualButton::Pressed() const
{
	if (MInput::Disabled()) {
		return false;
	}

	if (m_consumed) {
		return false;
	}

	if (m_bufferCounter > 0 || m_repeating) {
		return true;
	}

	return std::ranges::any_of(m_nodes, [&](const auto* node) { return node->Pressed(); });
}

bool VirtualButton::Released() const
{
	if (MInput::Disabled()) {
		return false;
	}

	return std::ranges::any_of(m_nodes, [&](const auto* node) { return node->Released(); });
}

void VirtualButton::SetRepeat(float repeatTime)
{
	SetRepeat(repeatTime, repeatTime);
}

void VirtualButton::SetRepeat(float firstRepeatTime, float multiRepeatTime)
{
	m_firstRepeatTime = firstRepeatTime;
	m_multiRepeatTime = multiRepeatTime;
	m_canRepeat = m_firstRepeatTime > 0;
	if (!m_canRepeat) {
		m_repeating = false;
	}
}

void VirtualButton::ConsumeBuffer()
{
	m_bufferCounter = 0;
}

void VirtualButton::ConsumePress()
{
	m_bufferCounter = 0;
	m_consumed = true;
}

bool VirtualButton::VirtualAxisTrigger::Check() const
{
	switch (m_mode) {
	case ThresholdModes::LargerThan:
		return m_axis->Value() >= m_threshold;
	case ThresholdModes::LessThan:
		return m_axis->Value() <= m_threshold;
	case ThresholdModes::EqualTo:
	default:
		return m_axis->Value() == m_threshold;
	}
}

bool VirtualButton::VirtualAxisTrigger::Pressed() const
{
	switch (m_mode) {
	case ThresholdModes::LargerThan:
		return m_axis->Value() >= m_threshold && m_axis->PreviousValue() < m_threshold;
	case ThresholdModes::LessThan:
		return m_axis->Value() <= m_threshold && m_axis->PreviousValue() > m_threshold;
	case ThresholdModes::EqualTo:
	default:
		return m_axis->Value() == m_threshold && m_axis->PreviousValue() != m_threshold;
	}
}

bool VirtualButton::VirtualAxisTrigger::Released() const
{
	switch (m_mode) {
	case ThresholdModes::LargerThan:
		return m_axis->Value() < m_threshold && m_axis->PreviousValue() >= m_threshold;
	case ThresholdModes::LessThan:
		return m_axis->Value() > m_threshold && m_axis->PreviousValue() <= m_threshold;
	case ThresholdModes::EqualTo:
	default:
		return m_axis->Value() != m_threshold && m_axis->PreviousValue() == m_threshold;
	}
}

bool VirtualButton::VirtualIntegerAxisTrigger::Check() const
{
	switch (m_mode) {
	case ThresholdModes::LargerThan:
		return m_axis->Value() >= m_threshold;
	case ThresholdModes::LessThan:
		return m_axis->Value() <= m_threshold;
	case ThresholdModes::EqualTo:
	default:
		return m_axis->Value() == m_threshold;
	}
}

bool VirtualButton::VirtualIntegerAxisTrigger::Pressed() const
{
	switch (m_mode) {
	case ThresholdModes::LargerThan:
		return m_axis->Value() >= m_threshold && m_axis->PreviousValue() < m_threshold;
	case ThresholdModes::LessThan:
		return m_axis->Value() <= m_threshold && m_axis->PreviousValue() > m_threshold;
	case ThresholdModes::EqualTo:
	default:
		return m_axis->Value() == m_threshold && m_axis->PreviousValue() != m_threshold;
	}
}

bool VirtualButton::VirtualIntegerAxisTrigger::Released() const
{
	switch (m_mode) {
	case ThresholdModes::LargerThan:
		return m_axis->Value() < m_threshold && m_axis->PreviousValue() >= m_threshold;
	case ThresholdModes::LessThan:
		return m_axis->Value() > m_threshold && m_axis->PreviousValue() <= m_threshold;
	case ThresholdModes::EqualTo:
	default:
		return m_axis->Value() != m_threshold && m_axis->PreviousValue() == m_threshold;
	}
}

bool VirtualButton::VirtualJoystickXTrigger::Check() const
{
	switch (m_mode) {
	case ThresholdModes::LargerThan:
		return m_joystick->Value().x >= m_threshold;
	case ThresholdModes::LessThan:
		return m_joystick->Value().x <= m_threshold;
	case ThresholdModes::EqualTo:
	default:
		return m_joystick->Value().x == m_threshold;
	}
}

bool VirtualButton::VirtualJoystickXTrigger::Pressed() const
{
	switch (m_mode) {
	case ThresholdModes::LargerThan:
		return m_joystick->Value().x >= m_threshold && m_joystick->PreviousValue().x < m_threshold;
	case ThresholdModes::LessThan:
		return m_joystick->Value().x <= m_threshold && m_joystick->PreviousValue().x > m_threshold;
	case ThresholdModes::EqualTo:
	default:
		return m_joystick->Value().x == m_threshold && m_joystick->PreviousValue().x != m_threshold;
	}
}

bool VirtualButton::VirtualJoystickXTrigger::Released() const
{
	switch (m_mode) {
	case ThresholdModes::LargerThan:
		return m_joystick->Value().x < m_threshold && m_joystick->PreviousValue().x >= m_threshold;
	case ThresholdModes::LessThan:
		return m_joystick->Value().x > m_threshold && m_joystick->PreviousValue().x <= m_threshold;
	case ThresholdModes::EqualTo:
	default:
		return m_joystick->Value().x != m_threshold && m_joystick->PreviousValue().x == m_threshold;
	}
}

bool VirtualButton::VirtualJoystickYTrigger::Check() const
{
	switch (m_mode) {
	case ThresholdModes::LargerThan:
		return m_joystick->Value().y >= m_threshold;
	case ThresholdModes::LessThan:
		return m_joystick->Value().y <= m_threshold;
	case ThresholdModes::EqualTo:
	default:
		return m_joystick->Value().y == m_threshold;
	}
}

bool VirtualButton::VirtualJoystickYTrigger::Pressed() const
{
	switch (m_mode) {
	case ThresholdModes::LargerThan:
		return m_joystick->Value().y >= m_threshold && m_joystick->PreviousValue().y < m_threshold;
	case ThresholdModes::LessThan:
		return m_joystick->Value().y <= m_threshold && m_joystick->PreviousValue().y > m_threshold;
	case ThresholdModes::EqualTo:
	default:
		return m_joystick->Value().y == m_threshold && m_joystick->PreviousValue().y != m_threshold;
	}
}

bool VirtualButton::VirtualJoystickYTrigger::Released() const
{
	switch (m_mode) {
	case ThresholdModes::LargerThan:
		return m_joystick->Value().y < m_threshold && m_joystick->PreviousValue().y >= m_threshold;
	case ThresholdModes::LessThan:
		return m_joystick->Value().y > m_threshold && m_joystick->PreviousValue().y <= m_threshold;
	case ThresholdModes::EqualTo:
	default:
		return m_joystick->Value().y != m_threshold && m_joystick->PreviousValue().y == m_threshold;
	}
}

} // namespace cherrysoda
