#include <CherrySoda/Util/Commands.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

using cherrysoda::Commands;

using cherrysoda::Color;
using cherrysoda::Engine;
using cherrysoda::Graphics;
using cherrysoda::STL;
using cherrysoda::String;
using cherrysoda::StringID;
using cherrysoda::StringUtil;


struct CommandTrieNode {
	STL::Map<char,CommandTrieNode*> next;
	bool exists = false;
};

static CommandTrieNode* GetCommandTrieRoot()
{
	static CommandTrieNode* commandTrieRoot = new CommandTrieNode;
	return commandTrieRoot;
}

static void CommandTrieInsert(const String& command)
{
	CommandTrieNode* node = GetCommandTrieRoot();
	for (char c : command) {
		if (!STL::ContainsKey(node->next, c)) {
			node->next[c] = new CommandTrieNode;
		}
		node = node->next[c];
	}
	node->exists = true;
}

static void CommandTrieTraverse(CommandTrieNode* node, String current, STL::Vector<String>& result)
{
	if (node->exists) {
		STL::Add(result, current);
	}
	for (auto& p : node->next) {
		CommandTrieTraverse(p.second, current + p.first, result);
	}
}

static STL::Vector<String> CommandTrieFindPrefix(const String& prefix)
{
	STL::Vector<String> ret;
	CommandTrieNode* node = GetCommandTrieRoot();
	for (auto c : prefix) {
		if (!STL::ContainsKey(node->next, c)) {
			return ret;
		}
		node = node->next[c];
	}
	CommandTrieTraverse(node, "", ret);
	return ret;
}

Commands::ReturnValue Commands::ms_returnValue;

char Commands::ms_currentText[512] = "";
STL::Vector<STL::Pair<Color,String>> Commands::ms_drawCommands;
bool Commands::ms_consoleTextScrollNeeded = false;

STL::Vector<String> Commands::ms_commandHistory;
int Commands::ms_commandHistoryIndex = -1;

STL::List<Commands::SliderInfo> Commands::ms_sliderInfo;

void Commands::Register(const String& command, STL::Action<const STL::Vector<String>&> action, const String& help)
{
	String lowerCommand = StringUtil::ToLower(command);
	CHERRYSODA_ASSERT_FORMAT(!STL::ContainsKey(INTERNAL_GetCommands(), lowerCommand), "Command alreay exists: %s\n", lowerCommand.c_str());
	INTERNAL_GetCommands()[command] = { action, lowerCommand, help };
	CommandTrieInsert(command);
}

void Commands::ExecuteCommand()
{
	String currentText(ms_currentText);
	STL::Add(ms_commandHistory, currentText);
	ms_commandHistoryIndex = -1;
	ms_currentText[0] = '\0';
	Log("cherrysoda # " + currentText);
	ExecuteCommand(currentText);	
}

void Commands::ExecuteCommand(const String& command)
{
	ms_returnValue = { false, 0.f, 0, "" };

	auto data = StringUtil::Split(command);
	Commands::CommandInfo info;
	if (STL::TryGetValue(INTERNAL_GetCommands(), StringUtil::ToLower(data[0]), info)) {
		info.action(STL::Vector<String>(data.begin() + 1, data.end()));
	}
	else {
		Log(StringUtil::Format("Command not found: %s", data[0].c_str()), Color::Orange);
	}
}

void Commands::Log(const String& str, const Color& color/* = Color::White*/)
{
	auto lines = StringUtil::Split(str, '\n');
	for (auto& line : lines) {
		STL::Add(ms_drawCommands, STL::MakePair(color, line));
	}
	ms_consoleTextScrollNeeded = true;
}

void Commands::Return(float f)
{
	ms_returnValue.floatValue = f;
	ms_returnValue.hasReturnValue = true;
}

void Commands::Return(int i)
{
	ms_returnValue.intValue = i;
	ms_returnValue.hasReturnValue = true;
}

void Commands::Return(const String& s)
{
	ms_returnValue.stringValue = s;
	ms_returnValue.hasReturnValue = true;
}

bool Commands::HasReturnValue()
{
	return ms_returnValue.hasReturnValue;
}

float Commands::GetReturnedFloat()
{
	return ms_returnValue.floatValue;
}

int Commands::GetReturnedInt()
{
	return ms_returnValue.intValue;
}

String Commands::GetReturnedString()
{
	return ms_returnValue.stringValue;
}

void Commands::ClearCommand()
{
	ms_currentText[0] = '\0';
}

String Commands::GetCompletionSuffix(const String& prefix)
{
	if (prefix == "") return "";
	auto suffixes = CommandTrieFindPrefix(prefix);
	int suffixCount = STL::Count(suffixes);
	if (suffixCount == 0) {
		return "";
	}
	else if (suffixCount == 1) {
		return suffixes[0] + " ";
	}
	else {
		for (auto& suffix : suffixes) {
			Log(prefix + suffix);
		}
		return "";
	}
}

