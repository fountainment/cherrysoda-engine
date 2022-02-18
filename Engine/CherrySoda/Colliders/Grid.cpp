#include <CherrySoda/Colliders/Grid.h>

#include <CherrySoda/Colliders/Collide.h>
#include <CherrySoda/Colliders/ColliderList.h>
#include <CherrySoda/Colliders/Circle.h>
#include <CherrySoda/Colliders/Hitbox.h>
#include <CherrySoda/Util/Camera.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Draw.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/Math.h>

using cherrysoda::Grid;

using cherrysoda::Camera;
using cherrysoda::Circle;
using cherrysoda::Collide;
using cherrysoda::ColliderList;
using cherrysoda::Color;
using cherrysoda::Draw;
using cherrysoda::Hitbox;
using cherrysoda::Math;

void Grid::SetRect(int x, int y, int width, int height, bool to/* = true*/)
{
	if (x < 0) {
		width += x;
		x = 0;
	}

	if (y < 0) {
		height += y;
		y = 0;
	}

	if (x + width > CellsX()) {
		width = CellsX() - x;
	}

	if (y + height > CellsY()) {
		height = CellsY() - y;
	}

	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j)
			m_data->Set(x + i, y + j, to);
}

bool Grid::CheckRect(int x, int y, int width, int height) const
{
	if (x < 0) {
		width += x;
		x = 0;
	}

	if (y < 0) {
		height += y;
		y = 0;
	}

	if (x + width > CellsX()) {
		width = CellsX() - x;
	}

	if (y + height > CellsY()) {
		height = CellsY() - y;
	}

	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			if (m_data->Get(x + i, y + j))
				return true;
		}
	}

	return false;
}

bool Grid::Collide(const Circle* circle) const
{
	CHERRYSODA_DEBUG("Grid-Circle collision is not implemented!\n");
	return false;
}

bool Grid::Collide(const ColliderList* list) const
{
	return list->Collide(this);
}

bool Grid::Collide(const Hitbox* hitbox) const
{
	return Collide(hitbox->Bounds());
}

bool Grid::Collide(const Grid* grid) const
{
	CHERRYSODA_DEBUG("Grid-Grid collision is not implemented!\n");
	return false;
}

bool Grid::Collide(const Math::Vec2& point) const
{
	if (point.x >= AbsoluteLeft() && point.y >= AbsoluteBottom() && point.x < AbsoluteRight() && point.y < AbsoluteTop())
		return m_data->Get((int)((point.x - AbsoluteLeft()) / CellWidth()), (int)((point.y - AbsoluteBottom()) / CellHeight()));
	else
		return false;
}

bool Grid::Collide(const Math::Rectangle& rect) const
{
	if (rect.Intersects(Bounds())) {
		int x = (int)((rect.Left() - AbsoluteLeft()) / CellWidth());
		int y = (int)((rect.Bottom() - AbsoluteBottom()) / CellHeight());
		int w = (int)((rect.Right() - AbsoluteLeft() - 1) / CellWidth()) - x + 1;
		int h = (int)((rect.Top() - AbsoluteBottom() - 1) / CellHeight()) - y + 1;	

		return CheckRect(x, y, w, h);
	}
	else {
		return false;
	}
}

bool Grid::Collide(const Math::Vec2& from, const Math::Vec2& to) const
{
	return false;
}

void Grid::Render(const Camera* camera, const Color& color) const
{
	if (camera == nullptr) {
		for (int i = 0; i < CellsX(); ++i)
			for (int j = 0; j < CellsY(); ++j)
				if (m_data->Get(i, j))
					Draw::HollowRect(AbsoluteLeft() + i * CellWidth(), AbsoluteBottom() + j * CellHeight(), CellWidth(), CellHeight(), color);
	}
	else {
		int left = (int)Math_Max(0.f, ((camera->Left() - AbsoluteLeft()) / CellWidth()));
		int right = (int)Math_Min(CellsX() - 1.f, Math_Ceiling((camera->Right() - AbsoluteLeft()) / CellWidth()));
		int bottom = (int)Math_Max(0.f, ((camera->Bottom() - AbsoluteBottom()) / CellHeight()));
		int top = (int)Math_Min(CellsY() - 1.f, Math_Ceiling((camera->Top() - AbsoluteBottom()) / CellHeight()));

		for (int tx = left; tx <= right; ++tx)
			for (int ty = bottom; ty <= top; ++ty)
				if (m_data->Get(tx, ty))
					Draw::HollowRect(AbsoluteLeft() + tx * CellWidth(), AbsoluteBottom() + ty * CellHeight(), CellWidth(), CellHeight(), color);
	}
}
