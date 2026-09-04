#include <CherrySoda/Components/Logic/CoroutineHolder.h>

#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

int CoroutineHolder::StartCoroutine(STL::Func<bool> routine)
{
	int id = m_nextId++;
	STL::Add(m_coroutines, CoroutineData{.m_id = id, .m_routine = std::move(routine)});
	return id;
}

void CoroutineHolder::EndCoroutine(int id)
{
	for (auto it = m_coroutines.begin(); it != m_coroutines.end(); ++it) {
		if (it->m_id == id) {
			// While updating, defer the removal: erasing now would invalidate
			// the entry a routine is currently running from
			if (m_running) {
				STL::Add(m_toEnd, id);
			}
			else {
				m_coroutines.erase(it);
			}
			break;
		}
	}
}

void CoroutineHolder::Update()
{
	m_running = true;
	for (size_t i = 0; i < STL::Count(m_coroutines); ++i) {
		if (m_coroutines[i].m_routine == nullptr) {
			continue;
		}
		// Move the routine out so routines that start or end coroutines while
		// running never invalidate the entry they are called from; clear the
		// entry explicitly because a moved-from std::function may still hold
		// its target
		STL::Func<bool> current = std::move(m_coroutines[i].m_routine);
		m_coroutines[i].m_routine = nullptr;
		bool finished = current();
		if (finished) {
			STL::Add(m_toEnd, m_coroutines[i].m_id);
		}
		else {
			m_coroutines[i].m_routine = std::move(current);
		}
	}
	m_running = false;

	if (STL::IsNotEmpty(m_toEnd)) {
		for (int id : m_toEnd) {
			EndCoroutine(id);
		}
		STL::Clear(m_toEnd);
	}
}

} // namespace cherrysoda
