#ifndef _CHERRYSODA_UTIL_XML_H_
#define _CHERRYSODA_UTIL_XML_H_

#include <CherrySoda/Util/String.h>

#include <tinyxml2.h>

namespace cherrysoda {

namespace xml {

using namespace ::tinyxml2;

} // namespace xml

class XMLUtil
{
public:
	static bool ReadXMLFile(xml::XMLDocument& doc, const String& filename);
};

} // namespace cherrysoda


#endif // _CHERRYSODA_UTIL_XML_H_
