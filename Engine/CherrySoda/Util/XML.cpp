#include <CherrySoda/Util/XML.h>

namespace cherrysoda {

bool XMLUtil::ReadXMLFile(xml::XMLDocument& doc, const String& filename)
{
	xml::XMLError result = doc.LoadFile(filename.c_str());
	if (result != xml::XMLError::XML_SUCCESS) {
		// Clean up.
		doc.Clear();
		return false;
	}

	return true;
}

} // namespace cherrysoda
