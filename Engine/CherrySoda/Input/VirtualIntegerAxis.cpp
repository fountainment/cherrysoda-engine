#include <CherrySoda/Input/VirtualIntegerAxis.h>

#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

VirtualIntegerAxis::~VirtualIntegerAxis()
{
	for (auto* node : m_nodes) {
		delete node;
	}
}

void VirtualIntegerAxis::Update()
{
	for (auto* node : m_nodes) {
		node->Update();
	}

	m_previousValue = m_value;
	m_value = 0;
	for (auto* node : m_nodes) {
		float value = node->Value();
		if (value != 0) {
			m_value = value < 0 ? -1 : 1;
			break;
		}
	}
}

} // namespace cherrysoda
