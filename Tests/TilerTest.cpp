#include "Test.h"

#include <CherrySoda/Colliders/Grid.h>
#include <CherrySoda/Components/Graphics/TileGrid.h>
#include <CherrySoda/Graphics/TileSet.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Json.h>
#include <CherrySoda/Util/Tiler.h>

namespace {

TEST(GridBitstringTest, ConstructorSizesAndRoundTrips)
{
	String bits = "10\n01";
	Grid grid(8.f, 8.f, bits);

	EXPECT_EQ(2, grid.CellsX());
	EXPECT_EQ(2, grid.CellsY());
	EXPECT_EQ(bits, grid.GetBitstring());
	EXPECT_TRUE(grid.Get(0, 0));
	EXPECT_FALSE(grid.Get(1, 0));
	EXPECT_FALSE(grid.Get(0, 1));
	EXPECT_TRUE(grid.Get(1, 1));
}

TEST(GridBitstringTest, LoadPadsShortRows)
{
	Grid grid(3, 2, 8.f, 8.f);
	grid.LoadBitstring("111\n1");

	EXPECT_EQ(String("111\n100"), grid.GetBitstring());
}

TEST(GridBitstringTest, IsBitstringEmpty)
{
	EXPECT_TRUE(Grid::IsBitstringEmpty("0000\n0000\n0000"));
	EXPECT_FALSE(Grid::IsBitstringEmpty("0000\n0010"));
	EXPECT_TRUE(Grid::IsBitstringEmpty(""));
}

TEST(GridExtendTest, LeftAndDownShiftPositionAndRepeatEdges)
{
	Grid grid(8.f, 8.f, "10\n01");
	grid.Extend(1, 0, 0, 1);

	EXPECT_EQ(3, grid.CellsX());
	EXPECT_EQ(3, grid.CellsY());
	EXPECT_FLOAT_EQ(-8.f, grid.Position2D().x);
	EXPECT_FLOAT_EQ(-8.f, grid.Position2D().y);
	// The old data keeps its world placement; new border cells repeat the
	// old left column and bottom row
	EXPECT_EQ(String("110\n110\n001"), grid.GetBitstring());
}

TEST(GridExtendTest, RightAndUpKeepPosition)
{
	Grid grid(8.f, 8.f, "10\n01");
	grid.Extend(0, 1, 1, 0);

	EXPECT_FLOAT_EQ(0.f, grid.Position2D().x);
	EXPECT_FLOAT_EQ(0.f, grid.Position2D().y);
	EXPECT_EQ(String("100\n011\n011"), grid.GetBitstring());
}

TEST(GridQueriesTest, ColumnRowAndEmpty)
{
	Grid grid(8.f, 8.f, "111\n101\n101");

	EXPECT_TRUE(grid.CheckColumn(0));
	EXPECT_TRUE(grid.CheckColumn(2));
	EXPECT_FALSE(grid.CheckColumn(1));
	EXPECT_TRUE(grid.CheckRow(0));
	EXPECT_FALSE(grid.IsEmpty());

	grid.Clear();
	EXPECT_TRUE(grid.IsEmpty());

	grid.SetRect(0, 0, 3, 3, true);
	EXPECT_TRUE(grid.CheckRow(2));
	EXPECT_TRUE(grid.CheckColumn(1));
}

TEST(TilerTest, NeighborStateWithFalseEdges)
{
	VirtualMap<bool> bits(3, 3);
	bits.Set(1, 1, true);

	int queried = 0;
	bool sawLeft = true, sawRight = true, sawUp = true, sawDown = true;
	bool sawAllFalse = false;
	Tiler::Tile(
		bits,
		[&]() {
			++queried;
			sawLeft = Tiler::Left();
			sawRight = Tiler::Right();
			sawUp = Tiler::Up();
			sawDown = Tiler::Down();
			sawAllFalse = !Tiler::Left() && !Tiler::Right() && !Tiler::Up() && !Tiler::Down() && !Tiler::UpLeft() &&
						  !Tiler::UpRight() && !Tiler::DownLeft() && !Tiler::DownRight();
			return 0;
		},
		nullptr, EdgeBehavior::False);

	EXPECT_EQ(1, queried);
	EXPECT_TRUE(sawAllFalse);

	// A tile with only a neighbor above reports Down=true (that neighbor is
	// below it), the rest false
	bits.Set(1, 2, true);
	Tiler::Tile(
		bits,
		[&]() {
			sawLeft = Tiler::Left();
			sawRight = Tiler::Right();
			sawUp = Tiler::Up();
			sawDown = Tiler::Down();
			return 0;
		},
		nullptr, EdgeBehavior::False);
	// Queried in x-then-y order: (1,1) first has Up=true, then (1,2) has Down=true
	// The last query is the top tile
	EXPECT_FALSE(sawLeft);
	EXPECT_FALSE(sawRight);
	EXPECT_FALSE(sawUp);
	EXPECT_TRUE(sawDown);
}

TEST(TilerTest, AutotileDataPicksTiles)
{
	// Row 0 of the map is the bottom row (Y-up)
	const char* jsonText = R"({
		"Center": [4],
		"Single": [5],
		"SingleHorizontalCenter": [14],
		"SingleHorizontalLeft": [12],
		"SingleHorizontalRight": [13],
		"SingleVerticalCenter": [24],
		"SingleVerticalTop": [23],
		"SingleVerticalBottom": [25],
		"Top": [6],
		"Bottom": [7],
		"Left": [8],
		"Right": [9],
		"TopLeft": [16],
		"TopRight": [17],
		"BottomLeft": [18],
		"BottomRight": [19]
	})";
	json::Document doc;
	doc.Parse(jsonText);
	ASSERT_FALSE(doc.HasParseError());
	AutotileData autotile(doc);

	// A single isolated tile
	VirtualMap<bool> single(3, 3);
	single.Set(1, 1, true);
	auto tiles = Tiler::Tile(single, autotile, nullptr, EdgeBehavior::False);
	EXPECT_EQ(5, tiles[1][1]);

	// A horizontal run: left cap, center, right cap (the left cap's only
	// neighbor is to its right)
	VirtualMap<bool> run(3, 3);
	run.Set(0, 1, true);
	run.Set(1, 1, true);
	run.Set(2, 1, true);
	tiles = Tiler::Tile(run, autotile, nullptr, EdgeBehavior::False);
	EXPECT_EQ(12, tiles[0][1]);
	EXPECT_EQ(14, tiles[1][1]);
	EXPECT_EQ(13, tiles[2][1]);

	// A full block: bottom/right edge midpoint, center, top/right corner
	VirtualMap<bool> block(3, 3);
	for (int x = 0; x < 3; ++x)
		for (int y = 0; y < 3; ++y)
			block.Set(x, y, true);
	tiles = Tiler::Tile(block, autotile, nullptr, EdgeBehavior::False);
	EXPECT_EQ(7, tiles[1][0]);  // nothing below: bottom edge
	EXPECT_EQ(4, tiles[1][1]);  // surrounded: center
	EXPECT_EQ(17, tiles[2][2]); // left and down only: top-right corner
}

