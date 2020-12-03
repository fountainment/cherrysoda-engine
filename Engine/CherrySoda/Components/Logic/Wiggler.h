#ifndef _CHERRYSODA_COMPONENTS_LOGIC_H_
#define _CHERRYSODA_COMPONENTS_LOGIC_H_

#include <CherrySoda/Components/Component.h>

#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class Entity;

class Wiggler : public Component
{
public:
	CHERRYSODA_DECLARE_COMPONENT(Wiggler, Component);

	void Removed(Entity* entity) override;
	void Start();
	void Start(float duration, float frequency);
	inline void Stop() { Active(false); }
	void StopAndClear();

	void Update() override;

	static Wiggler* Create(float duration, float frequency, STL::Action<float> onChange = nullptr, bool start = false, bool removeSelfOnFinish = false);

private:
	Wiggler() : base(false, false) {}

	void Init(float duration, float frequency, STL::Action<float> onChange, bool start, bool removeSelfOnFinish);

	float m_counter;
	float m_value;
	bool m_startZero;
	bool m_useRawDeltaTime;

	float m_sineCounter;
	float m_increment;
	float m_sineAdd;
	STL::Action<float> m_onChange;
	bool m_removeSelfOnFinish;

	static STL::Stack<Wiggler*> ms_cache;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_LOGIC_H_
