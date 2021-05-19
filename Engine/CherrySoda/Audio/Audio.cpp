#include <CherrySoda/Audio/Audio.h>

#include <CherrySoda/Util/String.h>
#include <CherrySoda/Util/NumType.h>

#include <cmixer.h>
#include <SDL.h>

#include <cstring>

using cherrysoda::Audio;


static SDL_mutex* audio_mutex;

static void lock_handler(cm_Event *e)
{
	if (e->type == CM_EVENT_LOCK) {
		SDL_LockMutex(audio_mutex);
	}
	if (e->type == CM_EVENT_UNLOCK) {
		SDL_UnlockMutex(audio_mutex);
	}
}

static void audio_callback(void *udata, cherrysoda::type::UInt8 *stream, int size)
{
	cm_process((cherrysoda::type::Int16*) stream, size / 2);
}

void Audio::Initialize()
{
	SDL_AudioDeviceID dev;
	SDL_AudioSpec fmt, got;

	/* Init SDL */
	SDL_Init(SDL_INIT_AUDIO);
	audio_mutex = SDL_CreateMutex();

	/* Init SDL audio */
	std::memset(&fmt, 0, sizeof(fmt));
	fmt.freq      = 44100;
	fmt.format    = AUDIO_S16;
	fmt.channels  = 2;
	fmt.samples   = 1024;
	fmt.callback  = audio_callback;	

	dev = SDL_OpenAudioDevice(NULL, 0, &fmt, &got, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
	CHERRYSODA_ASSERT_FORMAT(dev, "Error: failed to open audio device '%s'\n", SDL_GetError());

	/* Init library */
	cm_init(got.freq);
	cm_set_lock(lock_handler);
	cm_set_master_gain(0.5);

	/* Start audio */
	SDL_PauseAudioDevice(dev, 0);
}
