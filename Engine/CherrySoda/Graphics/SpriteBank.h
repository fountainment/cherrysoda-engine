#ifndef _CHERRYSODA_GRAPHICS_SPRITEBANK_H_
#define _CHERRYSODA_GRAPHICS_SPRITEBANK_H_

#include <CherrySoda/Graphics/SpriteData.h>
#include <CherrySoda/Util/Json.h>
#include <CherrySoda/Util/String.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class Atlas;
class Sprite;

class SpriteBank
{
public:
	// SpriteBank(Atlas* atlas, json::Document json);
	// SpriteBank(Atlas* atlas, String jsonPath)
	// : SpriteBank(atlas, JsonUtil::ReadJsonFile(jsonPath)) {}

	bool Has(const StringID& id);
	Sprite* Create(const StringID& id);
	Sprite* CreateOn(Sprite* sprite, const StringID& id);

private:
	Atlas* m_atlas;
	json::Document m_json;
	STL::HashMap<StringID, SpriteData> m_spriteData;	
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_SPRITEBANK_H_
