#ifndef _CHERRYSODA_COMPONENTS_GRAPHICS_TIMERTEXT_H_
#define _CHERRYSODA_COMPONENTS_GRAPHICS_TIMERTEXT_H_

#include <CherrySoda/Components/Graphics/PixelText.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

#include <utility>

namespace cherrysoda {

// PixelText counting frames up or down, formatted as seconds with two
// decimals; counting down fires OnComplete at zero. Port of Monocle's
// TimerText.
class TimerText : public PixelText
{
public:
	enum class CountModes
	{
		Down,
		Up
	};

	enum class TimerModes
	{
		SecondsMilliseconds
	};

	CHERRYSODA_DECLARE_COMPONENT(TimerText, PixelText);

	TimerText(PixelFont* font, TimerModes timerMode, CountModes countMode, int frames, const Math::Vec2& justify,
			  const Color& color, STL::Action<> onComplete = nullptr)
		: base(font, "", color)
	{
		CHERRYSODA_ASSERT(frames >= 0, "TimerText frames must be larger than or equal to zero!\n");

		m_timerMode = timerMode;
		m_countMode = countMode;
		m_frames = frames;
		m_justify = justify;
		m_onComplete = std::move(onComplete);

		UpdateText();
	}

	void Update() override
	{
		if (m_countMode == CountModes::Down) {
			if (m_frames > 0) {
				--m_frames;
				if (m_frames == 0 && m_onComplete != nullptr) {
					m_onComplete();
				}
				UpdateText();
			}
		}
		else {
			++m_frames;
			UpdateText();
		}
	}

	void Render() override { RenderText(Math::Vec2(Width(), Height()) * -m_justify, GetColor()); }

	inline int Frames() const { return m_frames; }
	void Frames(int frames)
	{
		CHERRYSODA_ASSERT(frames >= 0, "TimerText frames must be larger than or equal to zero!\n");
		if (m_frames != frames) {
			m_frames = frames;
			UpdateText();
		}
	}

	CHERRYSODA_GETTER_SETTER_OF_VEC2(Justify, m_justify);
	CHERRYSODA_GETTER_SETTER_OF_TYPE(CountModes, CountMode, m_countMode);

private:
	void UpdateText()
	{
		constexpr float deltaTime = 1.f / 60.f;
		const int seconds = m_frames / 60;
		Text(CHERRYSODA_FORMAT("%.2f", seconds + ((m_frames % 60) * deltaTime)));
	}

	TimerModes m_timerMode;
	CountModes m_countMode;
	int m_frames = 0;
	Math::Vec2 m_justify = Vec2_Zero;

	STL::Action<> m_onComplete = nullptr;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_GRAPHICS_TIMERTEXT_H_
