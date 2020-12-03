#ifndef _CHERRYSODA_COMPONENTS_LOGIC_SHAKER_H_
#define _CHERRYSODA_COMPONENTS_LOGIC_SHAKER_H_

#include <CherrySoda/Components/Component.h>

#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class Shaker : public Component
{
public:
	CHERRYSODA_DECLARE_COMPONENT(Shaker, Component);

	Shaker(bool on = true, STL::Action<Math::Vec2> onShake = nullptr)
		: base(true, false)
	{
		On(on);
		m_onShake = onShake;	
	}

	Shaker* ShakeFor(float seconds, bool removeOnFinish = false)
	{
		On(true);
		m_timer = seconds;
		m_removeOnFinish = removeOnFinish;	
		return this;
	}

	void Update() override;

private:
	void On(bool on)
	{
		m_on = on;
		if (!m_on) {
			m_timer = 0.f;
			if (m_value != Vec2_Zero) {
				m_value = Vec2_Zero;
				if (m_onShake != nullptr) {
					m_onShake(Vec2_Zero);
				}
			}
		}
	}

	Math::Vec2 m_value;
	float m_interval = .05f;	
	float m_timer;
	bool m_removeOnFinish;
	bool m_on;

	STL::Action<Math::Vec2> m_onShake;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_LOGIC_SHAKER_H_
