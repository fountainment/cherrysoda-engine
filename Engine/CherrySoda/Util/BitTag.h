#ifndef _CHERRYSODA_UTIL_BITTAG_H_
#define _CHERRYSODA_UTIL_BITTAG_H_

#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/String.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

typedef type::UInt32 BitTagValueType;

static constexpr int TagBitsAmount = sizeof(BitTagValueType) * 8;

class BitTag
{
public:
	BitTag()
	{
	}

	BitTag(const StringID& name)
	{
#ifndef NDEBUG // Debug
		m_name = name.GetStr();

		CHERRYSODA_ASSERT(ms_totalTags < TagBitsAmount, "Maximum tag limit exceeded!\n");
		CHERRYSODA_ASSERT(!STL::Contains(ms_byName, name.GetID()), "Two tags defined with the same name: '" + m_name + "'!\n");
#endif
		m_id = ms_totalTags;
		m_value = 1 << ms_totalTags;

		ms_byID[m_id] = *this;
		ms_byName[name] = *this;

		ms_totalTags++;
	}

	operator BitTagValueType() const { return m_value; }

	static BitTag Get(const StringID& name)
	{
#ifndef NDEBUG // Debug
		CHERRYSODA_ASSERT(STL::Contains(ms_byName, name.GetID()), "No tag with the name '" + name.GetStr() + "' has been defined!\n");
#endif
		return ms_byName[name];
	}

private:
	int m_id = 0;
	BitTagValueType m_value = 0;
#ifndef NDEBUG // Debug
	String m_name;
#endif
	static int ms_totalTags;

	static BitTag ms_byID[TagBitsAmount];
	static STL::Map<int,BitTag> ms_byName;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_BITTAG_H_
