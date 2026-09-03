#include "Test.h"

#include <CherrySoda/Components/Logic/Coroutine.h>
#include <CherrySoda/Components/Logic/CoroutineHolder.h>
#include <CherrySoda/Components/Logic/StateMachine.h>
#include <CherrySoda/Entity.h>
#include <CherrySoda/Scene.h>

namespace {

TEST(CoroutineCombinatorsTest, WaitFrames)
{
	auto routine = Coroutines::WaitFrames(3);
	EXPECT_FALSE(routine());
	EXPECT_FALSE(routine());
	EXPECT_TRUE(routine());
}

TEST(CoroutineCombinatorsTest, WaitUntil)
{
	bool condition = false;
	auto routine = Coroutines::WaitUntil([&condition]() { return condition; });
	EXPECT_FALSE(routine());
	EXPECT_FALSE(routine());
	condition = true;
	EXPECT_TRUE(routine());
}

TEST(CoroutineCombinatorsTest, SequenceRunsStepsInOrderAndSkipsGaps)
{
	STL::Vector<String> events;
	auto routine = Coroutines::Sequence(STL::Vector<STL::Func<bool>>{
		[&events]() {
			STL::Add(events, String("a"));
			return true;
		},
		nullptr,
		Coroutines::WaitFrames(2),
		[&events]() {
			STL::Add(events, String("b"));
			return true;
		},
	});

	// Frame one runs "a" and starts the wait
	EXPECT_FALSE(routine());
	ASSERT_EQ(1u, STL::Count(events));
	EXPECT_EQ(String("a"), events[0]);

	// When the wait ends, "b" runs in the same frame and the sequence finishes
	EXPECT_TRUE(routine());
	ASSERT_EQ(2u, STL::Count(events));
	EXPECT_EQ(String("b"), events[1]);
}

TEST(CoroutineCombinatorsTest, ParallelFinishesWhenAllStepsHave)
{
	int aFrames = 0;
	int bFrames = 0;
	auto routine = Coroutines::Parallel(STL::Vector<STL::Func<bool>>{
		[&aFrames]() {
			++aFrames;
			return aFrames >= 2;
		},
		[&bFrames]() {
			++bFrames;
			return bFrames >= 4;
		},
	});

	EXPECT_FALSE(routine());
	EXPECT_FALSE(routine());
	EXPECT_FALSE(routine());
	EXPECT_TRUE(routine());

	// Finished steps are not stepped again
	EXPECT_EQ(2, aFrames);
	EXPECT_EQ(4, bFrames);
}

TEST(CoroutineCombinatorsTest, RepeatRunsBodyCountTimes)
{
	int started = 0;
	auto routine = Coroutines::Repeat(
		[&started]() {
			++started;
			return Coroutines::WaitFrames(1);
		},
		3);

	int calls = 0;
	while (!routine()) {
		++calls;
	}
	++calls;

	EXPECT_EQ(3, calls);
	EXPECT_EQ(3, started);
}

TEST(CoroutineCombinatorsTest, EachFrameNeverFinishes)
{
	int fired = 0;
	auto routine = Coroutines::EachFrame([&fired]() { ++fired; });
	EXPECT_FALSE(routine());
	EXPECT_FALSE(routine());
	EXPECT_FALSE(routine());
	EXPECT_EQ(3, fired);
}

TEST(CoroutineTest, ComponentRunsUntilFinished)
{
	Entity entity;

	int ranCount = 0;
	Coroutine* coroutine = Coroutine::Create(
		[&ranCount]() {
			++ranCount;
			return ranCount >= 3;
		},
		false);
	entity.Add(coroutine);

	entity.Update();
	entity.Update();
	EXPECT_FALSE(coroutine->Finished());
	entity.Update();
	EXPECT_TRUE(coroutine->Finished());
	EXPECT_FALSE(coroutine->Active());
	EXPECT_EQ(3, ranCount);

	// Without RemoveOnComplete the component stays on the entity
	EXPECT_EQ(coroutine, entity.Get<Coroutine>());
}

TEST(CoroutineTest, ComponentRemovesItselfOnComplete)
{
	Scene scene;
	Entity entity;
	scene.Add(&entity);
	scene.Entities()->UpdateLists();

	Coroutine* coroutine = Coroutine::Create([]() { return true; });
	entity.Add(coroutine);

	EXPECT_EQ(coroutine, entity.Get<Coroutine>());
	entity.Update();
	// The removal was deferred while the component list was locked and has
	// been flushed by the time the update returns
	EXPECT_EQ(nullptr, entity.Get<Coroutine>());
}

TEST(CoroutineTest, ReplaceRestartsAndCancelEnds)
{
	Entity entity;

	int firstCount = 0;
	int secondCount = 0;
	Coroutine* coroutine = Coroutine::Create(
		[&firstCount]() {
			++firstCount;
			return false;
		},
		false);
	entity.Add(coroutine);

	coroutine->Update();
	EXPECT_EQ(1, firstCount);

	coroutine->Replace([&secondCount]() {
		++secondCount;
		return false;
	});
	coroutine->Update();
	coroutine->Update();
	EXPECT_EQ(1, firstCount);
	EXPECT_EQ(2, secondCount);
	EXPECT_FALSE(coroutine->Finished());

	coroutine->Cancel();
	EXPECT_TRUE(coroutine->Finished());
	coroutine->Update();
	EXPECT_EQ(2, secondCount);
}

TEST(CoroutineHolderTest, RunsMultipleRoutinesAndEndsById)
{
	Entity entity;
	CoroutineHolder holder;
	entity.Add(&holder);

	int aCount = 0;
	int bCount = 0;
	int idA = holder.StartCoroutine([&aCount]() {
		++aCount;
		return aCount >= 2;
	});
	holder.StartCoroutine([&bCount]() {
		++bCount;
		return bCount >= 5;
	});

	holder.Update();
	holder.Update();
	// A finished after two frames and was reclaimed; B keeps running
	EXPECT_EQ(2, aCount);
	EXPECT_EQ(2, bCount);

	holder.EndCoroutine(idA); // already gone; a no-op

	holder.Update();
	holder.Update();
	holder.Update();
	EXPECT_EQ(5, bCount);
}

TEST(StateMachineCoroutineTest, StateCoroutineRunsAndRestartsOnEntry)
{
	Scene scene;
	Entity entity;
	StateMachine stateMachine(2);

	int started = 0;
	int stepped = 0;
	// Register the coroutine factory before the machine enters its first state
	stateMachine.SetCallbacks(
		0, []() { return 0; },
		[&started, &stepped]() {
			++started;
			return [&stepped]() {
				++stepped;
				return stepped >= 2;
			};
		});

	entity.Add(&stateMachine);
	scene.Add(&entity);
	scene.Entities()->UpdateLists();

	stateMachine.Update();
	EXPECT_EQ(1, started);
	EXPECT_EQ(1, stepped); // the fresh routine is stepped the frame it starts

	stateMachine.Update();
	EXPECT_EQ(1, started);
	EXPECT_EQ(2, stepped); // finished this frame

	stateMachine.Update();
	EXPECT_EQ(2, stepped); // nothing left to step

	// Re-entering the state restarts the coroutine; the fresh routine is
	// stepped on the next Update
	stateMachine.ForceState(0);
	EXPECT_EQ(2, started);
	EXPECT_EQ(2, stepped);
	stateMachine.Update();
	EXPECT_EQ(3, stepped);
}

} // namespace
