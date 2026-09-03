#ifndef _CHERRYSODA_INPUT_VIRTUALINTEGERAXIS_H_
#define _CHERRYSODA_INPUT_VIRTUALINTEGERAXIS_H_

#include <CherrySoda/Input/VirtualAxis.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

// A virtual input represented as an integer of -1, 0 or 1; reuses the nodes
// of VirtualAxis. Port of Monocle's VirtualIntegerAxis.
class VirtualIntegerAxis : public VirtualInput
{
public:
	VirtualIntegerAxis() = default;
	VirtualIntegerAxis(const STL::Vector<VirtualAxis::Node*>& nodes) { m_nodes = nodes; }
	~VirtualIntegerAxis() override;

	void Update() override;

	inline int Value() const { return m_value; }
	inline int PreviousValue() const { return m_previousValue; }

	inline STL::Vector<VirtualAxis::Node*>& Nodes() { return m_nodes; }

private:
	STL::Vector<VirtualAxis::Node*> m_nodes;
	int m_value = 0;
	int m_previousValue = 0;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_INPUT_VIRTUALINTEGERAXIS_H_
