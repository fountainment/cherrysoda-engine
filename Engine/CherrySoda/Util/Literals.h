#ifndef _CHERRYSODA_UTIL_LITERALS_H_
#define _CHERRYSODA_UTIL_LITERALS_H_

#include <CherrySoda/Util/NumType.h>

inline namespace cherrysoda_literals {

struct cherrysoda_su_proxy
{
	cherrysoda::type::UInt16 v;
};

constexpr cherrysoda_su_proxy operator "" _su(unsigned long long int lit)
{
	return { static_cast<cherrysoda::type::UInt16>(lit) };
}

constexpr cherrysoda::type::UInt16 operator + (cherrysoda::type::UInt16 a, cherrysoda_su_proxy b)
{
	return static_cast<cherrysoda::type::UInt16>(a + b.v);
}

} // namespace cherrysoda_literals

#endif // _CHERRYSODA_UTIL_LITERALS_H_
