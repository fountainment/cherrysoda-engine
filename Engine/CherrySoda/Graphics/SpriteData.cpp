#include <CherrySoda/Graphics/SpriteData.h>

#include <CherrySoda/Components/Graphics/Sprite.h>
#include <CherrySoda/Graphics/Atlas.h>
#include <CherrySoda/Util/Chooser.h>
#include <CherrySoda/Util/Json.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/String.h>
#include <CherrySoda/Util/STL.h>

using cherrysoda::SpriteData;

using cherrysoda::Atlas;
using cherrysoda::Chooser;
using cherrysoda::Math;
using cherrysoda::Sprite;
using cherrysoda::SpriteDataSource;
using cherrysoda::String;
using cherrysoda::StringID;
using cherrysoda::STL;

SpriteData::SpriteData(Atlas* atlas)
{
	m_sprite = new Sprite(atlas, "");
	m_atlas = atlas;
}

SpriteData::~SpriteData()
{
	if (m_sprite) {
		delete m_sprite;
		m_sprite = nullptr;
	}
	for (auto& source : m_sources) {
		delete source;
	}
	STL::Clear(m_sources);
}

void SpriteData::Add(const cherrysoda::json::Value* jsonValue, const String& overridePath/* = ""*/)
{
	auto jsonObj = jsonValue->GetObject();

	auto source = new SpriteDataSource;
	source->m_json = jsonValue;
	source->m_path = jsonObj["path"].GetString();
	source->m_overridePath = overridePath;

	// Error Checking
	{
		// TODO: add error checking
	}

	// Create the Sprite
	{
		const char* normalPath = jsonObj.HasMember("path") ? jsonObj["path"].GetString() : "";
		float masterDelay = jsonObj.HasMember("delay") ? jsonObj["delay"].GetFloat() : 0.f;
		bool useIdForDefaultAnimationPath = jsonObj.HasMember("UseIdForDefaultAnimationPath");

		// Build Animation
		if (jsonObj.HasMember("Anim")) {
			for (const auto& anim : jsonObj["Anim"].GetArray()) {
				Chooser<StringID> into;
				if (anim.HasMember("goto")) {
					into = Chooser<StringID>::FromString(anim["goto"].GetString());
				}

				const char* id = anim["id"].GetString();
				String path = "";
				if (anim.HasMember("path")) {
					path = anim["path"].GetString();
				}
				else if (useIdForDefaultAnimationPath) {
					path = id;
				}
				float delay = anim.HasMember("delay") ? anim["delay"].GetFloat() : masterDelay;
				if (overridePath.length() != 0) {
					path = overridePath + path;
				}
				else {
					path = normalPath + path;
				}
				// TODO: add frames
				m_sprite->Add(id, path, delay, into);
			}
		}

		// Build Loops
		if (jsonObj.HasMember("Loop")) {
			for (const auto& loop : jsonObj["Loop"].GetArray()) {
				const char* id = loop["id"].GetString();
				String path = "";
				if (loop.HasMember("path")) {
					path = loop["path"].GetString();
				}
				else if (useIdForDefaultAnimationPath) {
					path = id;
				}
				float delay = loop.HasMember("delay") ? loop["delay"].GetFloat() : masterDelay;
				if (overridePath.length() != 0) {
					path = overridePath + path;
				}
				else {
					path = normalPath + path;
				}
				// TODO: add frames
				m_sprite->AddLoop(id, path, delay);
			}
		}

		// Origin
		if (jsonObj.HasMember("Center")) {
			m_sprite->CenterOrigin();
			m_sprite->Justify(Math::Vec2(0.5f));
		}
		else if (jsonObj.HasMember("Justify")) {
			auto& justifyObj = jsonObj["Justify"];
			Math::Vec2 justify(justifyObj["x"].GetFloat(), justifyObj["y"].GetFloat());
			m_sprite->JustifyOrigin(justify);
			m_sprite->Justify(justify);
		}
		else if (jsonObj.HasMember("Origin")) {
			auto& originObj = jsonObj["Origin"];
			Math::Vec2 origin(originObj["x"].GetFloat(), originObj["y"].GetFloat());
			m_sprite->Origin(origin);
		}

		// Position
		if (jsonObj.HasMember("Position")) {
			auto& posObj = jsonObj["Position"];
			Math::Vec2 pos(posObj["x"].GetFloat(), posObj["y"].GetFloat());
			m_sprite->Position(pos);
		}

		// Start Animation
		if (jsonObj.HasMember("start")) {
			m_sprite->Play(jsonObj["start"].GetString());
		}
	}

	STL::Add(m_sources, source);
}

Sprite* SpriteData::Create()
{
	return m_sprite->CreateClone();
}

Sprite* SpriteData::CreateOn(Sprite* clone)
{
	return m_sprite->CloneInto(clone);
}
