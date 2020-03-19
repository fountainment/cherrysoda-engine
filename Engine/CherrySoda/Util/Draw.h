#ifndef _CHERRYSODA_UTIL_DRAW_H_
#define _CHERRYSODA_UTIL_DRAW_H_

namespace cherrysoda {

class Renderer;

class Draw
{
public:
	static inline Renderer* GetRenderer() { return ms_renderer; }
	static inline void SetRenderer(Renderer* renderer) { ms_renderer = renderer; } 

private:
	static Renderer* ms_renderer;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_DRAW_H_