#include <CherrySoda/Util/CheatListener.h>

#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

#include <utility>

namespace cherrysoda {

void CheatListener::AddCheat(const String& code, STL::Action<> onEntered /* = nullptr*/)
{
	STL::Add(m_cheats, Cheat{.m_code = code, .m_onEntered = std::move(onEntered)});
	m_maxInput = Math_Max(static_cast<int>(code.size()), m_maxInput);
}

void CheatListener::AddInput(char id, STL::Func<bool> checker)
{
	STL::Add(m_inputs, Input{.m_id = id, .m_checker = std::move(checker)});
}

void CheatListener::Update()
{
	// Detect input
	bool changed = false;
	for (auto& input : m_inputs) {
		if (input.m_checker != nullptr && input.m_checker()) {
			m_currentInput += input.m_id;
			changed = true;
		}
	}

	// Handle changes
	if (changed) {
		if (std::cmp_greater(m_currentInput.size(), m_maxInput)) {
			m_currentInput = m_currentInput.substr(m_currentInput.size() - m_maxInput);
		}

		if (m_logging) {
			CHERRYSODA_LOG(m_currentInput);
		}

		for (auto it = m_cheats.begin(); it != m_cheats.end(); ++it) {
			if (m_currentInput.find(it->m_code) != String::npos) {
				m_currentInput.clear();
				STL::Action<> onEntered = it->m_onEntered;
				m_cheats.erase(it);
				if (onEntered != nullptr) {
					onEntered();
				}

				if (m_logging) {
					CHERRYSODA_LOG("Cheat Activated!\n");
				}

				break;
			}
		}
	}
}

} // namespace cherrysoda
