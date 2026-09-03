#include <CherrySoda/Util/Tiler.h>

#include <CherrySoda/Util/Calc.h>
#include <CherrySoda/Util/Json.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

int Tiler::ms_tileX = 0;
int Tiler::ms_tileY = 0;
bool Tiler::ms_left = false;
bool Tiler::ms_right = false;
bool Tiler::ms_up = false;
bool Tiler::ms_down = false;
bool Tiler::ms_upLeft = false;
bool Tiler::ms_upRight = false;
bool Tiler::ms_downLeft = false;
bool Tiler::ms_downRight = false;

AutotileData::AutotileData(const json::Value& json)
{
	auto read = [&json](const char* key) {
		STL::Vector<int> values;
		if (json.IsObject() && json.HasMember(key) && json[key].IsArray()) {
			for (const auto& value : json[key].GetArray()) {
				if (value.IsInt()) {
					STL::Add(values, value.GetInt());
				}
			}
		}
		return values;
	};

	m_center = read("Center");
	m_single = read("Single");

	m_singleHorizontalLeft = read("SingleHorizontalLeft");
	m_singleHorizontalCenter = read("SingleHorizontalCenter");
	m_singleHorizontalRight = read("SingleHorizontalRight");

	m_singleVerticalTop = read("SingleVerticalTop");
	m_singleVerticalCenter = read("SingleVerticalCenter");
	m_singleVerticalBottom = read("SingleVerticalBottom");

	m_top = read("Top");
	m_bottom = read("Bottom");
	m_left = read("Left");
	m_right = read("Right");

	m_topLeft = read("TopLeft");
	m_topRight = read("TopRight");
	m_bottomLeft = read("BottomLeft");
	m_bottomRight = read("BottomRight");

	m_insideTopLeft = read("InsideTopLeft");
	m_insideTopRight = read("InsideTopRight");
	m_insideBottomLeft = read("InsideBottomLeft");
	m_insideBottomRight = read("InsideBottomRight");
}

int AutotileData::TileHandler() const
{
	if (Tiler::Left() && Tiler::Right() && Tiler::Up() && Tiler::Down() && Tiler::UpLeft() && Tiler::UpRight() &&
		Tiler::DownLeft() && Tiler::DownRight()) {
		return GetTileID(m_center);
	}

	else if (!Tiler::Up() && !Tiler::Down()) {
		if (Tiler::Left() && Tiler::Right()) {
			return GetTileID(m_singleHorizontalCenter);
		}
		else if (!Tiler::Left() && !Tiler::Right()) {
			return GetTileID(m_single);
		}
		else if (Tiler::Left()) {
			return GetTileID(m_singleHorizontalRight);
		}
		else {
			return GetTileID(m_singleHorizontalLeft);
		}
	}
	else if (!Tiler::Left() && !Tiler::Right()) {
		if (Tiler::Up() && Tiler::Down()) {
			return GetTileID(m_singleVerticalCenter);
		}
		else if (Tiler::Down()) {
			return GetTileID(m_singleVerticalTop);
		}
		else {
			return GetTileID(m_singleVerticalBottom);
		}
	}

	else if (Tiler::Up() && Tiler::Down() && Tiler::Left() && !Tiler::Right()) {
		return GetTileID(m_right);
	}
	else if (Tiler::Up() && Tiler::Down() && !Tiler::Left() && Tiler::Right()) {
		return GetTileID(m_left);
	}

	else if (Tiler::Up() && !Tiler::Left() && Tiler::Right() && !Tiler::Down()) {
		return GetTileID(m_bottomLeft);
	}
	else if (Tiler::Up() && Tiler::Left() && !Tiler::Right() && !Tiler::Down()) {
		return GetTileID(m_bottomRight);
	}
	else if (Tiler::Down() && Tiler::Right() && !Tiler::Left() && !Tiler::Up()) {
		return GetTileID(m_topLeft);
	}
	else if (Tiler::Down() && !Tiler::Right() && Tiler::Left() && !Tiler::Up()) {
		return GetTileID(m_topRight);
	}

	else if (Tiler::Up() && Tiler::Down() && !Tiler::DownRight() && Tiler::DownLeft()) {
		return GetTileID(m_insideTopLeft);
	}
	else if (Tiler::Up() && Tiler::Down() && Tiler::DownRight() && !Tiler::DownLeft()) {
		return GetTileID(m_insideTopRight);
	}
	else if (Tiler::Up() && Tiler::Down() && Tiler::UpLeft() && !Tiler::UpRight()) {
		return GetTileID(m_insideBottomLeft);
	}
	else if (Tiler::Up() && Tiler::Down() && !Tiler::UpLeft() && Tiler::UpRight()) {
		return GetTileID(m_insideBottomRight);
	}

	else if (!Tiler::Down()) {
		return GetTileID(m_bottom);
	}
	else {
		return GetTileID(m_top);
	}
}

int AutotileData::GetTileID(const STL::Vector<int>& choices) const
{
	if (STL::IsEmpty(choices)) {
		return -1;
	}
	else if (STL::Count(choices) == 1) {
		return STL::Front(choices);
	}
	else {
		return Calc::GetRandom()->Choose(choices);
	}
}

STL::Vector<STL::Vector<int>> Tiler::Tile(const VirtualMap<bool>& bits, STL::Func<int> tileDecider,
										  STL::Action<int> tileOutput, EdgeBehavior edges)
{
	return TileImpl(bits, nullptr, std::move(tileDecider), std::move(tileOutput), edges);
}

