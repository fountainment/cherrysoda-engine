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
		: base(false, true)
		, m_tileWidth(tileWidth)
		, m_tileHeight(tileHeight)
	{
		m_tiles = new VirtualMap<const MTexture*>(tilesX, tilesY);
	}

	~TileGrid() { delete m_tiles; m_tiles = nullptr; }

	inline int TileWidth() const { return m_tileWidth; }
	inline int TileHeight() const { return m_tileHeight; }
	inline int TilesX() const { return m_tiles->Columns(); }
	inline int TilesY() const { return m_tiles->Rows(); }

	void Populate(const TileSet* tileset, const STL::Vector<STL::Vector<int>>& tiles, int offsetX = 0, int offsetY = 0);
	void FillRect(int x, int y, int columns, int rows, const MTexture* tile);
	void Clear();

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
