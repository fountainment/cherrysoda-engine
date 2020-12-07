#include <CherrySoda/Components/Logic/StateMachine.h>

#include <CherrySoda/Entity.h>
#include <CherrySoda/Scene.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/STL.h>

using cherrysoda::StateMachine;
using cherrysoda::STL;
using cherrysoda::Entity;
using cherrysoda::Scene;

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
	CHERRYSODA_ASSERT(state >= 0 && state < static_cast<int>(STL::Count(m_updates)), "StateMachine state out of range\n");

	if (!m_locked && m_state != state) {
		m_changedStates = true;
		m_previousState = m_state;
		m_state = state;

		if (m_previousState != -1 && m_ends[m_previousState] != nullptr) {
			// TODO: Log
			m_ends[m_previousState]();
		}
		if (m_begins[state] != nullptr) {
			// TODO: Log
			m_begins[state]();
		}
		// TODO: Add coroutine
	}
}

void StateMachine::SetCallbacks(int state, STL::Func<int> onUpdate, STL::Func<void> coroutine/* = nullptr*/, STL::Action<> begin/* = nullptr*/, STL::Action<> end/* = nullptr*/)
{
	m_updates[state] = onUpdate;
	m_begins[state] = begin;
	m_ends[state] = end;	
	// TODO: Add coroutine
}

void StateMachine::Update()
{
	m_changedStates = false;	

	if (m_updates[m_state] != nullptr) {
		// TODO: Log
		State(m_updates[m_state]());
	}
	// TODO: Add coroutine
}