STL::Vector<STL::Vector<int>> Tiler::Tile(const VirtualMap<bool>& bits, const VirtualMap<bool>& mask,
										  STL::Func<int> tileDecider, STL::Action<int> tileOutput, EdgeBehavior edges)
{
	return TileImpl(bits, &mask, std::move(tileDecider), std::move(tileOutput), edges);
}

STL::Vector<STL::Vector<int>> Tiler::Tile(const VirtualMap<bool>& bits, const AutotileData& autotileData,
										  STL::Action<int> tileOutput, EdgeBehavior edges)
{
	return TileImpl(
		bits, nullptr, [&autotileData]() { return autotileData.TileHandler(); }, std::move(tileOutput), edges);
}

STL::Vector<STL::Vector<int>> Tiler::Tile(const VirtualMap<bool>& bits, const VirtualMap<bool>& mask,
										  const AutotileData& autotileData, STL::Action<int> tileOutput,
										  EdgeBehavior edges)
{
	return TileImpl(
		bits, &mask, [&autotileData]() { return autotileData.TileHandler(); }, std::move(tileOutput), edges);
}

STL::Vector<STL::Vector<int>> Tiler::TileImpl(const VirtualMap<bool>& bits, const VirtualMap<bool>* mask,
											  STL::Func<int> tileDecider, STL::Action<int> tileOutput,
											  EdgeBehavior edges)
{
	int boundsX = bits.Columns();
	int boundsY = bits.Rows();

	// Non-solid cells keep -1 so the result feeds straight into
	// TileGrid::Overlay, which skips negative ids
	STL::Vector<STL::Vector<int>> tiles;
	STL::Resize(tiles, boundsX);
	for (int x = 0; x < boundsX; ++x) {
		STL::Resize(tiles[x], boundsY);
		STL::Fill(tiles[x], -1);
	}

	for (int x = 0; x < boundsX; ++x) {
		for (int y = 0; y < boundsY; ++y) {
			if (bits.Get(x, y)) {
				ms_tileX = x;
				ms_tileY = y;
				FillNeighbors(bits, mask, x, y, edges);

				int tile = tileDecider != nullptr ? tileDecider() : -1;
				if (tileOutput != nullptr) {
					tileOutput(tile);
				}
				tiles[x][y] = tile;
			}
		}
	}

	return tiles;
}

void Tiler::FillNeighbors(const VirtualMap<bool>& bits, const VirtualMap<bool>* mask, int tileX, int tileY,
						  EdgeBehavior edges)
{
	int boundsX = bits.Columns();
	int boundsY = bits.Rows();

	auto at = [&](int x, int y) { return bits.Get(x, y) || (mask != nullptr && mask->Get(x, y)); };

	switch (edges) {
	case EdgeBehavior::True:
		ms_left = tileX == 0 ? true : at(tileX - 1, tileY);
		ms_right = tileX == boundsX - 1 ? true : at(tileX + 1, tileY);
		ms_down = tileY == 0 ? true : at(tileX, tileY - 1);
		ms_up = tileY == boundsY - 1 ? true : at(tileX, tileY + 1);

		ms_downLeft = (tileX == 0 || tileY == 0) ? true : at(tileX - 1, tileY - 1);
		ms_downRight = (tileX == boundsX - 1 || tileY == 0) ? true : at(tileX + 1, tileY - 1);
		ms_upLeft = (tileX == 0 || tileY == boundsY - 1) ? true : at(tileX - 1, tileY + 1);
		ms_upRight = (tileX == boundsX - 1 || tileY == boundsY - 1) ? true : at(tileX + 1, tileY + 1);
		break;

	case EdgeBehavior::Wrap:
		ms_left = at((tileX + boundsX - 1) % boundsX, tileY);
		ms_right = at((tileX + 1) % boundsX, tileY);
		ms_down = at(tileX, (tileY + boundsY - 1) % boundsY);
		ms_up = at(tileX, (tileY + 1) % boundsY);

		ms_downLeft = at((tileX + boundsX - 1) % boundsX, (tileY + boundsY - 1) % boundsY);
		ms_downRight = at((tileX + 1) % boundsX, (tileY + boundsY - 1) % boundsY);
		ms_upLeft = at((tileX + boundsX - 1) % boundsX, (tileY + 1) % boundsY);
		ms_upRight = at((tileX + 1) % boundsX, (tileY + 1) % boundsY);
		break;

	case EdgeBehavior::False:
	default:
		ms_left = tileX == 0 ? false : at(tileX - 1, tileY);
		ms_right = tileX == boundsX - 1 ? false : at(tileX + 1, tileY);
		ms_down = tileY == 0 ? false : at(tileX, tileY - 1);
		ms_up = tileY == boundsY - 1 ? false : at(tileX, tileY + 1);

		ms_downLeft = (tileX == 0 || tileY == 0) ? false : at(tileX - 1, tileY - 1);
		ms_downRight = (tileX == boundsX - 1 || tileY == 0) ? false : at(tileX + 1, tileY - 1);
		ms_upLeft = (tileX == 0 || tileY == boundsY - 1) ? false : at(tileX - 1, tileY + 1);
		ms_upRight = (tileX == boundsX - 1 || tileY == boundsY - 1) ? false : at(tileX + 1, tileY + 1);
		break;
	}
}

} // namespace cherrysoda
