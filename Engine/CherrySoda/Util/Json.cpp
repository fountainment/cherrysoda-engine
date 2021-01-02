#include <CherrySoda/Util/Json.h>

#include <CherrySoda/Util/String.h>

#include <rapidjson/istreamwrapper.h>
#include <fstream>

using cherrysoda::JsonUtil;
using cherrysoda::String;

void JsonUtil::ReadJsonFile(cherrysoda::json::Document& doc, const String& filename)
{
	std::ifstream ifs(filename);
	cherrysoda::json::IStreamWrapper isw(ifs);
	doc.ParseStream(isw);
}
