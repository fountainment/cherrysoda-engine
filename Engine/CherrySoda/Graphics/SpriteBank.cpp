#include <CherrySoda/Graphics/SpriteBank.h>

#include <CherrySoda/Components/Graphics/Sprite.h>
#include <CherrySoda/Util/Json.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/String.h>

using cherrysoda::SpriteBank;

using cherrysoda::Sprite;
using cherrysoda::StringID;
using cherrysoda::STL;

SpriteBank::SpriteBank(Atlas* atlas, cherrysoda::json::Document json)
{
	m_atlas = atlas;
	// m_json = json;	

	STL::HashMap<StringID, cherrysoda::json::Value> values;
}

bool SpriteBank::Has(const StringID& id)
{
	return STL::ContainsKey(m_spriteData, id);
}

Sprite* SpriteBank::Create(const StringID& id)
{
	if (STL::ContainsKey(m_spriteData, id)) {
		return m_spriteData[id].Create();
	}
	else {
		CHERRYSODA_ASSERT_FORMAT(false, "Missing animation name in SpriteData: '%s'!\n", id.GetStr().c_str());
		return nullptr;
	}
}

Sprite* SpriteBank::CreateOn(Sprite* sprite, const StringID& id)
{
	if (STL::ContainsKey(m_spriteData, id)) {
		return m_spriteData[id].CreateOn(sprite);
	}
	else {
		CHERRYSODA_ASSERT_FORMAT(false, "Missing animation name in SpriteData: '%s'!\n", id.GetStr().c_str());
		return nullptr;
	}
}
