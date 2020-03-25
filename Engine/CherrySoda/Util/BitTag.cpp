#include <CherrySoda/Util/BitTag.h>

#include <CherrySoda/Util/STL.h>

using cherrysoda::BitTag;
using cherrysoda::STL;

int BitTag::ms_totalTags = 0;

BitTag BitTag::ms_byID[cherrysoda::TagBitsAmount];
