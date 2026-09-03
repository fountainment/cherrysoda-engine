#include "Test.h"

#include <cmath>

namespace {

TEST(ColliderTestCircle, PointCollision)
{
	Circle circle(2.f, Math::Vec2(5.f, 5.f));

	EXPECT_TRUE(circle.Collide(Math::Vec2(5.f, 5.f)));
	EXPECT_TRUE(circle.Collide(Math::Vec2(7.f, 5.f))); // exactly on the rim
	EXPECT_TRUE(circle.Collide(Math::Vec2(3.f, 5.f)));
	EXPECT_FALSE(circle.Collide(Math::Vec2(7.01f, 5.f)));
	EXPECT_FALSE(circle.Collide(Math::Vec2(0.f, 0.f)));
}

TEST(ColliderTestCircle, CircleRectAndLineCollision)
{
	Circle circle(2.f, Math::Vec2(5.f, 5.f));

	Circle touching(1.f, Math::Vec2(8.f, 5.f)); // distance 3 == 2 + 1
	EXPECT_TRUE(circle.Collide(&touching));
	Circle apart(0.9f, Math::Vec2(8.f, 5.f)); // distance 3 > 2.9
	EXPECT_FALSE(circle.Collide(&apart));

	// Rectangle containing the circle center
	EXPECT_TRUE(circle.Collide(Math::Rectangle{Math::Vec2(3.f, 3.f), Math::Vec2(4.f, 4.f)}));
	EXPECT_FALSE(circle.Collide(Math::Rectangle{Math::Vec2(10.f, 10.f), Math::Vec2(4.f, 4.f)}));

	// Line through the center vs. far away line
	EXPECT_TRUE(circle.Collide(Math::Vec2(-5.f, 5.f), Math::Vec2(15.f, 5.f)));
	EXPECT_FALSE(circle.Collide(Math::Vec2(-5.f, 9.f), Math::Vec2(15.f, 9.f)));
}

TEST(ColliderTestCircle, Bounds)
{
	Circle circle(2.f, Math::Vec2(5.f, 5.f));

	EXPECT_FLOAT_EQ(3.f, circle.Left());
	EXPECT_FLOAT_EQ(7.f, circle.Right());
	EXPECT_FLOAT_EQ(3.f, circle.Bottom());
	EXPECT_FLOAT_EQ(7.f, circle.Top());
	EXPECT_FLOAT_EQ(4.f, circle.Width());
	EXPECT_FLOAT_EQ(4.f, circle.Height());
	EXPECT_FLOAT_EQ(4.f, circle.RadiusSq());
}

TEST(ColliderTestHitbox, PointAndIntersects)
{
	Hitbox hitbox(10.f, 10.f);

	EXPECT_TRUE(hitbox.Collide(Math::Vec2(0.f, 0.f))); // bottom-left corner counts as inside
	EXPECT_TRUE(hitbox.Collide(Math::Vec2(9.99f, 9.99f)));
	EXPECT_FALSE(hitbox.Collide(Math::Vec2(10.f, 5.f))); // right edge is exclusive
	EXPECT_FALSE(hitbox.Collide(Math::Vec2(-0.01f, 5.f)));
	EXPECT_FALSE(hitbox.Collide(Math::Vec2(5.f, 10.f))); // top edge is exclusive

	Hitbox overlapping(5.f, 5.f, 5.f, 5.f);
	EXPECT_TRUE(hitbox.Intersects(&overlapping));

	Hitbox touching(5.f, 5.f, 10.f, 10.f); // shares only the right edge
	EXPECT_FALSE(hitbox.Intersects(&touching));

	Hitbox separate(5.f, 5.f, 100.f, 100.f);
	EXPECT_FALSE(hitbox.Intersects(&separate));
}

TEST(ColliderTestHitbox, RectLineAndCircleCollision)
{
	Hitbox hitbox(2.f, 10.f);

	// Rectangle overlap
	EXPECT_TRUE(hitbox.Collide(Math::Rectangle{Math::Vec2(1.f, 5.f), Math::Vec2(2.f, 2.f)}));
	EXPECT_FALSE(hitbox.Collide(Math::Rectangle{Math::Vec2(3.f, 5.f), Math::Vec2(2.f, 2.f)}));

	// Line crossing vs. missing
	EXPECT_TRUE(hitbox.Collide(Math::Vec2(-1.f, 5.f), Math::Vec2(5.f, 5.f)));
	EXPECT_FALSE(hitbox.Collide(Math::Vec2(-1.f, 20.f), Math::Vec2(5.f, 20.f)));

	// Circle delegation
	Circle overlappingCircle(1.f, Math::Vec2(1.f, 5.f));
	Circle missingCircle(0.5f, Math::Vec2(5.f, 5.f));
	EXPECT_TRUE(hitbox.Collide(&overlappingCircle));
	EXPECT_FALSE(hitbox.Collide(&missingCircle));
}

TEST(ColliderTestHitbox, CenterOrigin)
{
	Hitbox hitbox(10.f, 6.f);
	hitbox.CenterOrigin();
	EXPECT_FLOAT_EQ(-5.f, hitbox.Left());
	EXPECT_FLOAT_EQ(5.f, hitbox.Right());
	EXPECT_FLOAT_EQ(-3.f, hitbox.Bottom());
	EXPECT_FLOAT_EQ(3.f, hitbox.Top());
	EXPECT_TRUE(hitbox.Collide(Math::Vec2(0.f, 0.f)));
}

TEST(ColliderTestGrid, SetRectCheckRectClamping)
{
	Grid grid(4, 4, 10, 10);

	grid.SetRect(1, 1, 2, 2);
	for (int x = 0; x < 4; ++x) {
		for (int y = 0; y < 4; ++y) {
			bool inRect = (x == 1 || x == 2) && (y == 1 || y == 2);
			EXPECT_EQ(inRect, grid.Get(x, y));
		}
	}

	EXPECT_TRUE(grid.CheckRect(1, 1, 2, 2));
	EXPECT_TRUE(grid.CheckRect(2, 2, 1, 1));
	EXPECT_FALSE(grid.CheckRect(0, 0, 1, 1));
	EXPECT_FALSE(grid.CheckRect(3, 3, 1, 1));

	// Negative offsets get clipped against the grid origin
	Grid clipped(4, 4, 10, 10);
	clipped.SetRect(-1, -1, 3, 3);
	EXPECT_TRUE(clipped.Get(0, 0));
	EXPECT_TRUE(clipped.Get(1, 0));
	EXPECT_TRUE(clipped.Get(0, 1));
	EXPECT_TRUE(clipped.Get(1, 1));
	EXPECT_FALSE(clipped.Get(2, 0));
	EXPECT_FALSE(clipped.Get(0, 2));

	// Over-sized rects get clipped against the grid extent
	EXPECT_TRUE(clipped.CheckRect(-5, -5, 100, 100));
	Grid empty(4, 4, 10, 10);
	EXPECT_FALSE(empty.CheckRect(-5, -5, 100, 100));
	EXPECT_FALSE(empty.CheckRect(4, 4, 1, 1)); // entirely outside

	// Out-of-bounds Get is always false
	EXPECT_FALSE(grid.Get(-1, 0));
	EXPECT_FALSE(grid.Get(0, -1));
	EXPECT_FALSE(grid.Get(4, 0));
	EXPECT_FALSE(grid.Get(0, 4));

	// Set ignores the bounds helper and writes through to the VirtualMap
	grid.Set(3, 3, true);
	EXPECT_TRUE(grid.Get(3, 3));
}

TEST(ColliderTestGrid, PointAndRectCollision)
{
	Grid grid(4, 4, 10, 10);
	grid.SetRect(1, 1, 2, 2);

	EXPECT_EQ(4, grid.CellsX());
	EXPECT_EQ(4, grid.CellsY());
	EXPECT_FLOAT_EQ(10.f, grid.CellWidth());
	EXPECT_FLOAT_EQ(40.f, grid.Width());
	EXPECT_FLOAT_EQ(40.f, grid.Height());

	EXPECT_TRUE(grid.Collide(Math::Vec2(15.f, 15.f))); // cell (1,1)
	EXPECT_TRUE(grid.Collide(Math::Vec2(25.f, 25.f))); // cell (2,2)
	EXPECT_FALSE(grid.Collide(Math::Vec2(5.f, 5.f)));  // cell (0,0)
	EXPECT_FALSE(grid.Collide(Math::Vec2(45.f, 5.f))); // outside on X
	EXPECT_FALSE(grid.Collide(Math::Vec2(5.f, -1.f))); // outside on Y

	EXPECT_TRUE(grid.Collide(Math::Rectangle{Math::Vec2(12.f, 12.f), Math::Vec2(2.f, 2.f)}));
	EXPECT_FALSE(grid.Collide(Math::Rectangle{Math::Vec2(2.f, 2.f), Math::Vec2(2.f, 2.f)}));
	EXPECT_FALSE(grid.Collide(Math::Rectangle{Math::Vec2(50.f, 50.f), Math::Vec2(2.f, 2.f)}));
}

TEST(ColliderTestGrid, LineCollision)
{
	Grid grid(4, 4, 10, 10);
	grid.Set(2, 0, true); // spans world x in [20, 30), y in [0, 10)

	EXPECT_TRUE(grid.Collide(Math::Vec2(0.f, 5.f), Math::Vec2(40.f, 5.f)));     // crosses the solid cell
	EXPECT_FALSE(grid.Collide(Math::Vec2(0.f, 15.f), Math::Vec2(40.f, 15.f)));  // same column, empty row
	EXPECT_FALSE(grid.Collide(Math::Vec2(50.f, 50.f), Math::Vec2(80.f, 50.f))); // outside the grid

	// Diagonal ray stepping across several cells into the solid one
	EXPECT_TRUE(grid.Collide(Math::Vec2(0.f, 40.f), Math::Vec2(25.f, 5.f)));
}

TEST(ColliderTestColliderList, CollideDispatch)
{
	Circle circle(1.f);
	Hitbox farBox(10.f, 10.f, 20.f, 20.f);
	ColliderList::IterableColliders colliders = {&circle, &farBox};
	ColliderList list(colliders);

	EXPECT_TRUE(list.Collide(Math::Vec2(0.f, 0.f)));    // hits the circle
	EXPECT_TRUE(list.Collide(Math::Vec2(25.f, 25.f)));  // hits the hitbox
	EXPECT_FALSE(list.Collide(Math::Vec2(10.f, 10.f))); // hits neither

	Circle overlappingCircle(0.5f);
	Hitbox overlappingBox(2.f, 2.f, 24.f, 24.f);
	Hitbox missingBox(2.f, 2.f, 10.f, 10.f);
	EXPECT_TRUE(list.Collide(&overlappingCircle));
	EXPECT_TRUE(list.Collide(&overlappingBox));
	EXPECT_FALSE(list.Collide(&missingBox));

	EXPECT_TRUE(list.Collide(Math::Rectangle{Math::Vec2(-1.f, -1.f), Math::Vec2(1.f, 1.f)}));
	EXPECT_FALSE(list.Collide(Math::Rectangle{Math::Vec2(5.f, 5.f), Math::Vec2(1.f, 1.f)}));

	EXPECT_TRUE(list.Collide(Math::Vec2(-5.f, 0.f), Math::Vec2(5.f, 0.f))); // line through the circle
	EXPECT_FALSE(list.Collide(Math::Vec2(-5.f, 5.f), Math::Vec2(5.f, 5.f)));

	// Union bounds over all children
	EXPECT_FLOAT_EQ(-1.f, list.Left());
	EXPECT_FLOAT_EQ(30.f, list.Right());
	EXPECT_FLOAT_EQ(-1.f, list.Bottom());
	EXPECT_FLOAT_EQ(30.f, list.Top());
	EXPECT_FLOAT_EQ(31.f, list.Width());
	EXPECT_FLOAT_EQ(31.f, list.Height());
}

TEST(ColliderTestColliderList, EmptyBoundsAreNaN)
{
	ColliderList::IterableColliders empty;
	ColliderList list(empty);

	EXPECT_TRUE(std::isnan(list.Left()));
	EXPECT_TRUE(std::isnan(list.Right()));
	EXPECT_TRUE(std::isnan(list.Bottom()));
	EXPECT_TRUE(std::isnan(list.Top()));
}

TEST(ColliderTestColliderList, NegativeCoordinateBounds)
{
	Hitbox nearBox(2.f, 2.f, -10.f, -10.f);
	Hitbox farBox(2.f, 2.f, -30.f, -30.f);
	ColliderList::IterableColliders colliders = {&nearBox, &farBox};
	ColliderList list(colliders);

	// Right/Top pick the max child bound even when every bound is negative
	EXPECT_FLOAT_EQ(-8.f, list.Right());
	EXPECT_FLOAT_EQ(-8.f, list.Top());
	EXPECT_FLOAT_EQ(-30.f, list.Left());
	EXPECT_FLOAT_EQ(-30.f, list.Bottom());
}

TEST(ColliderTestColliderList, AddAndRemove)
{
	Circle circle(1.f);
	ColliderList::IterableColliders empty;
	ColliderList list(empty);

	ColliderList::IterableColliders toAdd = {&circle};
	list.Add(toAdd);
	EXPECT_TRUE(list.Collide(Math::Vec2(0.f, 0.f)));

	ColliderList::IterableColliders toRemove = {&circle};
	list.Remove(toRemove);
	EXPECT_FALSE(list.Collide(Math::Vec2(0.f, 0.f)));
}

TEST(ColliderTestEntityBinding, AbsolutePositions)
{
	Hitbox hitbox(10.f, 10.f);
	Entity entity(Math::Vec2(100.f, 50.f));

	// Without an entity the collider reports local coordinates
	EXPECT_FLOAT_EQ(0.f, hitbox.AbsoluteLeft());
	EXPECT_FLOAT_EQ(10.f, hitbox.AbsoluteRight());

	entity.SetCollider(&hitbox);
	EXPECT_EQ(&entity, hitbox.GetEntity());

	// Bound to the entity the collider reports entity-relative coordinates
	EXPECT_FLOAT_EQ(100.f, hitbox.AbsoluteLeft());
	EXPECT_FLOAT_EQ(110.f, hitbox.AbsoluteRight());
	EXPECT_FLOAT_EQ(50.f, hitbox.AbsoluteBottom());
	EXPECT_FLOAT_EQ(60.f, hitbox.AbsoluteTop());

	// Entity-level bounds include the collider extents
	EXPECT_FLOAT_EQ(100.f, entity.Left());
	EXPECT_FLOAT_EQ(110.f, entity.Right());
	EXPECT_FLOAT_EQ(50.f, entity.Bottom());
	EXPECT_FLOAT_EQ(60.f, entity.Top());

	// Point collision uses the entity offset
	EXPECT_TRUE(hitbox.Collide(Math::Vec2(105.f, 55.f)));
	EXPECT_FALSE(hitbox.Collide(Math::Vec2(5.f, 5.f)));

	// Edge setters move the entity so the collider lands on the requested edge
	entity.Left(0.f);
	EXPECT_FLOAT_EQ(0.f, entity.PositionX());
	EXPECT_FLOAT_EQ(0.f, entity.Left());

	entity.Top(100.f);
	EXPECT_FLOAT_EQ(90.f, entity.PositionY());
	EXPECT_FLOAT_EQ(100.f, entity.Top());

	entity.SetCollider(nullptr);
	EXPECT_EQ(nullptr, hitbox.GetEntity());
}

} // namespace
