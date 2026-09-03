#ifndef _CHERRYSODA_UTIL_XML_H_
#define _CHERRYSODA_UTIL_XML_H_

#include <CherrySoda/Util/String.h>

#include <pugixml.hpp>

namespace cherrysoda {

namespace xml {

using namespace ::pugi;

} // namespace xml

class XMLUtil
{
public:
	static bool ReadXMLFile(xml::xml_document& doc, const String& filename);
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_XML_H_
