#ifndef _CHERRYSODA_UTIL_STRING_H_
#define _CHERRYSODA_UTIL_STRING_H_

#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>

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

constexpr type::Int32 GetHashBKDR(const char* str)
{
	type::Int32 seed = 131;
	type::Int32 hash = 0;
	for (int i = 0; str[i]; ++i) {
		hash = hash * seed + str[i];
	}
	return hash & 0x7fffffff;
}

class StringUtil
{
public:
	static const String Format(const char* format, ...);
};

class StringID
{
public:
	inline StringID() : StringID("") {}

	inline StringID(const String& str) : StringID(str.c_str()) {}

	constexpr type::Int32 GetID() const { return m_id; }
	constexpr operator type::Int32() const { return GetID(); }
	constexpr bool operator ==(const StringID& other) const { return GetID() == other.GetID(); }

#ifdef CHERRYSODA_ENABLE_DEBUG
public:
	StringID(const char* str)
	: m_id(GetHashBKDR(str))
	, m_str(String(str))
	{
		static STL::Map<type::Int32,String> s_hashCollisionCheckMap;
		auto it = STL::FindKey(s_hashCollisionCheckMap, m_id);
		if (it != s_hashCollisionCheckMap.end()) {
			CHERRYSODA_ASSERT_FORMAT( it->second == m_str, \
				"StringID Hash Collision: \"%s\" collide with \"%s\" at %d!", \
				m_str.c_str(), it->second.c_str(), m_id);
		}
		else {
			s_hashCollisionCheckMap[m_id] = m_str;
		}
	}

	inline String GetStr() const { return m_str; }

#else // CHERRYSODA_ENABLE_DEBUG
public:
	constexpr StringID(const char* str)
	: m_id(GetHashBKDR(str))
	{}

	inline String GetStr() const { return ""; }
#endif // CHERRYSODA_ENABLE_DEBUG

private:
	type::Int32 m_id;

#ifdef CHERRYSODA_ENABLE_DEBUG
	String m_str;
#endif // CHERRYSODA_ENABLE_DEBUG
};

} // namespace cherrysoda

CHERRYSODA_HASHABLE(cherrysoda::StringID, GetID);

#endif // _CHERRYSODA_UTILITY_STRING_H_
