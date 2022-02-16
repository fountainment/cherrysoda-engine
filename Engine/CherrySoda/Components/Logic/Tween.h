#ifndef _CHERRYSODA_COMPONENTS_LOGIC_TWEEN_H_
#define _CHERRYSODA_COMPONENTS_LOGIC_TWEEN_H_

#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Util/Ease.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class Entity;

enum class TweenMode
{
	Persist,
	Oneshot,
	Looping,
	YoyoOneshot,
	YoyoLooping
};

class Tween : public Component
{
public:
	CHERRYSODA_DECLARE_COMPONENT(Tween, Component);

	Tween() : base(false, false) {}

	void Update() override;
	void Removed(Entity* entity) override;

	void Start(bool reverse = false);
	void Stop();
	void Reset();

	inline void OnUpdate(STL::Action<Tween*> onUpdate) { m_onUpdate = onUpdate; }
	inline void OnComplete(STL::Action<Tween*> onComplete) { m_onComplete = onComplete; }
	inline void OnStart(STL::Action<Tween*> onStart) { m_onStart = onStart; }

	inline float Eased() const { return m_eased; }

	inline const Easer GetEaser() const { return m_easer; }
	inline void SetEaser(Easer easer) { m_easer = easer; }

	static Tween* Create(TweenMode mode, Easer easer = nullptr, float duration = 1.f, bool start = false);

private:
	void Init(TweenMode mode, Easer easer, float duration, bool start);

	Easer m_easer;
	STL::Action<Tween*> m_onUpdate;
	STL::Action<Tween*> m_onComplete;
	STL::Action<Tween*> m_onStart;

	TweenMode m_mode;
	float m_duration;
	float m_timeLeft;
	float m_percent;
	float m_eased;
	bool m_useRawDeltaTime;
	bool m_startedReversed;
	bool m_reverse;

	static STL::Stack<Tween*> ms_cached;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_LOGIC_TWEEN_H_
