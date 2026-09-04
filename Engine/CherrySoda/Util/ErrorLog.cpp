#include <CherrySoda/Util/ErrorLog.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Util/Calc.h>
#include <CherrySoda/Util/Time.h>

#include <cstdio>
#include <cstring>
#include <ctime>
#include <fstream>
#include <sstream>

#ifdef __APPLE__
#define CHERRYSODA_ERRORLOG_OPEN_COMMAND "open \"%s\""
#elif defined(_WIN32)
#define CHERRYSODA_ERRORLOG_OPEN_COMMAND "start \"\" \"%s\""
#else
#define CHERRYSODA_ERRORLOG_OPEN_COMMAND "xdg-open \"%s\""
#endif

namespace cherrysoda {

String ErrorLog::ms_filename = "error_log.txt";

void ErrorLog::Write(const String& str)
{
	// Keep the previous errors when the existing file carries our marker
	String content;
	{
		std::ifstream file(ms_filename);
		if (file.is_open()) {
			std::stringstream buffer;
			buffer << file.rdbuf();
			content = buffer.str();
			if (content.find(Marker) == String::npos) {
				content.clear();
			}
		}
	}

	std::stringstream output;
	output << (Engine::Instance() != nullptr ? Engine::Instance()->GetTitle() : String("CherrySoda Engine"))
		   << " Error Log\n"
		   << Marker << "\n\n";

	auto now = static_cast<std::time_t>(Time::GetSystemTime());
	char timeBuffer[64];
	(void)std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
	output << timeBuffer << "\n";

	output << str << "\n";

	if (!content.empty()) {
		size_t at = content.find(Marker) + std::strlen(Marker) - 1;
		output << content.substr(at);
	}

	std::ofstream file(ms_filename, std::ofstream::out | std::ofstream::trunc);
	file << output.str();
}

void ErrorLog::Open()
{
	if (Calc::FileExists(ms_filename)) {
		char command[512];
		(void)std::snprintf(command, sizeof(command), CHERRYSODA_ERRORLOG_OPEN_COMMAND, ms_filename.c_str());
		(void)std::system(command); // NOLINT(bugprone-command-processor,cert-env33-c) — intentionally open the log file
	}
}

} // namespace cherrysoda
