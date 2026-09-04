#include "Test.h"

#include <CherrySoda/Colliders/Hitbox.h>
#include <CherrySoda/Components/CollidableComponent.h>
#include <CherrySoda/Scene.h>
#include <CherrySoda/Util/Tracker.h>

namespace {

class TrackedSolid : public Entity
{
public:
	TrackedSolid() = default;
	TrackedSolid(const Math::Vec2& position) : base(position) {}

	CHERRYSODA_DECLARE_ENTITY(TrackedSolid, Entity);
};
CHERRYSODA_TRACK_ENTITY(TrackedSolid);

class TrackedHazard : public CollidableComponent
{
public:
	TrackedHazard() : base(true, false, true) {}

	CHERRYSODA_DECLARE_COMPONENT(TrackedHazard, CollidableComponent);
};
CHERRYSODA_TRACK_COMPONENT(TrackedHazard);

Math::Rectangle MakeRect(float x, float y, float w, float h)
{
	return Math::Rectangle{.m_coord = Math::Vec2(x, y), .m_size = Math::Vec2(w, h)};
}

TEST(TrackerTest, TracksEntitiesThroughSceneLifecycle)
{
	Scene scene;
	EXPECT_EQ(nullptr, scene.GetTracker()->GetEntitiesOfType(TrackedSolid::EntityTypeID()));

	TrackedSolid solid;
	scene.Add(&solid);
	EXPECT_EQ(nullptr, scene.GetTracker()->GetEntitiesOfType(TrackedSolid::EntityTypeID()));

	scene.Entities()->UpdateLists();
	const auto* list = scene.GetTracker()->GetEntitiesOfType(TrackedSolid::EntityTypeID());
	ASSERT_NE(nullptr, list);
	EXPECT_EQ(1u, STL::Count(*list));
	EXPECT_EQ(&solid, STL::Front(*list));

	// Leaving the scene empties the tracked list (the entry itself stays)
	scene.Remove(&solid);
	scene.Entities()->UpdateLists();
	const auto* emptied = scene.GetTracker()->GetEntitiesOfType(TrackedSolid::EntityTypeID());
	ASSERT_NE(nullptr, emptied);
	EXPECT_EQ(0u, STL::Count(*emptied));
}

TEST(TrackerTest, TracksComponents)
{
	Scene scene;
	Entity entity;
	TrackedHazard hazard;
	Hitbox hazardBox(2.f, 2.f);
	hazard.SetCollider(&hazardBox);

	entity.Add(&hazard);
	scene.Add(&entity);
	scene.Entities()->UpdateLists();

	const auto* list = scene.GetTracker()->GetComponentsOfType(TrackedHazard::ComponentTypeID());
	ASSERT_NE(nullptr, list);
	EXPECT_EQ(1u, STL::Count(*list));
	EXPECT_EQ(&hazard, STL::Front(*list));

	// Removing the whole entity empties the tracked component list
	scene.Remove(&entity);
	scene.Entities()->UpdateLists();
	const auto* emptied = scene.GetTracker()->GetComponentsOfType(TrackedHazard::ComponentTypeID());
	ASSERT_NE(nullptr, emptied);
	EXPECT_EQ(0u, STL::Count(*emptied));
}

TEST(TrackerTest, TypedSceneQueries)
{
	Scene scene;

	TrackedSolid solid(Math::Vec2(4.f, 4.f));
	Hitbox solidBox(2.f, 2.f);
	solid.SetCollider(&solidBox);

	Entity player(Math::Vec2(5.f, 5.f));
	Hitbox playerBox(2.f, 2.f);
	player.SetCollider(&playerBox);

	scene.Add(&solid);
	scene.Add(&player);
	scene.Entities()->UpdateLists();

	// Untracked types simply report no hits
	EXPECT_FALSE(scene.CollideCheck<Entity>(Math::Vec2(5.f, 5.f)));

	EXPECT_TRUE(scene.CollideCheck<TrackedSolid>(Math::Vec2(5.f, 5.f)));
	EXPECT_FALSE(scene.CollideCheck<TrackedSolid>(Math::Vec2(50.f, 50.f)));
	EXPECT_EQ(&solid, scene.CollideFirst<TrackedSolid>(Math::Vec2(5.f, 5.f)));
	EXPECT_EQ(nullptr, scene.CollideFirst<TrackedSolid>(Math::Vec2(50.f, 50.f)));

	EXPECT_TRUE(scene.CollideCheck<TrackedSolid>(Math::Vec2(0.f, 5.f), Math::Vec2(10.f, 5.f)));
	EXPECT_FALSE(scene.CollideCheck<TrackedSolid>(Math::Vec2(0.f, 9.f), Math::Vec2(10.f, 9.f)));

	// Rect queries: the engine's Y axis points up
	EXPECT_TRUE(scene.CollideCheck<TrackedSolid>(MakeRect(5.f, 5.f, 2.f, 2.f)));
	EXPECT_FALSE(scene.CollideCheck<TrackedSolid>(MakeRect(8.f, 8.f, 2.f, 2.f)));
	EXPECT_EQ(&solid, scene.CollideFirst<TrackedSolid>(MakeRect(5.f, 5.f, 2.f, 2.f)));

	auto hits = scene.CollideAll<TrackedSolid>(MakeRect(0.f, 0.f, 20.f, 20.f));
	EXPECT_EQ(1u, STL::Count(hits));
	EXPECT_EQ(&solid, STL::Front(hits));
}

TEST(TrackerTest, TypedEntityQueries)
{
	static const BitTag s_solidTag("csd_tracker_solid");

	Scene scene;

	TrackedSolid solid(Math::Vec2(4.f, 4.f));
	Hitbox solidBox(2.f, 2.f);
	solid.SetCollider(&solidBox);
	solid.AddTag(s_solidTag);

	TrackedSolid farSolid(Math::Vec2(100.f, 100.f));
	Hitbox farBox(2.f, 2.f);
	farSolid.SetCollider(&farBox);
	farSolid.AddTag(s_solidTag);

	Entity player(Math::Vec2(5.f, 5.f));
	Hitbox playerBox(2.f, 2.f);
	player.SetCollider(&playerBox);

	scene.Add(&solid);
	scene.Add(&farSolid);
	scene.Add(&player);
	scene.Entities()->UpdateLists();

	EXPECT_TRUE(player.CollideCheck<TrackedSolid>());
	EXPECT_EQ(&solid, player.CollideFirst<TrackedSolid>());
	EXPECT_EQ(1, static_cast<int>(STL::Count(player.CollideAll<TrackedSolid>())));

	int visited = 0;
	player.CollideDo<TrackedSolid>([&visited](TrackedSolid*) { ++visited; });
	EXPECT_EQ(1, visited);

	// The queried position can be overridden
	EXPECT_FALSE(player.CollideCheck<TrackedSolid>(Math::Vec2(90.f, 90.f)));

	// CollideRect and the Outside/Closest helpers
	EXPECT_TRUE(player.CollideRect(MakeRect(0.f, 0.f, 10.f, 10.f)));
	EXPECT_FALSE(player.CollideRect(MakeRect(20.f, 20.f, 5.f, 5.f)));
	// The player overlaps solid but not farSolid
	EXPECT_TRUE(player.CollideCheckOutside(s_solidTag));
	EXPECT_EQ(&farSolid, player.CollideFirstOutside(s_solidTag));
	EXPECT_EQ(&solid, player.Closest(s_solidTag));
}

TEST(TrackerTest, TypedQueriesByComponent)
{
	Scene scene;

	Entity hazardEntity(Math::Vec2(4.f, 4.f));
	TrackedHazard hazard;
	Hitbox hazardBox(2.f, 2.f);
	hazard.SetCollider(&hazardBox);
	hazardEntity.Add(&hazard);

	Entity player(Math::Vec2(5.f, 5.f));
	Hitbox playerBox(2.f, 2.f);
	player.SetCollider(&playerBox);

	scene.Add(&hazardEntity);
	scene.Add(&player);
	scene.Entities()->UpdateLists();

	EXPECT_TRUE(player.CollideCheckByComponent<TrackedHazard>());
	EXPECT_EQ(&hazard, player.CollideFirstByComponent<TrackedHazard>());
	EXPECT_EQ(1, static_cast<int>(STL::Count(player.CollideAllByComponent<TrackedHazard>())));

	EXPECT_TRUE(scene.CollideCheckByComponent<TrackedHazard>(Math::Vec2(5.f, 5.f)));
	EXPECT_EQ(&hazard, scene.CollideFirstByComponent<TrackedHazard>(MakeRect(0.f, 0.f, 10.f, 10.f)));
	EXPECT_EQ(1, static_cast<int>(STL::Count(
					 scene.CollideAllByComponent<TrackedHazard>(Math::Vec2(0.f, 5.f), Math::Vec2(10.f, 5.f)))));
}

TEST(SceneCollideTest, TagRectFirstAndAll)
{
	static const BitTag s_tag("csd_scene_collide_tag");

	Scene scene;
	Entity solid(Math::Vec2(4.f, 4.f));
	Hitbox solidBox(2.f, 2.f);
	solid.SetCollider(&solidBox);
	solid.AddTag(s_tag);
	scene.Add(&solid);
	scene.Entities()->UpdateLists();

	// The scene collide helpers take the tag id, not the tag's bit value
	int tagId = s_tag.ID();

	EXPECT_TRUE(scene.CollideCheck(MakeRect(5.f, 5.f, 1.f, 1.f), tagId));
	EXPECT_FALSE(scene.CollideCheck(MakeRect(9.f, 9.f, 1.f, 1.f), tagId));

	EXPECT_EQ(&solid, scene.CollideFirst(Math::Vec2(5.f, 5.f), tagId));
	EXPECT_EQ(nullptr, scene.CollideFirst(Math::Vec2(50.f, 50.f), tagId));
	EXPECT_EQ(&solid, scene.CollideFirst(MakeRect(0.f, 0.f, 10.f, 10.f), tagId));

	auto hits = scene.CollideAll(Math::Vec2(4.5f, 4.5f), tagId);
	EXPECT_EQ(1u, STL::Count(hits));
}

} // namespace
