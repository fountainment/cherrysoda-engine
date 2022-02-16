#include <CherrySoda/Components/Graphics/PixelText.h>

#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/PixelFont.h>
#include <CherrySoda/Util/String.h>

using cherrysoda::PixelText;

using cherrysoda::Color;
using cherrysoda::Math;
using cherrysoda::PixelFont;
using cherrysoda::String;


PixelText::PixelText(PixelFont* font, const String& text, const Color& color)
	: base(false, true)
{
	Font(font);
	Text(text);

	m_color = color;
	m_size = m_font->FirstSize();

	Refresh();
}

void PixelText::Refresh()
{
	m_dirty = false;
	STL::Clear(m_characters);

	int widest = 0;
	int lines = 1;
	Math::Vec2 offset = Vec2_Zero;

	for (auto c : m_convertedText) {
		// new line
		if (c == '\n') {
			offset.x = 0.f;
			offset.y -= m_size.LineHeight();
			++lines;
			continue;
		}

		// white space
		if (c == ' ') {
			offset.x += m_size.SpaceWidth();
			continue;
		}

		// add char
		auto fontChar = m_size.Get(c);
		if (fontChar != nullptr) {
			STL::Add(m_characters, Char
			{
				offset + Math::Vec2(fontChar->m_xOffset, m_size.LineHeight() - fontChar->m_texture.Height() - fontChar->m_yOffset),
				fontChar,
				fontChar->m_texture.ClipRect()
			});
			if (offset.x > widest) {
				widest = static_cast<int>(offset.x);
			}
			offset.x += fontChar->m_xAdvance;
		}
	}

	m_width = widest;
	m_height = lines * m_size.LineHeight();
}

void PixelText::Render()
{
	if (m_dirty) {
		Refresh();
	}
	for (auto& character : m_characters) {
		character.m_charData->m_texture.Draw(Math::Vec3(m_position + character.m_offset, 0.f), Vec3_Zero, m_color);
	}
}
