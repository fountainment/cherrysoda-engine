#include <CherrySoda/Input/MInput.h>

#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/String.h>

#include <SDL2/SDL.h>

using cherrysoda::MInput;

using cherrysoda::StringUtil;

void MInput::Initialize()
{
	SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt");

	SDL_GameController *controller = NULL;
	for (int i = 0; i < SDL_NumJoysticks(); ++i) {
		if (SDL_IsGameController(i)) {
			controller = SDL_GameControllerOpen(i);
			if (controller) {
				CHERRYSODA_DEBUG(StringUtil::Format("Successfully open gamecontroller %i\n", i));
				break;
			} else {
				CHERRYSODA_DEBUG(StringUtil::Format("Could not open gamecontroller %i: %s\n", i, SDL_GetError()));
			}
		}
	}
}