#ifndef _CHERRYSODA_COMPONENTS_GRAPHICS_NUMBERTEXT_H_
#define _CHERRYSODA_COMPONENTS_GRAPHICS_NUMBERTEXT_H_

#include <CherrySoda/Components/Graphics/PixelText.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

namespace cherrysoda {

// PixelText displaying a numeric value with an optional prefix; setting Value
// updates the string and fires OnValueUpdate with the previous value. Port of
// Monocle's NumberText.
class NumberText : public PixelText
{
public:
	CHERRYSODA_DECLARE_COMPONENT(NumberText, PixelText);

	NumberText(PixelFont* font, const String& prefix, int value, const Color& color, bool centered = false)
		: base(font, "", color)
	{
		m_prefix = prefix;
		m_centered = centered;
		Value(value);
	}

	inline int Value() const { return m_value; }
	void Value(int value)
	{
		if (m_value != value) {
			int oldValue = m_value;
			m_value = value;
			UpdateString();
			if (m_onValueUpdate != nullptr) {
				m_onValueUpdate(oldValue);
			}
		}
	}

	CHERRYSODA_GETTER_SETTER_OF_TYPE(STL::Action<int>, OnValueUpdate, m_onValueUpdate);
	CHERRYSODA_GETTER_SETTER_OF_BOOL(Centered, m_centered);

	void UpdateString() { Text(m_prefix + StringUtil::ToString(m_value)); }

	void Render() override { RenderText(m_centered ? Math::Vec2(Width(), Height()) * -0.5f : Vec2_Zero, GetColor()); }

private:
	int m_value = 0;
	String m_prefix;
	bool m_centered = false;

	STL::Action<int> m_onValueUpdate = nullptr;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_GRAPHICS_NUMBERTEXT_H_
