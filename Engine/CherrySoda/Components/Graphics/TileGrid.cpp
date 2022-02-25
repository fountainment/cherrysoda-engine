#include <CherrySoda/Components/Graphics/TileGrid.h>

#include <CherrySoda/Entity.h>
#include <CherrySoda/Graphics/MTexture.h>
#include <CherrySoda/Graphics/TileSet.h>
#include <CherrySoda/Util/Camera.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/VirtualMap.h>

using cherrysoda::TileGrid;

using cherrysoda::Math;
using cherrysoda::STL;
using cherrysoda::TileSet;
using cherrysoda::MTexture;
using cherrysoda::VirtualMap;

void TileGrid::Populate(const TileSet* tileset, const STL::Vector<STL::Vector<int>>& tiles, int offsetX/* = 0*/, int offsetY/* = 0*/)
{
	if (STL::IsEmpty(tiles)) return;
	for (int x = 0; x < static_cast<int>(STL::Count(tiles)) && x + offsetX < TilesX(); ++x)
		for (int y = 0; y < static_cast<int>(STL::Count(tiles[0])) && y + offsetY < TilesY(); ++y)
			m_tiles->Set(x + offsetX, y + offsetY, tileset->Get(tiles[x][y]));
}

void TileGrid::FillRect(int x, int y, int columns, int rows, const MTexture* tile)
{
	int left = Math_Max(0, x);
	int bottom = Math_Max(0, y);
	int right = Math_Min(TilesX(), x + columns);
	int top = Math_Min(TilesY(), y + rows);

	for (int tx = left; tx < right; tx++)
		for (int ty = bottom; ty < top; ty++)
			m_tiles->Set(tx, ty, tile);
}

void TileGrid::Clear()
{
	for (int tx = 0; tx < TilesX(); ++tx)
		for (int ty = 0; ty < TilesY(); ++ty)
			m_tiles->Set(tx, ty, nullptr);
}

Math::Rectangle TileGrid::GetClippedRenderTiles() const
{
	auto pos = GetEntity()->Position2D() + Position();

	int left, top, right, bottom;
	if (m_clipCamera == nullptr) {
		left = -m_visualExtend;
		bottom = -m_visualExtend;
		right = TilesX() + m_visualExtend;
		top = TilesY() + m_visualExtend;
	}
	else {
		auto camera = m_clipCamera;
		left = (int)Math_Max(0.f, Math_Floor((camera->Left() - pos.x) / TileWidth()) - m_visualExtend);
		bottom = (int)Math_Max(0.f, Math_Floor((camera->Bottom() - pos.y) / TileHeight()) - m_visualExtend);
		right = (int)Math_Min((float)TilesX(), Math_Ceiling((camera->Right() - pos.x) / TileWidth()) + m_visualExtend);
		top = (int)Math_Min((float)TilesY(), Math_Ceiling((camera->Top() - pos.y) / TileHeight()) + m_visualExtend);
	}

	// clamp
	left = Math_Max(left, 0);
	bottom = Math_Max(bottom, 0);
	right = Math_Min(right, TilesX());
	top = Math_Min(top, TilesY());

	return Math::Rectangle{ Math::Vec2(left, bottom), Math::Vec2(right - left, top - bottom) };
}

void TileGrid::Render()
{
	RenderAt(GetEntity()->Position2D() + Position());
}

void TileGrid::RenderAt(const Math::Vec2& position)
{
	if (m_alpha <= 0.f) return;
	
	auto clip = GetClippedRenderTiles();
	auto color = m_color * m_alpha;

	for (int tx = clip.Left(); tx < clip.Right(); ++tx) {
		for (int ty = clip.Bottom(); ty < clip.Top(); ++ty) {
			const MTexture* tile = m_tiles->Get(tx, ty);
			if (tile != nullptr) {
				tile->Draw(Math::Vec3(m_position + Math::Vec2(tx * TileWidth(), ty * TileHeight()), 0.f), Vec3_Zero, color);
			}
		}
	}
}
