#include <CherrySoda/Util/Draw.h>

#include <CherrySoda/Graphics/MTexture.h>
#include <CherrySoda/Graphics/SpriteBatch.h>
#include <CherrySoda/Util/Calc.h>
#include <CherrySoda/Util/Color.h>

using cherrysoda::Draw;

using cherrysoda::Calc;
using cherrysoda::Color;
using cherrysoda::Math;
using cherrysoda::MTexture;
using cherrysoda::Renderer;
using cherrysoda::SpriteBatch;
using cherrysoda::SpriteEffects;

MTexture Draw::ms_debugPixelTexture;
MTexture Draw::ms_pixel;
MTexture Draw::ms_particle;
Renderer* Draw::ms_renderer = nullptr;
SpriteBatch* Draw::ms_spriteBatch = nullptr;

void Draw::Initialize()
{
	ms_spriteBatch = new SpriteBatch();
	UseDebugPixelTexture();
}

void Draw::Terminate()
{
	DestroyDebugPixelTexture();
	delete ms_spriteBatch;
	ms_spriteBatch = nullptr;
}

void Draw::UseDebugPixelTexture()
{
	ms_debugPixelTexture = MTexture(2, 2, Color::White);
	PixelTexture(MTexture(ms_debugPixelTexture, 0, 0, 1, 1));
	ParticleTexture(MTexture(ms_debugPixelTexture, 0, 0, 2, 2));
}

void Draw::DestroyDebugPixelTexture()
{
	Graphics::DestroyTexture(ms_debugPixelTexture.Texture().GetHandle());
}

void Draw::LineAngle(const Math::Vec2& start, float angle, float length, const Color& color/* = Color::White*/, float thickness/* = 1.f*/)
{
	GetSpriteBatch()->Draw(PixelTexture().Texture(), start, PixelTexture().ClipRect(), color, angle, IVec2_Zero, Math::Vec2(length, thickness), SpriteEffects::None, 0);
}

void Draw::Line(const Math::Vec2& start, const Math::Vec2& end, const Color& color/* = Color::White*/, float thickness/* = 1.f*/)
{
	LineAngle(start, Calc::Angle(end - start), Math_Length(end - start), color, thickness);
}

void Draw::HollowRect(float x, float y, float width, float height, const Color& color/* = Color::White*/)
{
	LineAngle(Math::Vec2(x, y), 0, width, color);
	LineAngle(Math::Vec2(x + width, y), Math::PiHalf, height, color);
	LineAngle(Math::Vec2(x + width, y + height), Math::Pi, width, color);
	LineAngle(Math::Vec2(x, y + height), Math::PiHalf * 3.f, height, color);
}

void Draw::HollowRect(const Math::Rectangle& rect, const Color& color/* = Color::White*/)
{
	HollowRect(rect.X(), rect.Y(), rect.Width(), rect.Height(), color);
}

void Draw::Circle(const Math::Vec2& position, float radius, const Color& color/* = Color::White*/, float resolution/* = 4.f*/)
{
	Math::Vec2 last = Vec2_XUp * radius;
	Math::Vec2 lastP = Calc::Perpendicular(last);
	for (int i = 1; i <= resolution; i++)
	{
		Math::Vec2 at = Calc::AngleToVector(i * Math::PiHalf / resolution, radius);
		Math::Vec2 atP = Calc::Perpendicular(at);

		Draw::Line(position + last, position + at, color);
		Draw::Line(position - last, position - at, color);
		Draw::Line(position + lastP, position + atP, color);
		Draw::Line(position - lastP, position - atP, color);

		last = at;
		lastP = atP;
	}
}