void Commands::CompleteCommand()
{
	String prefix = ms_currentText;
	String suffix = GetCompletionSuffix(prefix);
	String result = prefix + suffix;
	int resultLen = result.length();
	for (int i = 0; i < resultLen; ++i) {
		ms_currentText[i] = result[i];
	}
	ms_currentText[resultLen] = '\0';
}

String Commands::GetBackwardHistory()
{
	if (ms_commandHistoryIndex == -1) {
		ms_commandHistoryIndex = STL::Count(ms_commandHistory) - 1;
		if (ms_commandHistoryIndex == -1) {
			return ms_currentText;
		}
	}
	else if (ms_commandHistoryIndex > 0) {
		ms_commandHistoryIndex--;
	}
	return ms_commandHistory[ms_commandHistoryIndex];
}

String Commands::GetForwardHistory()
{
	if (ms_commandHistoryIndex == -1) {
		return ms_currentText;
	}
	else if (ms_commandHistoryIndex < static_cast<int>(STL::Count(ms_commandHistory)) - 1) {
		ms_commandHistoryIndex++;
	}
	else {
		ms_commandHistoryIndex = -1;
		return "";
	}
	return ms_commandHistory[ms_commandHistoryIndex];
}

void Commands::ClearLog()
{
	STL::Clear(ms_drawCommands);
}

void Commands::ShowHelp()
{
	for (auto& p : INTERNAL_GetCommands()) {
		Log(p.second.name);
		Log(p.second.help, Color::Gray);
	}
}

void Commands::ShowHelp(const String& command)
{
	Commands::CommandInfo info;
	if (STL::TryGetValue(INTERNAL_GetCommands(), command, info)) {
		if (info.help != "") {
			Log(info.help, Color::Gray);
		}
	}
	else {
		Log(StringUtil::Format("Command not found: %s", command.c_str()), Color::Orange);
	}
}

void Commands::AddParamSlider(const String& param, float minValue, float maxValue, float defaultValue)
{
	STL::Add(ms_sliderInfo, Commands::SliderInfo{ param, minValue, maxValue, defaultValue });
}

STL::HashMap<StringID, Commands::CommandInfo>& Commands::INTERNAL_GetCommands()
{
	static STL::HashMap<StringID, Commands::CommandInfo> commands;
	return commands;
}


// Internal Commands

CHERRYSODA_COMMAND(exit, "Exits the game")
{
	Engine::Instance()->Exit();
}

CHERRYSODA_COMMAND(clear, "Clears the terminal")
{
	Commands::ClearLog();
}

CHERRYSODA_COMMAND(vsync, "Enables or disables vertical sync")
{
	if (STL::IsEmpty(args)) return;
	if (args[0] == "0" || args[0] == "false") {
		Graphics::SetVsyncEnabled(false);
		Commands::Log("Vertical Sync Disabled!", Color::Green);
	}
	else if (args[0] == "1" || args[0] == "true") {
		Graphics::SetVsyncEnabled(true);
		Commands::Log("Vertical Sync Enabled!", Color::Green);
	}
}

CHERRYSODA_COMMAND(fullscreen, "Switches to fullscreen mode")
{
	Engine::Instance()->SetFullscreen();
}

CHERRYSODA_COMMAND(window, "Switches to window mode")
{
	Engine::Instance()->SetWindowed();
}

CHERRYSODA_COMMAND(help, "Shows usage help for a given command")
{
	if (STL::IsEmpty(args)) {
		Commands::ShowHelp();
	}
	else {
		Commands::ShowHelp(args[0]);
	}
}

CHERRYSODA_COMMAND(timerate, "Sets the global time rate")
{
	if (STL::IsNotEmpty(args)) {
		float r = 1.f;
		if (StringUtil::SafeTo<float>(args[0], r)) {
			Engine::Instance()->TimeRate(r);
		}
	}
	float timerate = Engine::Instance()->TimeRate();
	Commands::Log(StringUtil::ToString(timerate));
	Commands::Return(timerate);
}

CHERRYSODA_COMMAND(addslider, "Adds parameter slider")
{
	if (STL::IsEmpty(args)) return;
	float l = 0.f, r = 1.f, d = 1.f;
	int argnum = STL::Count(args);
	if (argnum >= 3) {
		StringUtil::SafeTo<float>(args[1], l);
		StringUtil::SafeTo<float>(args[2], r);
		if (l > r) {
			Commands::Log("Min value is larger than max value", Color::Orange);
			return;
		}
		if (argnum >= 4) {
			StringUtil::SafeTo<float>(args[3], d);
			if (d < l || d > r) {
				Commands::Log("Default value not in range", Color::Orange);
			}
		}
		else {
			Commands::ExecuteCommand(args[0]);
			if (Commands::HasReturnValue()) {
				d = Commands::GetReturnedFloat();
			}
			d = Math_Clamp(d, l, r);
		}
	}
	Commands::ExecuteCommand(args[0] + " " + StringUtil::ToString(d));
	Commands::AddParamSlider(args[0], l, r, d);
}
