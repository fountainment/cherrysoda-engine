#ifndef _CHERRYSODA_COMPONENTS_LOGIC_STATEMACHINE_H_
#define _CHERRYSODA_COMPONENTS_LOGIC_STATEMACHINE_H_

#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class Entity;
class Scene;

class StateMachine : public Component
{
public:
	CHERRYSODA_DECLARE_COMPONENT(StateMachine, Component);

	StateMachine(int maxStates = 32)
		: base(true, false)
	{
		STL::Resize(m_begins, maxStates);
		STL::Resize(m_updates, maxStates);
		STL::Resize(m_ends, maxStates);
	}

	void Added(Entity* entity) override;
	void EntityAdded(Scene* scene) override;

	inline operator int() const { return m_state; }

	inline int State() const { return m_state; }
	void State(int state);

	void SetCallbacks(int state, STL::Func<int> onUpdate, STL::Func<void> coroutine = nullptr, STL::Action<> begin = nullptr, STL::Action<> end = nullptr);

	void Update() override;

private:
	int m_state = -1;
	int m_previousState = -1;

	bool m_changedStates = false;
	// TODO: log state change in debug console
	// bool m_log = false;
	bool m_locked = false;

	STL::Vector<STL::Action<>> m_begins;
	STL::Vector<STL::Func<int>> m_updates;
	STL::Vector<STL::Action<>> m_ends;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_LOGIC_STATEMACHINE_H_
