#include <CherrySoda/Util/XML.h>

#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/String.h>

namespace cherrysoda {

bool XMLUtil::ReadXMLFile(xml::xml_document& doc, const String& filename)
{
	xml::xml_parse_result result = doc.load_file(filename.c_str());
	if (!result) {
		CHERRYSODA_LOG(CHERRYSODA_FORMAT("XMLUtil::ReadXMLFile: failed to parse \"%s\": %s\n", filename.c_str(),
										  result.description()));
		doc.reset();
		return false;
	}

	return true;
}

} // namespace cherrysoda
