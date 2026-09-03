#ifndef _CHERRYSODA_COMPONENTS_LOGIC_SHAKERLIST_H_
#define _CHERRYSODA_COMPONENTS_LOGIC_SHAKERLIST_H_

#include <CherrySoda/Components/Component.h>

#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

// Multi-value shaker for parallax-style effects; every entry of Values()
// shakes independently. Port of Monocle's ShakerList.
class ShakerList : public Component
{
public:
	CHERRYSODA_DECLARE_COMPONENT(ShakerList, Component);

	ShakerList(int length, bool on = true, STL::Action<STL::Vector<Math::Vec2>&> onShake = nullptr) : base(true, false)
	{
		STL::Resize(m_values, length);
		STL::Fill(m_values, Vec2_Zero);
		On(on);
		m_onShake = onShake;
	}

	ShakerList* ShakeFor(float seconds, bool removeOnFinish = false)
	{
		On(true);
		m_timer = seconds;
		m_removeOnFinish = removeOnFinish;
		return this;
	}

	inline const STL::Vector<Math::Vec2>& Values() const { return m_values; }
	inline bool On() const { return m_on; }
	void On(bool on);
	CHERRYSODA_GETTER_SETTER_OF_TYPE(float, Interval, m_interval);
	CHERRYSODA_GETTER_SETTER_OF_TYPE(float, Timer, m_timer);

	void Update() override;

private:
	STL::Vector<Math::Vec2> m_values;
	float m_interval = .05f;
	float m_timer = 0.f;
	bool m_removeOnFinish = false;
	bool m_on = false;

	STL::Action<STL::Vector<Math::Vec2>&> m_onShake = nullptr;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_LOGIC_SHAKERLIST_H_
