#ifndef _CHERRYSODA_UTIL_STRING_H_
#define _CHERRYSODA_UTIL_STRING_H_

#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>

#include <sstream>
#include <string>

#define CHERRYSODA_FORMAT cherrysoda::StringUtil::Format
#define CHERRYSODA_DEBUG_FORMAT(format,...)            CHERRYSODA_DEBUG(CHERRYSODA_FORMAT(format,##__VA_ARGS__))
#define CHERRYSODA_LOG_FORMAT(format,...)              CHERRYSODA_LOG(CHERRYSODA_FORMAT(format,##__VA_ARGS__))
#define CHERRYSODA_ASSERT_FORMAT(condition,format,...) CHERRYSODA_ASSERT(condition,CHERRYSODA_FORMAT(format,##__VA_ARGS__))

#ifdef CHERRYSODA_ENABLE_DEBUG
#	define CHERRYSODA_STRINGID_CONSTEXPR inline
#else
#	define CHERRYSODA_STRINGID_CONSTEXPR constexpr
#endif

namespace cherrysoda {

typedef std::string String;

class StringUtil
{
public:
	static constexpr type::Int32 GetHashBKDR(const char* str, int len = -1)
	{
		type::Int32 seed = 131;
		type::Int32 hash = 0;
		for (int i = 0; (len == -1 || i < len) && str[i]; ++i) {
			hash = static_cast<type::Int32>(static_cast<type::Int64>(hash) * seed + str[i]);
		}
		return hash & 0x7fffffff;
	}

	static constexpr type::UInt32 HexStrToUInt32(const char* str, int len = -1)
	{
		type::UInt32 ans = 0;
		for (int i = 0; (len == -1 || i < len) && str[i]; ++i) {
			ans <<= 4;
			int tmp = 0;
			if (str[i] >= 'a' && str[i] <= 'f') {
				tmp = str[i] - 'a' + 10;
			} else if (str[i] >= 'A' && str[i] <= 'F') {
				tmp = str[i] - 'A' + 10;
			} else if (str[i] >= '0' && str[i] <= '9') {
				tmp = str[i] - '0';
			} else {
				CHERRYSODA_ASSERT(false, "Hex string invalid.\n");
			}
			ans |= tmp;
		}
		return ans;
	}

	static const String Format(const char* format, ...);
	static const STL::Vector<String> Split(const String& s, char delim = ' ');
	static inline int IndexOf(const String& s, char c) { return s.find(c, 0); }
	static const String Trim(const String& s, char trim = ' ');

	static float ToFloat(const String& s) { return std::stof(s); }
	static int ToInt(const String& s) { return std::stoi(s); }

	template <typename T>
	static const String ToString(T t) { return std::to_string(t); }

	template <typename T>
	static bool SafeTo(const String& s, T& t)
	{
		T val;
		std::stringstream ss;
		ss << s;
		ss >> val;
		if(ss.fail()) {
			return false;
		}
		t = val;
		return true;
	}

	static const String ToLower(const String& s);
	static const String ToUpper(const String& s);

	// path utils
	static const String Path_GetDirectoryName(const String& path);
	static const String Path_GetFileName(const String& path);
	static const String Path_GetFileNameWithoutExtension(const String& path);

	// converter utils
	static const STL::Vector<type::UInt32> Utf8ToUnicode32(const String& text);
};

class StringID
{
public:
	inline StringID() : StringID("") {}

	inline StringID(const String& str) : StringID(str.c_str()) {}

	constexpr type::Int32 GetID() const { return m_id; }
	constexpr operator type::Int32() const { return GetID(); }
	constexpr bool operator ==(const StringID& other) const { return GetID() == other.GetID(); }
	constexpr bool IsEmpty() const { return GetID() == 0; }

#ifdef CHERRYSODA_ENABLE_DEBUG
public:
	StringID(const char* str)
	: m_id(StringUtil::GetHashBKDR(str))
	, m_str(String(str))
	{
		STL::Map<type::Int32,String>& hashCollisionCheckMap = INTERNAL_GetHashCollisionCheckMap();
		auto it = STL::FindKey(hashCollisionCheckMap, m_id);
		if (it != hashCollisionCheckMap.end()) {
			CHERRYSODA_ASSERT_FORMAT(it->second == m_str, \
				"StringID Hash Collision: \"%s\" collide with \"%s\" at %d!", \
				m_str.c_str(), it->second.c_str(), m_id);
		}
		else {
			hashCollisionCheckMap[m_id] = m_str;
		}
	}

	inline String GetStr() const { return m_str; }

#else // CHERRYSODA_ENABLE_DEBUG
public:
	constexpr StringID(const char* str)
	: m_id(StringUtil::GetHashBKDR(str))
	{}

	inline String GetStr() const { return ""; }
#endif // CHERRYSODA_ENABLE_DEBUG

private:
	type::Int32 m_id;

#ifdef CHERRYSODA_ENABLE_DEBUG
	static STL::Map<type::Int32,String>& INTERNAL_GetHashCollisionCheckMap();

	String m_str;
#endif // CHERRYSODA_ENABLE_DEBUG
};

} // namespace cherrysoda

CHERRYSODA_HASHABLE(cherrysoda::StringID, GetID);

#endif // _CHERRYSODA_UTILITY_STRING_H_
