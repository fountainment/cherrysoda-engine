#ifndef _CHERRYSODA_UTIL_DRAW_H_
#define _CHERRYSODA_UTIL_DRAW_H_

#include <CherrySoda/Graphics/MTexture.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>

namespace cherrysoda {

class Renderer;
class SpriteBatch;

class Draw
{
public:
	static void Initialize();
	static void Terminate();

	static void UseDebugPixelTexture();
	static void DestroyDebugPixelTexture();

	static inline Renderer* GetRenderer() { return ms_renderer; }
	static inline void SetRenderer(Renderer* renderer) { ms_renderer = renderer; } 

	static inline SpriteBatch* GetSpriteBatch() { return ms_spriteBatch; }

	static void LineAngle(const Math::Vec2& start, float angle, float length, const Color& color = Color::White, float thickness = 1.f);
	static void Line(const Math::Vec2& start, const Math::Vec2& end, const Color& color = Color::White, float thickness = 1.f);

	static void HollowRect(float x, float y, float width, float height, const Color& color = Color::White);
	static void Circle(const Math::Vec2& pos, float radius, const Color& color = Color::White, float resolution = 4.f);

	static const MTexture& PixelTexture() { return ms_pixel; }
	static const MTexture& ParticleTexture() { return ms_particle; }
	static void PixelTexture(const MTexture& texture) { ms_pixel = texture; }
	static void ParticleTexture(const MTexture& texture) { ms_particle = texture; }

private:
	static MTexture ms_debugPixelTexture;
	static MTexture ms_pixel;
	static MTexture ms_particle;
	static Renderer* ms_renderer;
	static SpriteBatch* ms_spriteBatch;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_DRAW_H_
