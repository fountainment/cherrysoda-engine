#ifndef _CHERRYSODA_UTIL_JSON_H_
#define _CHERRYSODA_UTIL_JSON_H_

#include <CherrySoda/Util/String.h>

#include <rapidjson/document.h>

namespace cherrysoda {

namespace json = rapidjson;

class JsonUtil
{
public:
	static void ReadJsonFile(json::Document& doc, const String& filename);
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_JSON_H_
