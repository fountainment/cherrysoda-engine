#ifndef _CHERRYSODA_UTIL_CHEATLISTENER_H_
#define _CHERRYSODA_UTIL_CHEATLISTENER_H_

#include <CherrySoda/Entity.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

namespace cherrysoda {

// Listens for sequences of inputs and fires a one-shot action when the
// recent input ends with a registered cheat code. Port of Monocle's
// CheatListener.
class CheatListener : public Entity
{
public:
	CHERRYSODA_DECLARE_ENTITY(CheatListener, Entity);

	CheatListener() { Visible(false); }

	void Update() override;

	// Registers a cheat code (e.g. "UUDDLRLR") fired once when entered
	void AddCheat(const String& code, STL::Action<> onEntered = nullptr);
	// Registers an input id appended to the buffer whenever checker returns
	// true (call from the checker's own Pressed-style query)
	void AddInput(char id, STL::Func<bool> checker);

	CHERRYSODA_GETTER_SETTER_OF_BOOL(Logging, m_logging);

	inline const String& CurrentInput() const { return m_currentInput; }

private:
	struct Input
	{
		char m_id;
		STL::Func<bool> m_checker;
	};
	struct Cheat
	{
		String m_code;
		STL::Action<> m_onEntered;
	};

	STL::Vector<Input> m_inputs;
	STL::Vector<Cheat> m_cheats;
	int m_maxInput = 0;
	bool m_logging = false;

	String m_currentInput;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_CHEATLISTENER_H_
