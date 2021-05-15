#ifndef _CHERRYSODA_UTIL_PIXELFONT_H_
#define _CHERRYSODA_UTIL_PIXELFONT_H_

#include <CherrySoda/Graphics/MTexture.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>
#include <CherrySoda/Util/Json.h>

namespace cherrysoda {

class Atlas;

class PixelFontCharacter
{
public:
	PixelFontCharacter(type::UInt32 character, const MTexture& texture, const json::Value* jsonValue)
	{
		m_character = character;
		auto json = jsonValue->GetObject();
		m_texture = texture.GetSubtexture(json["x"].GetInt(), json["y"].GetInt(), json["width"].GetInt(), json["height"].GetInt());
		m_xOffset = json["xoffset"].GetInt();
		m_yOffset = json["yoffset"].GetInt();
		m_xAdvance = json["xadvance"].GetInt();
	}

private:
	friend class PixelText;

	type::UInt32 m_character = 0;
	MTexture m_texture;
	int m_xOffset = 0;
	int m_yOffset = 0;
	int m_xAdvance = 0;
	STL::HashMap<type::UInt32, type::UInt32> m_kerning;
};

class PixelFontSize
{
public:
	inline int LineHeight() const { return m_lineHeight; }
	inline int SpaceWidth() const { return m_lineHeight / 3; }
	// TODO: 1/3 line height for space width could be inappropriate in some cases?
	inline float Size() const { return m_size; }

	const PixelFontCharacter* Get(type::UInt32 id) const
	{
		return STL::GetValueAddress(m_characters, id);
	}

private:
	friend class PixelFont;

	STL::Vector<MTexture> m_textures;
	STL::HashMap<type::UInt32, PixelFontCharacter> m_characters;
	int m_lineHeight = 0;
	float m_size = 0.f;
	bool m_outline = false;
};

class PixelFont
{
public:
	PixelFont(const String& face) { m_face = face; }

	const PixelFontSize AddFontSize(const String& path, Atlas* atlas = nullptr, bool outline = false);
	const PixelFontSize AddFontSize(const String& path, const json::Value* jsonValue, Atlas* atlas = nullptr, bool outline = false);

	const PixelFontSize Get(float size) const;

	const PixelFontSize FirstSize() const { return STL::Front(m_sizes); }

private:
	String m_face;
	STL::List<PixelFontSize> m_sizes;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_PIXELFONT_H_
