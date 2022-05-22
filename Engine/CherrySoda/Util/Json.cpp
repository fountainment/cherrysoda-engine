#include <CherrySoda/Util/Json.h>

#include <CherrySoda/Util/String.h>

#include <rapidjson/istreamwrapper.h>
#include <fstream>

namespace cherrysoda {

void JsonUtil::ReadJsonFile(json::Document& doc, const String& filename)
{
	std::ifstream ifs(filename);
	json::IStreamWrapper isw(ifs);
	doc.ParseStream(isw);
}

} // namespace cherrysoda
