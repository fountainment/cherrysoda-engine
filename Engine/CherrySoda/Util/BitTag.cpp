#include <CherrySoda/Util/BitTag.h>

#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

int BitTag::ms_totalTags = 0;

BitTag BitTag::ms_byID[TagBitsAmount];

} // namespace cherrysoda
