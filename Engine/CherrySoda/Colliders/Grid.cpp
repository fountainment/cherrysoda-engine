#include <CherrySoda/Colliders/Grid.h>

#include <CherrySoda/Colliders/Circle.h>
#include <CherrySoda/Colliders/Collide.h>
#include <CherrySoda/Colliders/ColliderList.h>
#include <CherrySoda/Colliders/Hitbox.h>
#include <CherrySoda/Util/Camera.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Draw.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/Math.h>

namespace cherrysoda {

void Grid::SetRect(int x, int y, int width, int height, bool to /* = true*/)
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
			if (m_data->Get(x + i, y + j)) return true;
		}
	}

	return false;
}

void Grid::Extend(int left, int right, int up, int down)
{
	// Row 0 sits on the grid's bottom edge (the engine's Y axis points up),
	// so extending down shifts the collider like extending up does in Monocle
	Position2D(Position2D() - Math::Vec2(left * CellWidth(), down * CellHeight()));

	int newWidth = CellsX() + left + right;
	int newHeight = CellsY() + up + down;
	if (newWidth <= 0 || newHeight <= 0) {
		delete m_data;
		m_data = new VirtualMap<bool>(0, 0);
		return;
	}

	auto newData = new VirtualMap<bool>(newWidth, newHeight);

	// Center
	for (int x = 0; x < CellsX(); ++x) {
		for (int y = 0; y < CellsY(); ++y) {
			int atX = x + left;
			int atY = y + down;

			if (atX >= 0 && atX < newWidth && atY >= 0 && atY < newHeight) {
				newData->Set(atX, atY, m_data->Get(x, y));
			}
		}
	}

	// Left
	for (int x = 0; x < left; ++x) {
		for (int y = 0; y < newHeight; ++y) {
			newData->Set(x, y, m_data->Get(0, Math_Clamp(y - down, 0, CellsY() - 1)));
		}
	}

	// Right
	for (int x = newWidth - right; x < newWidth; ++x) {
		for (int y = 0; y < newHeight; ++y) {
			newData->Set(x, y, m_data->Get(CellsX() - 1, Math_Clamp(y - down, 0, CellsY() - 1)));
		}
	}

	// Down (new rows below the old bottom edge repeat the old bottom row)
	for (int y = 0; y < down; ++y) {
		for (int x = 0; x < newWidth; ++x) {
			newData->Set(x, y, m_data->Get(Math_Clamp(x - left, 0, CellsX() - 1), 0));
		}
	}

	// Up (new rows above the old top edge repeat the old top row)
	for (int y = newHeight - up; y < newHeight; ++y) {
		for (int x = 0; x < newWidth; ++x) {
			newData->Set(x, y, m_data->Get(Math_Clamp(x - left, 0, CellsX() - 1), CellsY() - 1));
		}
	}

	delete m_data;
	m_data = newData;
}

void Grid::LoadBitstring(const String& bitstring)
{
	int x = 0;
	int y = 0;

	for (char c : bitstring) {
		if (c == '\n') {
			while (x < CellsX()) {
				m_data->Set(x, y, false);
				++x;
			}

			x = 0;
			++y;

			if (y >= CellsY()) {
				return;
			}
		}
		else if (x < CellsX()) {
			m_data->Set(x, y, c != '0');
			++x;
		}
	}
}

String Grid::GetBitstring() const
{
	String bits;
	for (int y = 0; y < CellsY(); ++y) {
		if (y != 0) {
			bits += '\n';
		}

		for (int x = 0; x < CellsX(); ++x) {
			bits += m_data->Get(x, y) ? '1' : '0';
		}
	}

	return bits;
}

void Grid::Clear(bool to /* = false*/)
{
	for (int x = 0; x < CellsX(); ++x) {
		for (int y = 0; y < CellsY(); ++y) {
			m_data->Set(x, y, to);
		}
	}
}

bool Grid::CheckColumn(int x) const
{
	for (int y = 0; y < CellsY(); ++y) {
		if (!m_data->Get(x, y)) {
			return false;
		}
	}
	return true;
}

bool Grid::CheckRow(int y) const
{
	for (int x = 0; x < CellsX(); ++x) {
		if (!m_data->Get(x, y)) {
			return false;
		}
	}
	return true;
}

bool Grid::IsEmpty() const
{
	for (int x = 0; x < CellsX(); ++x) {
		for (int y = 0; y < CellsY(); ++y) {
			if (m_data->Get(x, y)) {
				return false;
			}
		}
	}
	return true;
}

bool Grid::IsBitstringEmpty(const String& bitstring)
{
	for (char c : bitstring) {
		if (c == '1') {
			return false;
		}
	}
	return true;
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
	if (point.x >= AbsoluteLeft() && point.y >= AbsoluteBottom() && point.x < AbsoluteRight() &&
		point.y < AbsoluteTop())
		return m_data->Get((int)((point.x - AbsoluteLeft()) / CellWidth()),
						   (int)((point.y - AbsoluteBottom()) / CellHeight()));
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
	const float p[] = {-delta.x, delta.x, -delta.y, delta.y};
	const float q[] = {
		from.x - rect.Left(),
		rect.Right() - from.x,
		from.y - rect.Bottom(),
		rect.Top() - from.y,
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
	float tMaxX = stepX > 0   ? (static_cast<float>(x + 1) - startX) * tDeltaX
				  : stepX < 0 ? (startX - static_cast<float>(x)) * tDeltaX
							  : kInf;
	float tMaxY = stepY > 0   ? (static_cast<float>(y + 1) - startY) * tDeltaY
				  : stepY < 0 ? (startY - static_cast<float>(y)) * tDeltaY
							  : kInf;

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
					Draw::HollowRect(AbsoluteLeft() + i * CellWidth(), AbsoluteBottom() + j * CellHeight(), CellWidth(),
									 CellHeight(), color);
	}
	else {
		int left = (int)Math_Max(0.f, ((camera->Left() - AbsoluteLeft()) / CellWidth()));
		int right = (int)Math_Min(CellsX() - 1.f, Math_Ceiling((camera->Right() - AbsoluteLeft()) / CellWidth()));
		int bottom = (int)Math_Max(0.f, ((camera->Bottom() - AbsoluteBottom()) / CellHeight()));
		int top = (int)Math_Min(CellsY() - 1.f, Math_Ceiling((camera->Top() - AbsoluteBottom()) / CellHeight()));

		for (int tx = left; tx <= right; ++tx)
			for (int ty = bottom; ty <= top; ++ty)
				if (m_data->Get(tx, ty))
					Draw::HollowRect(AbsoluteLeft() + tx * CellWidth(), AbsoluteBottom() + ty * CellHeight(),
									 CellWidth(), CellHeight(), color);
	}
}

} // namespace cherrysoda
