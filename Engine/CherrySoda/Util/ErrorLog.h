#ifndef _CHERRYSODA_UTIL_ERRORLOG_H_
#define _CHERRYSODA_UTIL_ERRORLOG_H_

#include <CherrySoda/Util/String.h>

namespace cherrysoda {

// Appends engine errors to error_log.txt, keeping the previous entries
// below the new one. Port of Monocle's ErrorLog.
class ErrorLog
{
public:
	static constexpr const char* Marker = "==========================================";

	static inline const String& Filename() { return ms_filename; }
	static inline void Filename(const String& filename) { ms_filename = filename; }

	static void Write(const String& str);

	// Opens the log with the platform's default application
	static void Open();

private:
	static String ms_filename;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_ERRORLOG_H_