TEST(TilerTest, MaskCountsAsSolidForNeighbors)
{
	const char* jsonText = R"({
		"Center": [4],
		"Top": [6],
		"Bottom": [7],
		"BottomLeft": [18],
		"BottomRight": [19]
	})";
	json::Document doc;
	doc.Parse(jsonText);
	AutotileData autotile(doc);

	// A full block with a hole in the middle of its bottom edge
	VirtualMap<bool> bits(3, 3);
	for (int x = 0; x < 3; ++x)
		for (int y = 0; y < 3; ++y)
			bits.Set(x, y, true);
	bits.Set(1, 0, false);

	// Without the mask the hole is open, so (1,1) is a bottom-edge tile
	auto tiles = Tiler::Tile(bits, autotile, nullptr, EdgeBehavior::False);
	EXPECT_EQ(7, tiles[1][1]);

	// With the mask covering the hole, (1,1) is a center tile; masked cells
	// produce no tiles themselves
	VirtualMap<bool> mask(3, 3);
	mask.Set(1, 0, true);
	tiles = Tiler::Tile(bits, mask, autotile, nullptr, EdgeBehavior::False);
	EXPECT_EQ(4, tiles[1][1]);
	EXPECT_EQ(-1, tiles[1][0]);
}

TEST(TileGridTest, OverlaySkipsNegativeAndExtendMovesTiles)
{
	// The atlas-flavored constructor takes the size explicitly, so a sheet
	// can be built from pure rectangle math without a GPU texture
	MTexture root;
	MTexture sheet(root, StringID("tiler_test_sheet"), Math::IRectangle{Math::IVec2(0, 0), Math::IVec2(0, 0)},
				   Math::Vec2(0.f), 16, 16);
	TileSet tileset(sheet, 8, 8); // a 2x2 sheet of 8x8 tiles

	TileGrid grid(8, 8, 2, 2);
	STL::Vector<STL::Vector<int>> tiles = {{0, -1}, {1, 0}};
	grid.Overlay(&tileset, tiles);

	// tiles[x][y]: -1 keeps the cell empty, the rest index the tileset
	EXPECT_EQ(tileset.Get(0), grid.Get(0, 0));
	EXPECT_EQ(tileset.Get(1), grid.Get(1, 0));
	EXPECT_EQ(nullptr, grid.Get(0, 1));
	EXPECT_EQ(tileset.Get(0), grid.Get(1, 1));

	const MTexture* moved = grid.Get(1, 1);
	grid.Extend(1, 0, 0, 0);
	EXPECT_EQ(3, grid.TilesX());
	EXPECT_EQ(2, grid.TilesY());
	EXPECT_FLOAT_EQ(-8.f, grid.Position().x);
	// The old (1,1) tile now lives at (2,1)
	EXPECT_EQ(moved, grid.Get(2, 1));
}

} // namespace
