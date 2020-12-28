#ifndef _CHERRYSODA_MAIN_H_
#define _CHERRYSODA_MAIN_H_

#include <SDL2/SDL_Main.h>

#define CHERRYSODA_DEFAULT_MAIN \
int main(int argc, char* argv[]) \
{ GameApp* game = new GameApp; game->Run(argc, argv); return 0; }

#endif // _CHERRYSODA_MAIN_H_
