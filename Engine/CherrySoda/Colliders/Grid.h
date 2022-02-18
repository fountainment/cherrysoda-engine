#ifndef _CHERRYSODA_COLLIDERS_GRID_H_
#define _CHERRYSODA_COLLIDERS_GRID_H_

#include <CherrySoda/Colliders/Collider.h>
#include <CherrySoda/Util/VirtualMap.h>

namespace cherrysoda {

class Grid : public Collider
{
public:
	CHERRYSODA_DECLARE_COLLIDER(Grid, Collider);

	Grid(int cellsX, int cellsY, float cellWidth, float cellHeight)
		: m_cellWidth(cellWidth)
		, m_cellHeight(cellHeight)
	{
		m_data = new VirtualMap<bool>(cellsX, cellsY);
	}

	~Grid()
	{
		delete m_data;
		m_data = nullptr;
	}

	void SetRect(int x, int y, int width, int height, bool to = true);
	bool CheckRect(int x, int y, int width, int height) const;

	bool Collide(const Circle* circle) const override;
	bool Collide(const ColliderList* list) const override;
	bool Collide(const Hitbox* hitbox) const override;
	bool Collide(const Grid* grid) const override;
	bool Collide(const Math::Vec2& point) const override;
	bool Collide(const Math::Rectangle& rect) const override;
	bool Collide(const Math::Vec2& from, const Math::Vec2& to) const override;
	void Render(const Camera* camera, const Color& color) const override;

	float Left() const override { return InternalLeft(); }
	float Right() const override { return InternalRight(); }
	float Bottom() const override { return InternalBottom(); }
	float Top() const override { return InternalTop(); }

	float Width() const override { return InternalWidth(); }
	float Height() const override { return InternalHeight(); }

	bool Get(int x, int y) { if (x >= 0 && y >= 0 && x < CellsX() && y < CellsY()) return m_data->Get(x, y); else return false; }
	void Set(int x, int y, bool v) { m_data->Set(x, y, v); }

	inline float CellWidth() const { return m_cellWidth; }
	inline float CellHeight() const { return m_cellHeight; }
	inline int CellsX() const { return m_data->Columns(); }
	inline int CellsY() const { return m_data->Rows(); }

private:
	inline float InternalLeft() const { return PositionX(); }
	inline float InternalRight() const { return PositionX() + InternalWidth(); }
	inline float InternalBottom() const { return PositionY(); }
	inline float InternalTop() const { return PositionY() + InternalHeight(); }

	inline float InternalWidth() const { return CellWidth() * CellsX(); }
	inline float InternalHeight() const { return CellHeight() * CellsY(); }

	int m_cellWidth;
	int m_cellHeight;
	VirtualMap<bool>* m_data = nullptr;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COLLIDERS_GRID_H_
