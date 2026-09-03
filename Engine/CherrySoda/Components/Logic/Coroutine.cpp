#include <CherrySoda/Components/Logic/Coroutine.h>

#include <CherrySoda/Entity.h>
#include <CherrySoda/Util/Log.h>

namespace cherrysoda {

STL::Stack<Coroutine*> Coroutine::ms_cached;

Coroutine* Coroutine::Create(STL::Func<bool> routine, bool removeOnComplete /* = true*/)
{
	Coroutine* coroutine = nullptr;
	if (STL::IsEmpty(ms_cached)) {
		coroutine = new Coroutine();
	}
	else {
		coroutine = STL::Pop(ms_cached);
	}
	coroutine->Init(std::move(routine), removeOnComplete);
	return coroutine;
}

void Coroutine::Init(STL::Func<bool> routine, bool removeOnComplete)
{
	m_routine = std::move(routine);
	m_removeOnComplete = removeOnComplete;
	m_finished = false;
	m_ended = false;
	Active(true);
}

void Coroutine::Update()
{
	if (m_routine == nullptr) {
		return;
	}
	m_ended = false;
	// Move the routine out so that Cancel/Replace called from inside it never
	// assign over a closure that is currently running; clear the member
	// explicitly because a moved-from std::function may still hold its target
	STL::Func<bool> current = std::move(m_routine);
	m_routine = nullptr;
	bool finished = current();
	if (m_ended) {
		return;
	}
	if (!finished) {
		m_routine = std::move(current);
	}
	if (finished) {
		m_finished = true;
		Active(false);
		if (m_removeOnComplete) {
			RemoveSelf();
		}
	}
}

void Coroutine::Removed(Entity* entity)
{
	CancleAutoDelete();
	base::Removed(entity);
	STL::Push(ms_cached, this);
}

void Coroutine::Replace(STL::Func<bool> routine)
{
	Active(true);
	m_finished = false;
	m_routine = std::move(routine);
	m_ended = true;
}

void Coroutine::Cancel()
{
	Active(false);
	m_finished = true;
	m_routine = nullptr;
	m_ended = true;
}

} // namespace cherrysoda
