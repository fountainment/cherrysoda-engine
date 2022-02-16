#ifndef _CHERRYSODA_COMPONENTS_LOGIC_ALARM_H_
#define _CHERRYSODA_COMPONENTS_LOGIC_ALARM_H_

#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class Entity;

enum class AlarmMode
{
	Persist,
	Oneshot,
	Looping
};

class Alarm : public Component
{
public:
	CHERRYSODA_DECLARE_COMPONENT(Alarm, Component);

	inline AlarmMode Mode() const { return m_mode; }
	inline float Duration() const { return m_duration; }
	inline float TimeLeft() const { return m_timeLeft; }
	inline float ProgressLeft() const { return TimeLeft() / Duration(); }
	inline float Progress() const { return 1.0f - ProgressLeft(); }

	static Alarm* Create(AlarmMode mode, STL::Action<> onComplete, float duration = 1.f, bool start = false);
	static Alarm* Set(Entity* entity, float duration, STL::Action<> onComplete, AlarmMode alarmMode = AlarmMode::Oneshot);

	void Update() override;
	void Removed(Entity* entity) override;
	void Start();
	void Start(float duration);
	void Stop();

private:
	Alarm() : base(false, false) {}

	void Init(AlarmMode mode, STL::Action<> onComplete, float duration = 1.f, bool start = false);

	static STL::Stack<Alarm*> ms_cached;

	AlarmMode m_mode;
	STL::Action<> m_onComplete;

	float m_duration;
	float m_timeLeft;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_LOGIC_ALARM_H_
