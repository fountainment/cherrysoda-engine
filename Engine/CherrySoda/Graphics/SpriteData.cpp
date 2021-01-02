#include <CherrySoda/Graphics/SpriteData.h>

#include <CherrySoda/Components/Graphics/Sprite.h>
#include <CherrySoda/Graphics/Atlas.h>
#include <CherrySoda/Util/STL.h>

using cherrysoda::SpriteData;

using cherrysoda::Atlas;
using cherrysoda::Sprite;
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

void SpriteData::Add(const cherrysoda::json::Value* json, const String& overridePath/* = ""*/)
{
	// TODO: implement SpriteData::Add
}

Sprite* SpriteData::Create()
{
	return m_sprite->CreateClone(); 
}

Sprite* SpriteData::CreateOn(Sprite* clone)
{
	return m_sprite->CloneInto(clone);
}
