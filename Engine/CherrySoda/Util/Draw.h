#ifndef _CHERRYSODA_UTIL_DRAW_H_
#define _CHERRYSODA_UTIL_DRAW_H_

namespace cherrysoda {

class Renderer;
class SpriteBatch;

class Draw
{
public:
	static void Initialize();
	static void Terminate();

	static inline Renderer* GetRenderer() { return ms_renderer; }
	static inline void SetRenderer(Renderer* renderer) { ms_renderer = renderer; } 

	static inline SpriteBatch* GetSpriteBatch() { return ms_spriteBatch; }

private:
	static Renderer* ms_renderer;
	static SpriteBatch* ms_spriteBatch;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_DRAW_H_
