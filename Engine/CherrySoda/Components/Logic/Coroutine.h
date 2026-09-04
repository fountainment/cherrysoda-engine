#ifndef _CHERRYSODA_COMPONENTS_LOGIC_COROUTINE_H_
#define _CHERRYSODA_COMPONENTS_LOGIC_COROUTINE_H_

#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Engine.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

// A coroutine routine is a step function invoked once per frame that returns
// true when the whole routine has finished. This is the C++ port of Monocle's
// IEnumerator coroutines; routines are built with the combinators in namespace
// Coroutines below instead of C# yield state machines.
class Coroutine : public Component
{
public:
	CHERRYSODA_DECLARE_COMPONENT(Coroutine, Component);

	static Coroutine* Create(STL::Func<bool> routine, bool removeOnComplete = true);

	inline bool Finished() const { return m_finished; }
	CHERRYSODA_GETTER_SETTER_OF_BOOL(RemoveOnComplete, m_removeOnComplete);

	void Update() override;
	void Removed(Entity* entity) override;

	void Replace(STL::Func<bool> routine);
	void Cancel();

private:
	Coroutine() : base(true, false) {}

	void Init(STL::Func<bool> routine, bool removeOnComplete);

	static STL::Stack<Coroutine*> ms_cached;

	STL::Func<bool> m_routine = nullptr;
	bool m_removeOnComplete = true;
	bool m_finished = false;
	bool m_ended = false;
};

// Routine combinators, the C++ counterparts of Monocle's `yield return ...`
namespace Coroutines {

// Wait for the given time in seconds
inline STL::Func<bool> Wait(float seconds, bool useRawDeltaTime = false)
{
	return [useRawDeltaTime, timeLeft = seconds]() mutable {
		timeLeft -= useRawDeltaTime ? Engine::Instance()->RawDeltaTime() : Engine::Instance()->DeltaTime();
		return timeLeft <= 0.f;
	};
}

// Wait a number of frames
inline STL::Func<bool> WaitFrames(int frames)
{
	return [framesLeft = frames]() mutable { return --framesLeft <= 0; };
}

// Wait until the condition returns true
inline STL::Func<bool> WaitUntil(const STL::Func<bool>& condition)
{
	return [condition] { return condition == nullptr || condition(); };
}

// Run the steps one after another; finishes when the last one has
inline STL::Func<bool> Sequence(STL::Vector<STL::Func<bool>> steps)
{
	return [steps = std::move(steps), index = static_cast<size_t>(0)]() mutable {
		while (index < steps.size()) {
			if (steps[index] != nullptr && !steps[index]()) {
				return false;
			}
			++index;
		}
		return true;
	};
}

// Run all steps concurrently; finishes when every step has
inline STL::Func<bool> Parallel(STL::Vector<STL::Func<bool>> steps)
{
	return [steps = std::move(steps)]() mutable {
		bool allFinished = true;
		for (auto& step : steps) {
			if (step != nullptr) {
				if (step()) {
					step = nullptr;
				}
				else {
					allFinished = false;
				}
			}
		}
		return allFinished;
	};
}

// Invoke createBody() count times in a row, each returning a fresh routine
// (count < 0 repeats forever)
inline STL::Func<bool> Repeat(STL::Func<STL::Func<bool>> createBody, int count = -1)
{
	return [createBody = std::move(createBody), count, body = STL::Func<bool>(), completed = 0]() mutable {
		if (count >= 0 && completed >= count) {
			return true;
		}
		if (body == nullptr) {
			body = createBody == nullptr ? nullptr : createBody();
			if (body == nullptr) {
				return true;
			}
		}
		if (body()) {
			++completed;
			body = nullptr;
		}
		return count >= 0 && completed >= count;
	};
}

// Run the action every frame; never finishes on its own
inline STL::Func<bool> EachFrame(const STL::Action<>& action)
{
	return [action] {
		if (action != nullptr) {
			action();
		}
		return false;
	};
}

} // namespace Coroutines

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_LOGIC_COROUTINE_H_
