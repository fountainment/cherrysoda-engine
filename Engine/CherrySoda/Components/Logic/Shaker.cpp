#include <CherrySoda/Components/Logic/Shaker.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Scene.h>
#include <CherrySoda/Util/Calc.h>

using cherrysoda::Shaker;

using cherrysoda::Calc;
using cherrysoda::Engine;

void Shaker::Update()
{
	if (m_on && m_timer > 0)
	{
		m_timer -= Engine::Instance()->DeltaTime();
		if (m_timer <= 0)
		{
			On(false);
			m_value = Vec2_Zero;
			if (m_onShake != nullptr)
				m_onShake(Vec2_Zero);
			if (m_removeOnFinish)
				RemoveSelf();
			return;
		}
	}

	if (m_on && GetScene()->OnInterval(m_interval))
	{
		m_value = Calc::GetRandom()->ShakeVector();
		if (m_onShake != nullptr)
			m_onShake(m_value);
	}
}
