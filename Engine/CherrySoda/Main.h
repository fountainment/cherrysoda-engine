#ifndef _CHERRYSODA_MAIN_H_
#define _CHERRYSODA_MAIN_H_

// The engine provides its own main() entry via CHERRYSODA_DEFAULT_MAIN,
// so SDL's main handling is disabled on every platform.
#define SDL_MAIN_HANDLED

#include <SDL3/SDL_main.h>

#ifdef _MSC_VER
	#pragma comment(linker, "/NODEFAULTLIB:LIBCMT.lib")
	#ifdef NDEBUG
		#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
	#else
		#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
		#pragma comment(linker, "/NODEFAULTLIB:MSVCRT.lib")
	#endif
#endif

#define CHERRYSODA_DEFAULT_MAIN \
int main(int argc, char* argv[]) \
{ \
	GameApp* game = new GameApp; \
	game->Run(argc, argv); \
	delete game; \
	return 0; \
}

#endif // _CHERRYSODA_MAIN_H_
