#ifndef _CHERRYSODA_UTIL_JSON_H_
#define _CHERRYSODA_UTIL_JSON_H_

#include <CherrySoda/Util/String.h>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <fstream>

namespace cherrysoda {

namespace json = rapidjson;

class JsonUtil
{
public:
	static void ReadJsonFile(json::Document& doc, const String& filename)
	{
		std::ifstream ifs(filename);
		json::IStreamWrapper isw(ifs);
		doc.ParseStream(isw);	
	}
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_JSON_H_
