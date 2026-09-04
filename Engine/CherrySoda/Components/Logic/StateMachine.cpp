#include <CherrySoda/Components/Logic/StateMachine.h>

#include <CherrySoda/Entity.h>
#include <CherrySoda/Scene.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

#include <utility>

namespace cherrysoda {

void StateMachine::Added(Entity* entity)
{
	base::Added(entity);

	if (entity->GetScene() != nullptr && m_state == -1) {
		State(0);
	}
}

void StateMachine::EntityAdded(Scene* scene)
{
	base::EntityAdded(scene);

	if (m_state == -1) {
		State(0);
	}
}

void StateMachine::State(int state)
{
	if (!m_locked && m_state != state) {
		EnterState(state);
	}
}

void StateMachine::ForceState(int state)
{
	EnterState(state);
}

void StateMachine::EnterState(int state)
{
	CHERRYSODA_ASSERT(state >= 0 && std::cmp_less(state, STL::Count(m_updates)), "StateMachine state out of range\n");

	if (m_log) {
		CHERRYSODA_DEBUG(CHERRYSODA_FORMAT("Enter State %d (leaving %d)\n", state, m_state));
	}

	m_changedStates = true;
	m_previousState = m_state;
	m_state = state;

	if (m_previousState != -1 && m_ends[m_previousState] != nullptr) {
		if (m_log) {
			CHERRYSODA_DEBUG(CHERRYSODA_FORMAT("Calling End %d\n", m_previousState));
		}
		m_ends[m_previousState]();
	}
	if (m_begins[state] != nullptr) {
		if (m_log) {
			CHERRYSODA_DEBUG(CHERRYSODA_FORMAT("Calling Begin %d\n", state));
		}
		m_begins[state]();
	}

	// Any state change replaces or cancels the running coroutine
	m_coroutineReplaced = true;
	if (m_coroutines[state] != nullptr) {
		if (m_log) {
			CHERRYSODA_DEBUG(CHERRYSODA_FORMAT("Starting Coroutine %d\n", state));
		}
		m_currentCoroutine = m_coroutines[state]();
	}
	else {
		m_currentCoroutine = nullptr;
	}
}

void StateMachine::SetCallbacks(int state, STL::Func<int> onUpdate, STL::Func<STL::Func<bool>> coroutine /* = nullptr*/,
								STL::Action<> begin /* = nullptr*/, STL::Action<> end /* = nullptr*/)
{
	m_updates[state] = std::move(onUpdate);
	m_begins[state] = std::move(begin);
	m_ends[state] = std::move(end);
	m_coroutines[state] = std::move(coroutine);
}

void StateMachine::Update()
{
	m_changedStates = false;

	if (m_updates[m_state] != nullptr) {
		State(m_updates[m_state]());
	}

	if (m_currentCoroutine != nullptr) {
		m_coroutineReplaced = false;
		// Move the routine out so a state change inside it never assigns over
		// a closure that is currently running; clear the member explicitly
		// because a moved-from std::function may still hold its target
		STL::Func<bool> current = std::move(m_currentCoroutine);
		m_currentCoroutine = nullptr;
		bool finished = current();
		if (!m_coroutineReplaced) {
			if (!finished) {
				m_currentCoroutine = std::move(current);
			}
			else if (m_log && !m_changedStates) {
				CHERRYSODA_DEBUG(CHERRYSODA_FORMAT("Finished Coroutine %d\n", m_state));
			}
		}
	}
}

void StateMachine::LogAllStates()
{
	for (int i = 0; std::cmp_less(i, STL::Count(m_updates)); i++) {
		LogState(i);
	}
}

void StateMachine::LogState(int index)
{
	CHERRYSODA_DEBUG(CHERRYSODA_FORMAT("State %d: %s%s%s%s\n", index, m_updates[index] != nullptr ? "U" : "",
									   m_begins[index] != nullptr ? "B" : "", m_ends[index] != nullptr ? "E" : "",
									   m_coroutines[index] != nullptr ? "C" : ""));
}

} // namespace cherrysoda
