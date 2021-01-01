#ifndef _CHERRYSODA_GRAPHICS_SPRITEDATA_H_
#define _CHERRYSODA_GRAPHICS_SPRITEDATA_H_

#include <CherrySoda/Util/Json.h>
#include <CherrySoda/Util/String.h>

namespace cherrysoda {

class Atlas;
class Sprite;

struct SpriteDataSource
{
	json::Value m_json;
	String m_path;
	String m_overridePath;
};

class SpriteData
{
public:
	SpriteData() = default;
	SpriteData(Atlas* atlas);

	~SpriteData();

	Sprite* Create();
	Sprite* CreateOn(Sprite* clone);

private:
	Atlas* m_atlas = nullptr;
	Sprite* m_sprite = nullptr;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_SPRITEDATA_H_
