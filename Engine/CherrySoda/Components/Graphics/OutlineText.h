#ifndef _CHERRYSODA_COMPONENTS_GRAPHICS_OUTLINETEXT_H_
#define _CHERRYSODA_COMPONENTS_GRAPHICS_OUTLINETEXT_H_

#include <CherrySoda/Components/Graphics/PixelText.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>

namespace cherrysoda {

// PixelText that draws an eight-direction outline underneath itself. Port of
// Monocle's OutlineText.
class OutlineText : public PixelText
{
public:
	CHERRYSODA_DECLARE_COMPONENT(OutlineText, PixelText);

	OutlineText(PixelFont* font, const String& text, const Color& color) : base(font, text, color) {}

	CHERRYSODA_GETTER_SETTER_OF_TYPE(Color, OutlineColor, m_outlineColor);
	CHERRYSODA_GETTER_SETTER_OF_TYPE(int, OutlineOffset, m_outlineOffset);

	void Render() override
	{
		Math::Vec2 offsetStep = Math::Vec2((float)m_outlineOffset, (float)m_outlineOffset);
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (i != 0 || j != 0) {
					RenderText(offsetStep * Math::Vec2((float)i, (float)j), m_outlineColor);
				}
			}
		}
		base::Render();
	}

private:
	Color m_outlineColor = Color::Black;
	int m_outlineOffset = 1;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_GRAPHICS_OUTLINETEXT_H_
