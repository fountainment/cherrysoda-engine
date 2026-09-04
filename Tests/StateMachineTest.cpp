#include "Test.h"

#include <CherrySoda/Components/Logic/StateMachine.h>

namespace {

TEST(StateMachineTest, AutoStartsAtStateZeroWhenAddedToScene)
{
	Scene scene;
	Entity entity;
	StateMachine stateMachine(3);
	entity.Add(&stateMachine);

	// Adding the component to an entity without a scene does not start it yet
	EXPECT_EQ(-1, stateMachine.State());

	scene.Add(&entity);
	scene.Entities()->UpdateLists();

	// Entering the scene kicks the machine into its initial state
	EXPECT_EQ(0, stateMachine.State());
	EXPECT_EQ(0, static_cast<int>(stateMachine));
}

TEST(StateMachineTest, UpdateDrivesTransitionsWithCallbacks)
{
	Scene scene;
	Entity entity;
	StateMachine stateMachine(3);

	STL::Vector<String> events;
	stateMachine.SetCallbacks(
		0, [&] { return 1; }, nullptr, [&] { STL::Add(events, String("begin0")); },
		[&] { STL::Add(events, String("end0")); });
	stateMachine.SetCallbacks(
		1, [&] { return 1; }, nullptr, [&] { STL::Add(events, String("begin1")); },
		[&] { STL::Add(events, String("end1")); });
	stateMachine.SetCallbacks(
		2, nullptr, nullptr, [&] { STL::Add(events, String("begin2")); }, [&] { STL::Add(events, String("end2")); });

	entity.Add(&stateMachine);
	scene.Add(&entity);
	scene.Entities()->UpdateLists();

	// Entering the scene fired begin0
	ASSERT_EQ(1u, STL::Count(events));
	EXPECT_EQ(String("begin0"), events[0]);

	// Update asks the current state's callback for the next state
	stateMachine.Update();
	EXPECT_EQ(1, stateMachine.State());
	ASSERT_EQ(3u, STL::Count(events));
	EXPECT_EQ(String("end0"), events[1]);
	EXPECT_EQ(String("begin1"), events[2]);

	// Returning the current state does not re-enter it
	stateMachine.Update();
	EXPECT_EQ(1, stateMachine.State());
	EXPECT_EQ(3u, STL::Count(events));

	// Jumping states manually fires end then begin in order
	stateMachine.State(2);
	EXPECT_EQ(2, stateMachine.State());
	ASSERT_EQ(5u, STL::Count(events));
	EXPECT_EQ(String("end1"), events[3]);
	EXPECT_EQ(String("begin2"), events[4]);

	// State 2 has no update callback, so Update keeps it in place
	stateMachine.Update();
	EXPECT_EQ(2, stateMachine.State());
	EXPECT_EQ(5u, STL::Count(events));
}

TEST(StateMachineTest, SameStateDoesNotRefireBegin)
{
	StateMachine stateMachine(2);
	int beginCount = 0;
	stateMachine.SetCallbacks(1, nullptr, nullptr, [&] { ++beginCount; }, nullptr);

	stateMachine.State(1);
	EXPECT_EQ(1, beginCount);
	stateMachine.State(1);
	EXPECT_EQ(1, beginCount);

	// Out of range states assert in Debug builds, so only valid states here
	stateMachine.State(0);
	stateMachine.State(1);
	EXPECT_EQ(2, beginCount);
}

TEST(StateMachineTest, RetrievedFromEntityByType)
{
	Entity entity;
	StateMachine stateMachine(2);
	entity.Add(&stateMachine);

	EXPECT_EQ(&stateMachine, entity.Get<StateMachine>());

	// Getting by type goes through the component list
	Component* asComponent = entity.Get<StateMachine>();
	EXPECT_EQ(StateMachine::ComponentTypeID(), asComponent->TypeID());
	EXPECT_STREQ("StateMachine", asComponent->TypeCStr());
}

} // namespace
