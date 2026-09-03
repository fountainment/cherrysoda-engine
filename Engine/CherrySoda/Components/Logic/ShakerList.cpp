#include <CherrySoda/Components/Logic/ShakerList.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Scene.h>
#include <CherrySoda/Util/Calc.h>

namespace cherrysoda {

void ShakerList::On(bool on)
{
	m_on = on;
	if (!m_on) {
		m_timer = 0.f;
		bool anyNonZero = false;
		for (const auto& value : m_values) {
			if (value != Vec2_Zero) {
				anyNonZero = true;
				break;
			}
		}
		if (anyNonZero) {
			STL::Fill(m_values, Vec2_Zero);
			if (m_onShake != nullptr) {
				m_onShake(m_values);
			}
		}
	}
}

void ShakerList::Update()
{
	if (m_on && m_timer > 0.f) {
		m_timer -= Engine::Instance()->DeltaTime();
		if (m_timer <= 0.f) {
			On(false);
			if (m_removeOnFinish) {
				RemoveSelf();
			}
			return;
		}
	}

	if (m_on && GetScene()->OnInterval(m_interval)) {
		for (auto& value : m_values) {
			value = Calc::GetRandom()->ShakeVector();
		}
		if (m_onShake != nullptr) {
			m_onShake(m_values);
		}
	}
}

} // namespace cherrysoda
