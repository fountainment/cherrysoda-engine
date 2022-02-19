#ifndef _CHERRYSODA_GRAPHICS_TILESET_H_
#define _CHERRYSODA_GRAPHICS_TILESET_H_

#include <CherrySoda/Graphics/MTexture.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class TileSet
{
public:
	TileSet(const MTexture& texture, int tileWidth, int tileHeight)
		: m_texture(texture)
		, m_tileWidth(tileWidth)
		, m_tileHeight(tileHeight)
	{
		m_tiles = STL::Vector<STL::Vector<MTexture>>(m_texture.Width() / tileWidth, STL::Vector<MTexture>(m_texture.Height() / tileHeight));
		for (int x = 0; x < m_texture.Width() / tileWidth; ++x) {
			for (int y = 0; y < m_texture.Height() / tileHeight; ++y) {
				m_tiles[x][y] = MTexture(m_texture, x * tileWidth, y * tileHeight, tileWidth, tileHeight);
			}
		}
	}

	inline int TileWidth() const { return m_tileWidth; }
	inline int TileHeight() const { return m_tileHeight; }

	inline const MTexture* Get(int x, int y) const { return &m_tiles[x][y]; }
	inline const MTexture* Get(int index) const { return index < 0 ? nullptr : &m_tiles[index % STL::Count(m_tiles)][index / STL::Count(m_tiles)]; }

private:
	MTexture m_texture;
	int m_tileWidth;
	int m_tileHeight;
	STL::Vector<STL::Vector<MTexture>> m_tiles;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_TILESET_H_
