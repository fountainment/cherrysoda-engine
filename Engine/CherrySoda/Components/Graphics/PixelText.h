#ifndef _CHERRYSODA_COMPONENTS_GRAPHICS_PIXELTEXT_H_
#define _CHERRYSODA_COMPONENTS_GRAPHICS_PIXELTEXT_H_

#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/PixelFont.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

namespace cherrysoda {

class PixelText : public Component
{
public:
	CHERRYSODA_DECLARE_COMPONENT(PixelText, Component);

	PixelText(PixelFont* font, const String& text, const Color& color);

	void Refresh();
	void Render() override;

	void Font(PixelFont* font)
	{
		if (m_font != font) {
		 	m_font = font;
		 	m_dirty = true;
		}
	}

	void Size(float size)
	{
		if (m_size.Size() != size) {
			m_size = m_font->Get(size);
			m_dirty = true;
		}
	}

	void Text(const String& text)
	{
		if (m_text != text) {
			m_text = text;
			m_convertedText = StringUtil::Utf8ToUnicode32(m_text);
			m_dirty = true;
		}
	}

	CHERRYSODA_GETTER_SETTER_OF_VEC2(Position, m_position);

private:
	struct Char
	{
		Math::Vec2 m_offset;
		const PixelFontCharacter* m_charData;
		Math::IRectangle m_bounds;
	};

	STL::Vector<Char> m_characters;
	PixelFont* m_font = nullptr;
	PixelFontSize m_size;
	String m_text;
	STL::Vector<type::UInt32> m_convertedText;
	bool m_dirty = false;

	Math::Vec2 m_position = Vec2_Zero;
	Color m_color = Color::White;
	Math::Vec2 m_scale = Vec2_One;
	int m_width = 0;
	int m_height = 0;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_GRAPHICS_PIXELTEXT_H_
