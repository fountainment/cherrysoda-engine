#ifndef _CHERRYSODA_COMPONENTS_LOGIC_COROUTINEHOLDER_H_
#define _CHERRYSODA_COMPONENTS_LOGIC_COROUTINEHOLDER_H_

#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

// Runs multiple coroutine routines at once; see Coroutine.h for the routine
// combinators. Port of Monocle's CoroutineHolder.
class CoroutineHolder : public Component
{
public:
	CHERRYSODA_DECLARE_COMPONENT(CoroutineHolder, Component);

	CoroutineHolder() : base(true, false) {}

	int StartCoroutine(STL::Func<bool> routine);
	void EndCoroutine(int id);

	void Update() override;

private:
	struct CoroutineData
	{
		int m_id;
		STL::Func<bool> m_routine;
	};

	STL::Vector<CoroutineData> m_coroutines;
	STL::Vector<int> m_toEnd;
	int m_nextId = 0;
	bool m_running = false;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_LOGIC_COROUTINEHOLDER_H_
