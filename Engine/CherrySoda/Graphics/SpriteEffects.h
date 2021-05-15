#ifndef _CHERRYSODA_GRAPHICS_SPRITEEFFECTS_H_
#define _CHERRYSODA_GRAPHICS_SPRITEEFFECTS_H_

#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>

namespace cherrysoda {

enum class SpriteEffects : type::UInt8
{
	None = 0,
	FlipHorizontally = 1,
	FlipVertically = 2,
	FlipHV = 3,
	RoundRenderingPosition = 4,
	RoundFlipH = 5,
	RoundFlipV = 6,
	RoundFlipHV = 7
};

CHERRYSODA_DECLARE_ENUM_FLAG(SpriteEffects);

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_SPRITEEFFECTS_H_
