#include <CherrySoda/Util/String.h>

#include <CherrySoda/Util/STL.h>

#include <algorithm>
#include <cctype>
#include <cstdarg>
#include <sstream>
#include <string>
#include <utility>

using cherrysoda::STL;
using cherrysoda::String;
using cherrysoda::StringID;
using cherrysoda::StringUtil;

namespace type = cherrysoda::type;


#ifdef CHERRYSODA_ENABLE_DEBUG
STL::Map<type::Int32,String>& StringID::INTERNAL_GetHashCollisionCheckMap()
{
	static STL::Map<type::Int32,String> hashCollisionCheckMap;
	return hashCollisionCheckMap;	
}
#endif // CHERRYSODA_ENABLE_DEBUG

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

const String StringUtil::Trim(const String& s, char trim/* = ' '*/)
{
	int li = 0, ri = s.length() - 1;
	while (s[li] == trim) ++li;
	while (s[ri] == trim && ri > li) --ri;
	return s.substr(li, ri - li + 1);
}

const String StringUtil::ToLower(const String& s)
{
	String ret = s;
	std::transform(s.begin(), s.end(), ret.begin(),
		[](unsigned char c){ return std::tolower(c); });
	return ret;
}

const String StringUtil::ToUpper(const String& s)
{
	String ret = s;
	std::transform(s.begin(), s.end(), ret.begin(),
		[](unsigned char c){ return std::toupper(c); });
	return ret;
}

const String StringUtil::Path_GetDirectoryName(const String& path)
{
	return path.substr(0, path.find_last_of("/\\") + 1);
}

const String StringUtil::Path_GetFileName(const String& path)
{
	int len = path.length();
	int filenameLoc = path.find_last_of("/\\") + 1;
	return path.substr(filenameLoc, len - filenameLoc);
}

const String StringUtil::Path_GetFileNameWithoutExtension(const String& path)
{
	String filename = Path_GetFileName(path);
	int dotLoc = path.find_last_of(".");
	if (dotLoc == -1) {
		dotLoc = filename.length();
	}
	return path.substr(0, dotLoc);
}

const STL::Vector<type::UInt32> StringUtil::Utf8ToUnicode32(const String& text)
{
	const type::UInt8 rOne[] = {0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff};
	int length = text.length();
	int index = 0;
	STL::Vector<type::UInt32> result;
	while (index < length) {
		type::UInt8 u8Tmp = static_cast<type::UInt8>(text[index]);
		type::UInt32 u32Tmp;
		int fz = 0;
		while (u8Tmp & (1 << (7 - fz))) fz++;
		CHERRYSODA_ASSERT(fz != 1 && fz < 8, "Error decoding utf-8 text.\n");
		u32Tmp = static_cast<type::UInt32>(u8Tmp & rOne[7 - fz]);
		for (int offset = 1; offset < fz; offset++) {
			u32Tmp <<= 6;
			CHERRYSODA_ASSERT(index + offset < length, "Error decoding utf-8 text.\n");
			u32Tmp |= static_cast<type::UInt8>(text[index + offset]) & rOne[6];
		}
		STL::Add(result, u32Tmp);
		if (fz) {
			index += fz;
		} else {
			index++;
		}
	}
	return result;
}
