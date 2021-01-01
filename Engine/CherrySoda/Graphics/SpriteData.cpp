#include <CherrySoda/Graphics/SpriteData.h>

#include <CherrySoda/Components/Graphics/Sprite.h>
#include <CherrySoda/Graphics/Atlas.h>

using cherrysoda::SpriteData;

using cherrysoda::Atlas;
using cherrysoda::Sprite;

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
}

Sprite* SpriteData::Create()
{
	return m_sprite->CreateClone(); 
}

Sprite* SpriteData::CreateOn(Sprite* clone)
{
	return m_sprite->CloneInto(clone);
}
