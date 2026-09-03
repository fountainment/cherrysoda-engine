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

	StateMachine(int maxStates = 32) : base(true, false)
	{
		STL::Resize(m_begins, maxStates);
		STL::Resize(m_updates, maxStates);
		STL::Resize(m_ends, maxStates);
		STL::Resize(m_coroutines, maxStates);
	}

	void Added(Entity* entity) override;
	void EntityAdded(Scene* scene) override;

	inline operator int() const { return m_state; }

	inline int State() const { return m_state; }
	void State(int state);

	// Re-enters the state even when it is already the current one
	void ForceState(int state);

	// The coroutine parameter is a factory returning a fresh routine each time
	// the state is entered (see Coroutine.h for the combinators)
	void SetCallbacks(int state, STL::Func<int> onUpdate, STL::Func<STL::Func<bool>> coroutine = nullptr,
					  STL::Action<> begin = nullptr, STL::Action<> end = nullptr);

	inline int PreviousState() const { return m_previousState; }
	CHERRYSODA_GETTER_SETTER_OF_BOOL(ChangedStates, m_changedStates);
	CHERRYSODA_GETTER_SETTER_OF_BOOL(Locked, m_locked);
	CHERRYSODA_GETTER_SETTER_OF_BOOL(Log, m_log);

	void Update() override;

	void LogAllStates();
	void LogState(int index);

private:
	void EnterState(int state);

	int m_state = -1;
	int m_previousState = -1;

	bool m_changedStates = false;
	bool m_log = false;
	bool m_locked = false;

	STL::Vector<STL::Action<>> m_begins;
	STL::Vector<STL::Func<int>> m_updates;
	STL::Vector<STL::Action<>> m_ends;
	STL::Vector<STL::Func<STL::Func<bool>>> m_coroutines;

	STL::Func<bool> m_currentCoroutine = nullptr;
	bool m_coroutineReplaced = false;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_LOGIC_STATEMACHINE_H_
