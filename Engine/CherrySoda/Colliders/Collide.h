#ifndef _CHERRYSODA_COLLIDERS_COLLIDE_H_
#define _CHERRYSODA_COLLIDERS_COLLIDE_H_

#include <CherrySoda/Util/Calc.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class CollidableComponent;
class Entity;

enum class PointSectors { Center = 0, Top = 1, Bottom = 2, TopLeft = 9, TopRight = 5, Left = 8, Right = 4, BottomLeft = 10, BottomRight = 6 };
CHERRYSODA_DECLARE_ENUM_FLAG(PointSectors);

class Collide
{
public:
	static bool Check(const Entity* a, const Entity* b);
	static bool Check(Entity* a, const Entity* b, const Math::Vec2& at);
	static bool Check(const Entity* a, const CollidableComponent* b);
	static bool Check(Entity* a, const CollidableComponent* b, const Math::Vec2& at);

	static bool Check(const Entity* a, const STL::List<Entity*>& b);
	static bool Check(Entity* a, const STL::List<Entity*>& b, const Math::Vec2& at);

	static bool CheckPoint(const Entity* a, const Math::Vec2& point);
	static bool CheckLine(const Entity* a, const Math::Vec2& from, const Math::Vec2& to);

	static int Count(const Entity* a, const STL::List<Entity*>& b);

	static Entity* First(const Entity* a, const STL::List<Entity*>& b);

	static const STL::List<Entity*> All(const Entity* a, const STL::List<Entity*>& b);

	// Line
	static bool LineCheck(const Math::Vec2& a1, const Math::Vec2& a2, const Math::Vec2& b1, const Math::Vec2& b2);
	static bool LineCheck(const Math::Vec2& a1, const Math::Vec2& a2, const Math::Vec2& b1, const Math::Vec2& b2, Math::Vec2& intersection);

	// Circle
	static inline bool CircleToLine(const Math::Vec2& cPosition, float cRadius, const Math::Vec2& lineFrom, const Math::Vec2& lineTo)
	{
		return Math_LengthSq(cPosition - Calc::ClosestPointOnLine(lineFrom, lineTo, cPosition)) < cRadius * cRadius;
	}

	// Rect
	static bool RectToCircle(float rX, float rY, float rW, float rH, const Math::Vec2& cPosition, float cRadius);
	static inline bool RectToCircle(const Math::Rectangle& rect, const Math::Vec2& cPosition, float cRadius)
	{
		return RectToCircle(rect.X(), rect.Y(), rect.Width(), rect.Height(), cPosition, cRadius);
	}

	static bool RectToLine(float rX, float rY, float rW, float rH, const Math::Vec2& lineFrom, const Math::Vec2& lineTo);
	static inline bool RectToLine(const Math::Rectangle& rect, const Math::Vec2& lineFrom, const Math::Vec2& lineTo)
	{
		return RectToLine(rect.X(), rect.Y(), rect.Width(), rect.Height(), lineFrom, lineTo);
	}

	static inline bool RectToPoint(float rX, float rY, float rW, float rH, const Math::Vec2& point)
	{
		return point.x >= rX && point.y >= rY && point.x < rX + rW && point.y < rY + rH;
	}
	static inline bool RectToPoint(const Math::Rectangle& rect, const Math::Vec2& point)
	{
		return RectToPoint(rect.X(), rect.Y(), rect.Width(), rect.Height(), point);
	}

	// Sectors
	static PointSectors GetSector(float rX, float rY, float rW, float rH, const Math::Vec2& point);
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COLLIDERS_COLLIDE_H_
