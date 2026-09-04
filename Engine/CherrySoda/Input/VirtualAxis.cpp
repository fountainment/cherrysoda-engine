#include <CherrySoda/Input/VirtualAxis.h>

#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

VirtualAxis::~VirtualAxis()
{
	for (auto* node : m_nodes) {
		delete node;
	}
}

void VirtualAxis::Update()
{
	for (auto* node : m_nodes) {
		node->Update();
	}

	m_previousValue = m_value;
	m_value = 0;
	for (auto* node : m_nodes) {
		float value = node->Value();
		if (value != 0) {
			m_value = value;
			break;
		}
	}
}

void VirtualAxis::KeyboardKeys::Update()
{
	if (MInput::Keyboard()->Check(m_positive)) {
		if (MInput::Keyboard()->Check(m_negative)) {
			switch (m_overlapBehavior) {
			case OverlapBehaviors::CancelOut:
			default:
				m_value = 0.f;
				break;

			case OverlapBehaviors::TakeNewer:
				if (!m_turned) {
					m_value *= -1.f;
					m_turned = true;
				}
				break;

			case OverlapBehaviors::TakeOlder:
				// m_value stays the same
				break;
			}
		}
		else {
			m_turned = false;
			m_value = 1.f;
		}
	}
	else if (MInput::Keyboard()->Check(m_negative)) {
		m_turned = false;
		m_value = -1.f;
	}
	else {
		m_turned = false;
		m_value = 0.f;
	}
}

} // namespace cherrysoda
