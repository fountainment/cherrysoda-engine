#ifndef _CHERRYSODA_GRAPHICS_SPRITEDATA_H_
#define _CHERRYSODA_GRAPHICS_SPRITEDATA_H_

#include <CherrySoda/Util/Json.h>
#include <CherrySoda/Util/String.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class Atlas;
class Sprite;

struct SpriteDataSource
{
	const json::Value* m_json;
	String m_path;
	String m_overridePath;
};

class SpriteData
{
public:
	SpriteData() = default;
	SpriteData(Atlas* atlas);

	~SpriteData();

	void Add(const json::Value* jsonValue, const String& overridePath = "");

	Sprite* Create();
	Sprite* CreateOn(Sprite* clone);

private:
	Atlas* m_atlas = nullptr;
	Sprite* m_sprite = nullptr;
	STL::List<SpriteDataSource*> m_sources;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_SPRITEDATA_H_
