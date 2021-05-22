#include <CherrySoda/Util/Commands.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/Color.h>
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


char Commands::ms_currentText[512] = "";
STL::Vector<STL::Pair<Color,String>> Commands::ms_drawCommands;
bool Commands::ms_consoleTextScrollNeeded = false;

int Commands::ExecuteCommand()
{
	String currentText(ms_currentText);
	ms_currentText[0] = '\0';
	auto data = StringUtil::Split(currentText);
	Log("cherrysoda # " + currentText);
	Commands::CommandInfo info;
	if (STL::TryGetValue(INTERNAL_GetCommands(), StringUtil::ToLower(data[0]), info)) {
		return info.func(STL::Vector<String>(data.begin() + 1, data.end()));
	}
	else {
		Log(StringUtil::Format("Command not found: %s", data[0].c_str()), Color::Orange);
	}
	return 1;
}

void Commands::Log(const String& str, const Color& color/* = Color::White*/)
{
	auto lines = StringUtil::Split(str, '\n');
	for (auto& line : lines) {
		STL::Add(ms_drawCommands, STL::MakePair(color, line));
	}
	ms_consoleTextScrollNeeded = true;
}

void Commands::ClearLog()
{
	STL::Clear(ms_drawCommands);
}

void Commands::ShowHelp()
{
	// TODO: List commands
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

STL::HashMap<StringID, Commands::CommandInfo>& Commands::INTERNAL_GetCommands()
{
	static STL::HashMap<StringID, Commands::CommandInfo> commands;
	return commands;
}


// Internal Commands

CHERRYSODA_COMMAND(exit, "Exits the game")
{
	Engine::Instance()->Exit();
	return 0;
}

CHERRYSODA_COMMAND(clear, "Clears the terminal")
{
	Commands::ClearLog();
	return 0;
}

CHERRYSODA_COMMAND(vsync, "Enables or disables vertical sync")
{
	if (STL::IsEmpty(args)) return 1;
	if (args[0] == "0" || args[0] == "false") {
		Graphics::SetVsyncEnabled(false);
		Commands::Log("Vertical Sync Disabled!", Color::Green);
	}
	else if (args[0] == "1" || args[0] == "true") {
		Graphics::SetVsyncEnabled(true);
		Commands::Log("Vertical Sync Enabled!", Color::Green);
	}
	return 0;
}

CHERRYSODA_COMMAND(fullscreen, "Switches to fullscreen mode")
{
	Engine::Instance()->SetFullscreen();
	return 0;
}

CHERRYSODA_COMMAND(window, "Switches to window mode")
{
	Engine::Instance()->SetWindowed();
	return 0;
}

CHERRYSODA_COMMAND(help, "Shows usage help for a given command")
{
	if (STL::Count(args) < 1) {
		Commands::ShowHelp();
	}
	else {
		Commands::ShowHelp(args[0]);
	}
	return 0;
}
