#include <CherrySoda/Input/VirtualJoystick.h>

#include <CherrySoda/Util/Calc.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

VirtualJoystick::~VirtualJoystick()
{
	for (auto node : m_nodes) {
		delete node;
	}
}

void VirtualJoystick::Update()
{
	for (auto node : m_nodes) {
		node->Update();
	}

	m_previousValue = m_value;
	m_value = Vec2_Zero;
	for (auto node : m_nodes) {
		Math::Vec2 value = node->Value();
		if (value != Vec2_Zero) {
			if (m_normalized) {
				if (m_snapSlices > 0.f) {
					value = Calc::SnapedNormal(value, m_snapSlices);
				}
				else {
					value = Math_Normalize(value);
				}
			}
			else if (m_snapSlices > 0.f) {
				value = Calc::Snapped(value, m_snapSlices);
			}

			m_value = value;
			break;
		}
	}
}

void VirtualJoystick::KeyboardKeys::Update()
{
	// X axis
	if (MInput::Keyboard()->Check(m_left)) {
		if (MInput::Keyboard()->Check(m_right)) {
			switch (m_overlapBehavior) {
			case OverlapBehaviors::CancelOut:
			default:
				m_value.x = 0.f;
				break;

			case OverlapBehaviors::TakeNewer:
				if (!m_turnedX) {
					m_value.x *= -1.f;
					m_turnedX = true;
				}
				break;

			case OverlapBehaviors::TakeOlder:
				// x stays the same
				break;
			}
		}
		else {
			m_turnedX = false;
			m_value.x = -1.f;
		}
	}
	else if (MInput::Keyboard()->Check(m_right)) {
		m_turnedX = false;
		m_value.x = 1.f;
	}
	else {
		m_turnedX = false;
		m_value.x = 0.f;
	}

	// Y axis
	if (MInput::Keyboard()->Check(m_up)) {
		if (MInput::Keyboard()->Check(m_down)) {
			switch (m_overlapBehavior) {
			case OverlapBehaviors::CancelOut:
			default:
				m_value.y = 0.f;
				break;

			case OverlapBehaviors::TakeNewer:
				if (!m_turnedY) {
					m_value.y *= -1.f;
					m_turnedY = true;
				}
				break;

			case OverlapBehaviors::TakeOlder:
				// y stays the same
				break;
			}
		}
		else {
			m_turnedY = false;
			m_value.y = -1.f;
		}
	}
	else if (MInput::Keyboard()->Check(m_down)) {
		m_turnedY = false;
		m_value.y = 1.f;
	}
	else {
		m_turnedY = false;
		m_value.y = 0.f;
	}
}

} // namespace cherrysoda
