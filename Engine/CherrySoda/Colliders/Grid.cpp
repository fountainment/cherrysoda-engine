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

namespace cherrysoda {

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

// Liang-Barsky clip of segment [from, to] against rect; returns false if the segment misses the rect
static bool ClipSegmentToRect(const Math::Rectangle& rect, const Math::Vec2& from, const Math::Vec2& to,
                              Math::Vec2& start, Math::Vec2& end)
{
	float t0 = 0.f;
	float t1 = 1.f;
	const Math::Vec2 delta = to - from;
	const float p[] = { -delta.x, delta.x, -delta.y, delta.y };
	const float q[] = {
		from.x - rect.Left(),  rect.Right() - from.x,
		from.y - rect.Bottom(), rect.Top()  - from.y,
	};
	for (int i = 0; i < 4; ++i) {
		if (p[i] == 0.f) {
			if (q[i] < 0.f) {
				return false;
			}
		}
		else {
			float r = q[i] / p[i];
			if (p[i] < 0.f) {
				if (r > t1) return false;
				if (r > t0) t0 = r;
			}
			else {
				if (r < t0) return false;
				if (r < t1) t1 = r;
			}
		}
	}
	start = from + delta * t0;
	end = from + delta * t1;
	return true;
}

bool Grid::Collide(const Math::Vec2& from, const Math::Vec2& to) const
{
	if (CellWidth() <= 0.f || CellHeight() <= 0.f || CellsX() <= 0 || CellsY() <= 0) {
		return false;
	}

	const Math::Rectangle bounds = Bounds();
	Math::Vec2 start, end;
	if (!ClipSegmentToRect(bounds, from, to, start, end)) {
		return false;
	}

	// Amanatides-Woo grid traversal, in cell units
	const float startX = (start.x - bounds.Left()) / CellWidth();
	const float startY = (start.y - bounds.Bottom()) / CellHeight();
	const float dirX = (end.x - start.x) / CellWidth();
	const float dirY = (end.y - start.y) / CellHeight();

	constexpr float kInf = 1e30f;
	const int stepX = dirX > 0.f ? 1 : (dirX < 0.f ? -1 : 0);
	const int stepY = dirY > 0.f ? 1 : (dirY < 0.f ? -1 : 0);
	const float tDeltaX = stepX != 0 ? Math_Abs(1.f / dirX) : kInf;
	const float tDeltaY = stepY != 0 ? Math_Abs(1.f / dirY) : kInf;

	int x = static_cast<int>(Math_Floor(startX));
	int y = static_cast<int>(Math_Floor(startY));
	const int endX = static_cast<int>(Math_Floor(startX + dirX));
	const int endY = static_cast<int>(Math_Floor(startY + dirY));
	float tMaxX = stepX > 0 ? (static_cast<float>(x + 1) - startX) * tDeltaX
	          : stepX < 0 ? (startX - static_cast<float>(x)) * tDeltaX : kInf;
	float tMaxY = stepY > 0 ? (static_cast<float>(y + 1) - startY) * tDeltaY
	          : stepY < 0 ? (startY - static_cast<float>(y)) * tDeltaY : kInf;

	while (true) {
		if (Get(x, y)) {
			return true;
		}
		if (x == endX && y == endY) {
			return false;
		}
		if (tMaxX < tMaxY) {
			x += stepX;
			tMaxX += tDeltaX;
		}
		else {
			y += stepY;
			tMaxY += tDeltaY;
		}
	}
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

} // namespace cherrysoda
