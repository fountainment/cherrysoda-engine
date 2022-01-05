#include <CherrySoda/Graphics/SpriteBank.h>

#include <CherrySoda/Components/Graphics/Sprite.h>
#include <CherrySoda/Graphics/Atlas.h>
#include <CherrySoda/Graphics/SpriteData.h>
#include <CherrySoda/Util/Json.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/String.h>
#include <CherrySoda/Util/STL.h>

using cherrysoda::SpriteBank;

using cherrysoda::Atlas;
using cherrysoda::JsonUtil;
using cherrysoda::Sprite;
using cherrysoda::SpriteData;
using cherrysoda::String;
using cherrysoda::StringID;
using cherrysoda::STL;

SpriteBank::SpriteBank(Atlas* atlas, const String& jsonPath)
{
	m_atlas = atlas;
	JsonUtil::ReadJsonFile(m_json, jsonPath);

	STL::HashMap<StringID, String> jsons;

	STL::HashMap<StringID, const cherrysoda::json::Value*> elements;
	if (m_json.IsObject()) {
		for (const auto& element : m_json["Sprites"].GetArray()) {
			const char* elementName = element["Name"].GetString();
			STL::Add(elements, STL::MakePair(StringID(elementName), &element));
			CHERRYSODA_ASSERT_FORMAT(!STL::ContainsKey(m_spriteData, elementName), "Duplicate sprite name in SpriteData: '%s'!", elementName);

			auto data = m_spriteData[elementName] = new SpriteData(m_atlas);
			if (element.HasMember("copy")) {
				data->Add(elements[StringID(element["copy"].GetString())], element["path"].GetString());
			}
			data->Add(&element);
		}
	}
	else {
		CHERRYSODA_ASSERT_FORMAT(false, "Json validation failed on \"%s\"!\n", jsonPath.c_str());
	}
}

SpriteBank::SpriteBank(const String& atlasPath, const String& jsonPath)
: SpriteBank(Atlas::FromAtlas(atlasPath), jsonPath)
{
	m_ownAtlas = true;
}

SpriteBank::~SpriteBank()
{
	for (auto& data : m_spriteData) {
		delete data.second;
	}
	STL::Clear(m_spriteData);
	if (m_ownAtlas) {
		delete m_atlas;
	}
}

bool SpriteBank::Has(const StringID& id)
{
	return STL::ContainsKey(m_spriteData, id);
}

Sprite* SpriteBank::Create(const StringID& id)
{
	if (STL::ContainsKey(m_spriteData, id)) {
		return m_spriteData[id]->Create();
	}
	else {
		CHERRYSODA_ASSERT_FORMAT(false, "Missing animation name in SpriteData: '%s'!\n", id.GetStr().c_str());
		return nullptr;
	}
}

Sprite* SpriteBank::CreateOn(Sprite* sprite, const StringID& id)
{
	if (STL::ContainsKey(m_spriteData, id)) {
		return m_spriteData[id]->CreateOn(sprite);
	}
	else {
		CHERRYSODA_ASSERT_FORMAT(false, "Missing animation name in SpriteData: '%s'!\n", id.GetStr().c_str());
		return nullptr;
	}
}
