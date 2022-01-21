#include <CherrySoda/Util/XML.h>

using cherrysoda::String;
using cherrysoda::XMLUtil;


bool XMLUtil::ReadXMLFile(cherrysoda::xml::XMLDocument& doc, const String& filename)
{
	cherrysoda::xml::XMLError result = doc.LoadFile(filename.c_str());
	if (result != cherrysoda::xml::XMLError::XML_SUCCESS) {
		// Clean up.
		doc.Clear();
		return false;
	}

	return true;
}
