#ifndef _CHERRYSODA_UTIL_COMMANDS_H_
#define _CHERRYSODA_UTIL_COMMANDS_H_

#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

#define CHERRYSODA_CONSOLE_VARIABLE(VAR,TYPE,VALUE,HELP) \
static TYPE VAR=VALUE; \
static cherrysoda::CommandRegisterHelper CHERRYSODA_COMMAND_REGISTER_HELPER_##VAR(#VAR, \
[](const cherrysoda::STL::Vector<cherrysoda::String>& args) \
{ \
	if (cherrysoda::STL::Count(args) >= 1) \
		cherrysoda::StringUtil::SafeTo<TYPE>(args[0],VAR); \
	cherrysoda::Commands::Log(cherrysoda::StringUtil::ToString(VAR)); \
	cherrysoda::Commands::Return(static_cast<float>(VAR)); \
},HELP);

#define CHERRYSODA_REGISTER_COMMAND(COMMAND,FUNC,HELP) \
static cherrysoda::CommandRegisterHelper CHERRYSODA_COMMAND_REGISTER_HELPER_##COMMAND(#COMMAND,FUNC,HELP);

#define CHERRYSODA_COMMAND(COMMAND,HELP) \
void CherrySodaCommand_##COMMAND(const cherrysoda::STL::Vector<cherrysoda::String>& args); \
CHERRYSODA_REGISTER_COMMAND(COMMAND,CherrySodaCommand_##COMMAND,HELP); \
void CherrySodaCommand_##COMMAND(const cherrysoda::STL::Vector<cherrysoda::String>& args)

namespace cherrysoda {

class Commands
{
public:
	friend class GUI;

	static void Register(const String& command, STL::Action<const STL::Vector<String>&> action, const String& help);

	static void ExecuteCommand();
	static void ExecuteCommand(const String& command);

	static void Log(const String& str, const Color& color = Color::White);

	static void Return(float f);
	static void Return(int i);
	static void Return(const String& s);
	static bool HasReturnValue();
	static float GetReturnedFloat();
	static int GetReturnedInt();
	static String GetReturnedString();

	static void ClearCommand();
	static String GetCompletionSuffix(const String& prefix);
	static void CompleteCommand();

	static String GetBackwardHistory();
	static String GetForwardHistory();

	static void ClearLog();

	static void ShowHelp();
	static void ShowHelp(const String& command);

	static void AddParamSlider(const String& param, float minValue, float maxValue, float defaultValue);

private:
	struct CommandInfo
	{
		STL::Action<const STL::Vector<String>&> action;
		String name;
		String help;
	};

	struct SliderInfo
	{
		String param;
		float minValue;
		float maxValue;
		float value;
	};

	struct ReturnValue
	{
		bool hasReturnValue = false;
		float floatValue = 0.f;
		int intValue = 0;
		String stringValue = "";
	};

	static STL::HashMap<StringID, CommandInfo>& INTERNAL_GetCommands();

	static ReturnValue ms_returnValue;

	static char ms_currentText[512];
	static STL::Vector<STL::Pair<Color,String>> ms_drawCommands;
	static bool ms_consoleTextScrollNeeded;

	static STL::Vector<String> ms_commandHistory;
	static int ms_commandHistoryIndex;

	static STL::List<SliderInfo> ms_sliderInfo;
};

class CommandRegisterHelper
{
public:
	inline CommandRegisterHelper(const String& command, STL::Action<const STL::Vector<String>&> action, const String& help)
	{
		Commands::Register(command, action, help);
	}
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_COMMANDS_H_
