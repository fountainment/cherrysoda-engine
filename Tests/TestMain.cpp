#include <CherrySoda/CherrySoda.h>
#include <gtest/gtest.h>

#ifdef _MSC_VER
	#pragma comment(linker, "/NODEFAULTLIB:LIBCMT.lib")
	#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
	#ifndef NDEBUG
		#pragma comment(linker, "/NODEFAULTLIB:MSVCRT.lib")
	#endif
#endif

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
