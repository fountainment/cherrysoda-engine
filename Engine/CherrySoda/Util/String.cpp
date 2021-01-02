#include <CherrySoda/Util/String.h>

#include <CherrySoda/Util/STL.h>

#include <cstdarg>
#include <sstream>
#include <string>
#include <utility>

using cherrysoda::STL;
using cherrysoda::String;
using cherrysoda::StringID;
using cherrysoda::StringUtil;

const String StringUtil::Format(const char* format, ...)
{
	char buffer[256];
	va_list args;
	va_start(args, format);
	std::vsnprintf(buffer, 256, format, args);
	va_end(args);
	return String(buffer);
}

const STL::Vector<String> StringUtil::Split(const String& s, char delim/* = ' '*/)
{
	std::stringstream ss(s);
	std::string item;
	STL::Vector<String> elems;
	while (std::getline(ss, item, delim)) {
		STL::Add(elems, static_cast<String>(item));
	}
	return elems;
}
