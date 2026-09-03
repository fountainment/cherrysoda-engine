#ifndef _CHERRYSODA_INPUT_VIRTUALINPUT_H_
#define _CHERRYSODA_INPUT_VIRTUALINPUT_H_

#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

enum class OverlapBehaviors
{
	CancelOut,
	TakeOlder,
	TakeNewer
};

enum class ThresholdModes
{
	LargerThan,
	LessThan,
	EqualTo
};

// Base of the virtual input layer (VirtualButton, VirtualAxis,
// VirtualIntegerAxis, VirtualJoystick). Instances register themselves and are
// stepped once per frame from MInput::Update. Port of Monocle's VirtualInput;
// nodes added to a virtual input are owned and deleted by it.
class VirtualInput
{
public:
	VirtualInput();
	virtual ~VirtualInput();

	VirtualInput(const VirtualInput&) = delete;
	VirtualInput& operator=(const VirtualInput&) = delete;

	virtual void Update() = 0;

	void Deregister();

	static void UpdateVirtualInputs();

private:
	bool m_registered = true;

	static STL::List<VirtualInput*>& GetVirtualInputs();
};

// A single source of input state queried by a virtual input
class VirtualInputNode
{
public:
	VirtualInputNode() = default;
	virtual ~VirtualInputNode() = default;

	VirtualInputNode(const VirtualInputNode&) = delete;
	VirtualInputNode& operator=(const VirtualInputNode&) = delete;

	virtual void Update() {}
};

} // namespace cherrysoda

#endif // _CHERRYSODA_INPUT_VIRTUALINPUT_H_
