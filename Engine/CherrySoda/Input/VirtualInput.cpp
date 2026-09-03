#include <CherrySoda/Input/VirtualInput.h>

#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

STL::List<VirtualInput*>& VirtualInput::GetVirtualInputs()
{
	static STL::List<VirtualInput*> virtualInputs;
	return virtualInputs;
}

VirtualInput::VirtualInput()
{
	STL::Add(GetVirtualInputs(), this);
}

VirtualInput::~VirtualInput()
{
	Deregister();
}

void VirtualInput::Deregister()
{
	if (m_registered) {
		STL::Remove(GetVirtualInputs(), this);
		m_registered = false;
	}
}

void VirtualInput::UpdateVirtualInputs()
{
	for (auto input : GetVirtualInputs()) {
		input->Update();
	}
}

} // namespace cherrysoda
