#include <CherrySoda/Components/Graphics/PixelText.h>

#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/PixelFont.h>
#include <CherrySoda/Util/String.h>

namespace cherrysoda {

PixelText::PixelText(PixelFont* font, const String& text, const Color& color) : base(false, true)
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
	auto offset = Vec2_Zero;

	for (size_t i = 0; i < m_convertedText.size(); ++i) {
		type::UInt32 c = m_convertedText[i];

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
		const auto* fontChar = m_size.Get(c);
		if (fontChar != nullptr) {
			STL::Add(m_characters,
					 Char{
						 .m_offset = offset + Math::Vec2(fontChar->m_xOffset, m_size.LineHeight() -
																				  fontChar->m_texture.Height() -
																				  fontChar->m_yOffset),
						 .m_charData = fontChar,
						 .m_bounds = fontChar->m_texture.ClipRect(),
					 });
			if (offset.x > widest) {
				widest = static_cast<int>(offset.x);
			}
			offset.x += fontChar->m_xAdvance;

			// kerning against the next character
			if (i + 1 < m_convertedText.size()) {
				auto kerning = fontChar->m_kerning.find(m_convertedText[i + 1]);
				if (kerning != fontChar->m_kerning.end()) {
					offset.x += kerning->second;
				}
			}
		}
	}

	m_width = widest;
	m_height = lines * m_size.LineHeight();
}

void PixelText::Render()
{
	RenderText(Vec2_Zero, m_color);
}

void PixelText::RenderText(const Math::Vec2& offset, const Color& color)
{
	if (m_dirty) {
		Refresh();
	}
	for (auto& character : m_characters) {
		character.m_charData->m_texture.Draw(Math::Vec3(m_position + offset + character.m_offset, 0.f), Vec3_Zero,
											 color);
	}
}

} // namespace cherrysoda
