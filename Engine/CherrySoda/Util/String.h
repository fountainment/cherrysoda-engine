#ifndef _CHERRYSODA_UTIL_STRING_H_
#define _CHERRYSODA_UTIL_STRING_H_

#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/STL.h>

#include <string>

#define CHERRYSODA_FORMAT cherrysoda::StringUtil::Format
#define CHERRYSODA_DEBUG_FORMAT(format, ...) CHERRYSODA_DEBUG(CHERRYSODA_FORMAT(format, ##__VA_ARGS__))
#define CHERRYSODA_LOG_FORMAT(format, ...) CHERRYSODA_LOG(CHERRYSODA_FORMAT(format, ##__VA_ARGS__))

namespace cherrysoda {

typedef std::string String;

constexpr int GetHashBKDR(const char* str)
{
	int seed = 131;
	int hash = 0;
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
#ifdef NDEBUG
	constexpr StringID(const char* str)
	: m_id(GetHashBKDR(str))
	{
	}
#else
	StringID(const char* str)
	: m_id(GetHashBKDR(str))
	, m_str(String(str))
	{
		auto it = STL::Find(ms_hashCollisionCheckMap, m_id);
		if (it != ms_hashCollisionCheckMap.end()) {
			if (it->second != m_str) {
				CHERRYSODA_DEBUG_FORMAT( \
					"StringID Hash Collision: \"%s\" collide with \"%s\" at %d!", \
					m_str.c_str(), it->second.c_str(), m_id);
			}
		}
		ms_hashCollisionCheckMap[m_id] = m_str;
	}

	inline String GetStr() const { return m_str; }
#endif

	inline int GetID() const { return m_id; }

	bool operator = (const StringID& strID)
	{
		return GetID() == strID.GetID();
	}

	operator int() const { return GetID(); }

private:
	int m_id;
#ifndef NDEBUG
	String m_str;

	static STL::Map<int,String> ms_hashCollisionCheckMap;
#endif
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTILITY_STRING_H_
