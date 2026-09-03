#ifndef _CHERRYSODA_UTIL_TILER_H_
#define _CHERRYSODA_UTIL_TILER_H_

#include <CherrySoda/Util/Json.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/VirtualMap.h>

namespace cherrysoda {

enum class EdgeBehavior
{
	True,
	False,
	Wrap
};

// Tile-id table for the 47-tile autotiling ruleset; each entry lists the
// possible ids and a random one is picked when there are several
class AutotileData
{
public:
	AutotileData() = default;
	// Reads optional int arrays keyed by the field names ("Center", "Single",
	// "SingleHorizontalLeft", ...); missing keys leave the entry empty
	AutotileData(const json::Value& json);

	// Picks the tile id for the tile Tiler is currently deciding
	int TileHandler() const;

	STL::Vector<int> m_center;
	STL::Vector<int> m_single;
	STL::Vector<int> m_singleHorizontalLeft;
	STL::Vector<int> m_singleHorizontalCenter;
	STL::Vector<int> m_singleHorizontalRight;
	STL::Vector<int> m_singleVerticalTop;
	STL::Vector<int> m_singleVerticalCenter;
	STL::Vector<int> m_singleVerticalBottom;
	STL::Vector<int> m_top;
	STL::Vector<int> m_bottom;
	STL::Vector<int> m_left;
	STL::Vector<int> m_right;
	STL::Vector<int> m_topLeft;
	STL::Vector<int> m_topRight;
	STL::Vector<int> m_bottomLeft;
	STL::Vector<int> m_bottomRight;
	STL::Vector<int> m_insideTopLeft;
	STL::Vector<int> m_insideTopRight;
	STL::Vector<int> m_insideBottomLeft;
	STL::Vector<int> m_insideBottomRight;

private:
	int GetTileID(const STL::Vector<int>& choices) const;
};

// Autotiler over a boolean tile map, port of Monocle's Tiler. The neighbor
// state names physical directions in the engine's Y-up world, where row 0 of
// the map is the bottom row: Up reads row y + 1, Down reads row y - 1.
class Tiler
{
public:
	static STL::Vector<STL::Vector<int>> Tile(const VirtualMap<bool>& bits, STL::Func<int> tileDecider,
											  STL::Action<int> tileOutput, EdgeBehavior edges);
	// The mask also counts as solid for neighbor checks (same dimensions as
	// bits), but only bits cells produce tiles
	static STL::Vector<STL::Vector<int>> Tile(const VirtualMap<bool>& bits, const VirtualMap<bool>& mask,
											  STL::Func<int> tileDecider, STL::Action<int> tileOutput,
											  EdgeBehavior edges);
	static STL::Vector<STL::Vector<int>> Tile(const VirtualMap<bool>& bits, const AutotileData& autotileData,
											  STL::Action<int> tileOutput, EdgeBehavior edges);
	static STL::Vector<STL::Vector<int>> Tile(const VirtualMap<bool>& bits, const VirtualMap<bool>& mask,
											  const AutotileData& autotileData, STL::Action<int> tileOutput,
											  EdgeBehavior edges);

	// Neighbor state of the tile currently being decided; read these from a
	// custom tileDecider
	static inline int TileX() { return ms_tileX; }
	static inline int TileY() { return ms_tileY; }
	static inline bool Left() { return ms_left; }
	static inline bool Right() { return ms_right; }
	static inline bool Up() { return ms_up; }
	static inline bool Down() { return ms_down; }
	static inline bool UpLeft() { return ms_upLeft; }
	static inline bool UpRight() { return ms_upRight; }
	static inline bool DownLeft() { return ms_downLeft; }
	static inline bool DownRight() { return ms_downRight; }

private:
	static STL::Vector<STL::Vector<int>> TileImpl(const VirtualMap<bool>& bits, const VirtualMap<bool>* mask,
												  STL::Func<int> tileDecider, STL::Action<int> tileOutput,
												  EdgeBehavior edges);
	static void FillNeighbors(const VirtualMap<bool>& bits, const VirtualMap<bool>* mask, int tileX, int tileY,
							  EdgeBehavior edges);

	static int ms_tileX;
	static int ms_tileY;
	static bool ms_left;
	static bool ms_right;
	static bool ms_up;
	static bool ms_down;
	static bool ms_upLeft;
	static bool ms_upRight;
	static bool ms_downLeft;
	static bool ms_downRight;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_TILER_H_
