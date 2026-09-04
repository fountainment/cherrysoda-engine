#ifndef _CHERRYSODA_COMPONENTS_GRAPHICS_TILEGRID_H_
#define _CHERRYSODA_COMPONENTS_GRAPHICS_TILEGRID_H_

#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/VirtualMap.h>

namespace cherrysoda {

class Camera;
class MTexture;
class TileSet;

class TileGrid : public Component
{
public:
	CHERRYSODA_DECLARE_COMPONENT(TileGrid, Component);

	CHERRYSODA_GETTER_SETTER_OF_VEC2(Position, m_position);

	TileGrid(int tileWidth, int tileHeight, int tilesX, int tilesY)
		: base(false, true), m_tileWidth(tileWidth), m_tileHeight(tileHeight)
	{
		m_tiles = new VirtualMap<const MTexture*>(tilesX, tilesY);
	}

	~TileGrid() override
	{
		delete m_tiles;
		m_tiles = nullptr;
	}

	inline int TileWidth() const { return m_tileWidth; }
	inline int TileHeight() const { return m_tileHeight; }
	inline int TilesX() const { return m_tiles->Columns(); }
	inline int TilesY() const { return m_tiles->Rows(); }

	const MTexture* Get(int x, int y) const
	{
		if (x >= 0 && y >= 0 && x < TilesX() && y < TilesY()) return m_tiles->Get(x, y);
		return nullptr;
	}
	void Set(int x, int y, const MTexture* tile) { m_tiles->Set(x, y, tile); }

	void Populate(const TileSet* tileset, const STL::Vector<STL::Vector<int>>& tiles, int offsetX = 0, int offsetY = 0);
	// Like Populate, but tiles with a negative index are skipped instead of
	// clearing the cell
	void Overlay(const TileSet* tileset, const STL::Vector<STL::Vector<int>>& tiles, int offsetX = 0, int offsetY = 0);
	void FillRect(int x, int y, int columns, int rows, const MTexture* tile);
	// Grows the tile map on each side, shifting Position to compensate; the
	// new border cells repeat the old edge tiles
	void Extend(int left, int right, int up, int down);
	void Clear();

	CHERRYSODA_GETTER_SETTER_OF_TYPE(float, Alpha, m_alpha);
	CHERRYSODA_GETTER_SETTER_OF_TYPE(int, VisualExtend, m_visualExtend);
	inline Color GetColor() const { return m_color; }
	inline void SetColor(const Color& color) { m_color = color; }

	inline void ClipCamera(Camera* camera) { m_clipCamera = camera; }

	Math::Rectangle GetClippedRenderTiles() const;

	void Render() override;
	void RenderAt(const Math::Vec2& position);

private:
	Math::Vec2 m_position = Vec2_Zero;
	Color m_color = Color::White;
	int m_visualExtend = 0;
	VirtualMap<const MTexture*>* m_tiles = nullptr;
	Camera* m_clipCamera = nullptr;
	float m_alpha = 1.f;

	int m_tileWidth;
	int m_tileHeight;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_GRAPHICS_TILEGRID_H_
