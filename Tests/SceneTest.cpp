#include "Test.h"

#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Util/Pool.h>

namespace {

// Registered once per test binary; only a handful of tags may exist in a process.
const BitTag s_solidTag("csd_scene_test_solid");
const BitTag s_playerTag("csd_scene_test_player");
const BitTag s_npcTag("csd_scene_test_npc");

// A component that records which lifecycle callbacks have fired on it.
class SceneTestComponent : public Component
{
public:
	SceneTestComponent() : base(true, false) {}

	CHERRYSODA_DECLARE_COMPONENT(SceneTestComponent, Component);

	void Added(Entity* entity) override
	{
		++addedCount;
		base::Added(entity);
	}

	void Removed(Entity* entity) override
	{
		++removedCount;
		base::Removed(entity);
	}

	void EntityAdded(Scene* /*scene*/) override { ++entityAddedCount; }

	void EntityRemoved(Scene* /*scene*/) override { ++entityRemovedCount; }

	void EntityAwake() override { ++awakeCount; }

	void Update() override { ++updateCount; }

	int addedCount = 0;
	int removedCount = 0;
	int entityAddedCount = 0;
	int entityRemovedCount = 0;
	int awakeCount = 0;
	int updateCount = 0;
};

int CountEntities(Scene& scene)
{
	int count = 0;
	for (auto* entity : *scene.Entities()) {
		(void)entity;
		++count;
	}
	return count;
}

TEST(SceneTestEntityLifecycle, DeferredAddAndRemove)
{
	Scene scene;
	Entity entity;
	SceneTestComponent component;
	entity.Add(&component);

	// Adding a component while the list is open fires Added immediately
	EXPECT_EQ(1, component.addedCount);
	EXPECT_EQ(&entity, component.GetEntity());

	// The entity is not part of the scene until the lists update; the scene's
	// helper entity is pending too at this point.
	scene.Add(&entity);
	EXPECT_EQ(nullptr, entity.GetScene());
	EXPECT_EQ(0, CountEntities(scene));

	scene.Entities()->UpdateLists();
	EXPECT_EQ(&scene, entity.GetScene());
	EXPECT_EQ(2, CountEntities(scene)); // test entity + helper entity

	// Entity::Added propagates to components, then the deferred Awake pass runs
	EXPECT_EQ(1, component.entityAddedCount);
	EXPECT_EQ(1, component.awakeCount);

	// Removal is deferred the same way
	scene.Remove(&entity);
	EXPECT_EQ(&scene, entity.GetScene());
	EXPECT_EQ(2, CountEntities(scene));

	scene.Entities()->UpdateLists();
	EXPECT_EQ(nullptr, entity.GetScene());
	EXPECT_EQ(1, CountEntities(scene)); // only the helper entity remains
	EXPECT_EQ(1, component.entityRemovedCount);
	// Component::Removed only fires on Entity::Remove(component), not when
	// the whole entity leaves the scene.
	EXPECT_EQ(0, component.removedCount);
}

TEST(SceneTestEntityLifecycle, UpdateRunsActiveComponents)
{
	Scene scene;
	Entity entity;
	SceneTestComponent component;
	entity.Add(&component);
	scene.Add(&entity);
	scene.Entities()->UpdateLists();

	// Scene::Update walks the (active) entities and their (active) components
	scene.Update();
	EXPECT_EQ(1, component.updateCount);

	component.Active(false);
	scene.Update();
	EXPECT_EQ(1, component.updateCount);

	component.Active(true);
	entity.Active(false);
	scene.Update();
	EXPECT_EQ(1, component.updateCount);
}

TEST(SceneTestTags, AddRemoveAndLookup)
{
	Scene scene;
	Entity solid;
	Entity player;
	scene.Add(&solid);
	scene.Add(&player);
	scene.Entities()->UpdateLists();

	EXPECT_EQ(0u, solid.Tag());

	// Tagging an entity in a scene registers it in the tag list
	solid.AddTag(s_solidTag);
	EXPECT_TRUE(solid.TagCheck(s_solidTag));
	EXPECT_TRUE(STL::Contains(scene.Get(s_solidTag), &solid));
	EXPECT_EQ(1u, STL::Count(scene.Get(s_solidTag)));

	player.AddTag(s_playerTag);
	player.AddTag(s_solidTag);
	EXPECT_TRUE(player.TagFullCheck(s_playerTag | s_solidTag));
	EXPECT_FALSE(player.TagFullCheck(s_playerTag | s_solidTag | s_npcTag));
	EXPECT_EQ(2u, STL::Count(scene.Get(s_solidTag)));

	// Mask queries over the whole scene
	auto taggedBoth = scene.GetEntitiesByTagMask(s_solidTag | s_playerTag);
	EXPECT_EQ(2u, STL::Count(taggedBoth));
	auto excludingSolid = scene.GetEntitiesExcludingTagMask(s_solidTag);
	EXPECT_EQ(1u, STL::Count(excludingSolid)); // only the scene's untagged helper entity
	auto npcOnly = scene.GetEntitiesByTagMask(s_npcTag);
	EXPECT_EQ(0u, STL::Count(npcOnly));

	// Untagging removes it from the list again
	player.RemoveTag(s_solidTag);
	EXPECT_FALSE(STL::Contains(scene.Get(s_solidTag), &player));
	EXPECT_EQ(1u, STL::Count(scene.Get(s_solidTag)));

	// Removing the entity from the scene clears it from all tag lists
	scene.Remove(&solid);
	scene.Entities()->UpdateLists();
	EXPECT_EQ(0u, STL::Count(scene.Get(s_solidTag)));
}

TEST(SceneTestDepth, EntitiesSortedByActualDepth)
{
	Scene scene;
	Entity deep;
	Entity middle;
	Entity shallow;
	Entity tied;

	deep.Depth(5);
	middle.Depth(3);
	shallow.Depth(1);
	tied.Depth(3);

	scene.Add(&deep);
	scene.Add(&middle);
	scene.Add(&shallow);
	scene.Add(&tied);
	scene.Entities()->UpdateLists();

	// Higher depth renders first; equal depths keep insertion order via the
	// micro-depth offset handed out by Scene::INTERNAL_SetActualDepth.
	STL::Vector<Entity*> order;
	for (auto* entity : *scene.Entities()) {
		STL::Add(order, entity);
	}
	ASSERT_EQ(5u, STL::Count(order)); // four entities plus the scene's helper entity
	EXPECT_EQ(&deep, order[0]);
	EXPECT_EQ(&middle, order[1]);
	EXPECT_EQ(&tied, order[2]);
	EXPECT_EQ(&shallow, order[3]);
	EXPECT_EQ(scene.HelperEntity(), order[4]);
}

TEST(SceneTestEndOfFrame, ActionsDrainedByAfterUpdate)
{
	Scene scene;

	int firedCount = 0;
	scene.AddActionOnEndOfFrame([&firedCount] { ++firedCount; });

	// Actions wait until the end of the frame
	EXPECT_EQ(0, firedCount);

	scene.AfterUpdate();
	EXPECT_EQ(1, firedCount);

	// Each action only fires once
	scene.AfterUpdate();
	EXPECT_EQ(1, firedCount);
}

TEST(SceneTestCollision, CollideCheckPointAndLine)
{
	Scene scene;
	Entity solid(Math::Vec2(4.f, 4.f));
	Hitbox solidBox(2.f, 2.f);
	solid.SetCollider(&solidBox);
	solid.AddTag(s_solidTag);
	scene.Add(&solid);
	scene.Entities()->UpdateLists();

	int solidTagId = s_solidTag.ID();

	// Point checks against a tag
	EXPECT_TRUE(scene.CollideCheck(Math::Vec2(5.f, 5.f), solidTagId));
	EXPECT_TRUE(scene.CollideCheck(Math::Vec2(4.f, 4.f), solidTagId));
	EXPECT_FALSE(scene.CollideCheck(Math::Vec2(6.01f, 5.f), solidTagId));
	EXPECT_FALSE(scene.CollideCheck(Math::Vec2(0.f, 0.f), solidTagId));

	// Line checks against a tag
	EXPECT_TRUE(scene.CollideCheck(Math::Vec2(0.f, 5.f), Math::Vec2(10.f, 5.f), solidTagId));
	EXPECT_FALSE(scene.CollideCheck(Math::Vec2(0.f, 7.f), Math::Vec2(10.f, 7.f), solidTagId));

	// LineWalkCheck returns the last free point before the walk is blocked,
	// or the end point when nothing blocks it.
	EXPECT_EQ(Math::Vec2(3.f, 5.f), scene.LineWalkCheck(Math::Vec2(0.f, 5.f), Math::Vec2(10.f, 5.f), solidTagId, 1.f));
	EXPECT_EQ(Math::Vec2(10.f, 3.f), scene.LineWalkCheck(Math::Vec2(0.f, 3.f), Math::Vec2(10.f, 3.f), solidTagId, 1.f));
}

TEST(SceneTestCollision, EntityLevelQueries)
{
	Scene scene;
	Entity solid(Math::Vec2(4.f, 4.f));
	Hitbox solidBox(2.f, 2.f);
	solid.SetCollider(&solidBox);
	solid.AddTag(s_solidTag);

	Entity other(Math::Vec2(5.f, 5.f));
	Hitbox otherBox(2.f, 2.f);
	other.SetCollider(&otherBox);

	Entity away(Math::Vec2(50.f, 50.f));
	Hitbox awayBox(2.f, 2.f);
	away.SetCollider(&awayBox);

	scene.Add(&solid);
	scene.Add(&other);
	scene.Add(&away);
	scene.Entities()->UpdateLists();

	// Entity wrappers around the Collide helpers
	EXPECT_TRUE(other.CollideCheck(&solid));
	EXPECT_TRUE(other.CollideCheck(s_solidTag));
	EXPECT_TRUE(other.CollidePoint(Math::Vec2(6.f, 6.f)));
	EXPECT_FALSE(other.CollidePoint(Math::Vec2(0.f, 0.f)));
	EXPECT_TRUE(other.CollideLine(Math::Vec2(0.f, 6.f), Math::Vec2(10.f, 6.f)));
	EXPECT_FALSE(other.CollideLine(Math::Vec2(0.f, 20.f), Math::Vec2(10.f, 20.f)));

	EXPECT_EQ(1, other.CollideCount(s_solidTag));
	EXPECT_EQ(&solid, other.CollideFirst(s_solidTag));

	auto all = other.CollideAll(s_solidTag);
	EXPECT_EQ(1u, STL::Count(all));
	EXPECT_EQ(&solid, STL::Front(all));

	// Collidable off on the queried entity disables the checks against it
	solid.Collidable(false);
	EXPECT_FALSE(other.CollideCheck(&solid));
	EXPECT_FALSE(other.CollideCheck(s_solidTag));
	EXPECT_EQ(0, other.CollideCount(s_solidTag));
	EXPECT_EQ(nullptr, other.CollideFirst(s_solidTag));
	solid.Collidable(true);
}

TEST(SceneTestPool, PooledEntityRemovedAndRecycled)
{
	Scene scene;
	Pool<Entity, 2> pool;

	Entity* first = pool.Create(Math::Vec2(1.f, 2.f));
	Entity* second = pool.Create();
	EXPECT_NE(nullptr, first);
	EXPECT_NE(nullptr, second);
	EXPECT_TRUE(pool.IsFull());
	EXPECT_FLOAT_EQ(2.f, first->PositionY());

	int traversedCount = 0;
	pool.Traverse([&traversedCount](Entity*) { ++traversedCount; });
	EXPECT_EQ(2, traversedCount);

	// Removing a pooled entity from the scene hides it, then destroys it at
	// the end of the frame, freeing its slot for recycling.
	scene.Add(first);
	scene.Entities()->UpdateLists();
	first->RemoveSelf();
	scene.Entities()->UpdateLists();
	scene.AfterUpdate();

	EXPECT_FALSE(pool.IsFull());
	Entity* recycled = pool.Create();
	EXPECT_NE(nullptr, recycled);
	EXPECT_EQ(first, recycled); // LIFO slot reuse
}

} // namespace
