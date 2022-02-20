#include <CherrySoda/Audio/Audio.h>

#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>
#include <CherrySoda/Util/NumType.h>

#include <cmixer.h>
#include <SDL.h>

#include <cstring>

using cherrysoda::Audio;

using cherrysoda::STL;
using cherrysoda::String;
using cherrysoda::StringID;


double Audio::ms_masterVolume = 0.5;
bool Audio::ms_initialized = false;

static SDL_AudioDeviceID s_sdlAudioDev;

static int s_sourceCount = 0;
static STL::HashMap<StringID, Audio::EventDescription> s_descriptions;
static STL::HashMap<int, cm_Source*> s_sources;

Audio::EventInstance Audio::EventDescription::CreateInstance()
{
	int id = s_sourceCount++;
	if (data != nullptr) {
		s_sources[id] = cm_new_source_from_mem(data, size);
	}
	else {
		s_sources[id] = cm_new_source_from_file(filename.c_str());
	}
	return { id };
}

static SDL_mutex* audio_mutex;

static void lock_handler(cm_Event* e)
{
	if (e->type == CM_EVENT_LOCK) {
		SDL_LockMutex(audio_mutex);
	}
	if (e->type == CM_EVENT_UNLOCK) {
		SDL_UnlockMutex(audio_mutex);
	}
}

static void audio_callback(void* udata, cherrysoda::type::UInt8* stream, int size)
{
	cm_process((cherrysoda::type::Int16*) stream, size / 2);
}

void Audio::Initialize()
{
	SDL_AudioSpec fmt, got;

	/* Init SDL */
	SDL_InitSubSystem(SDL_INIT_AUDIO);
	audio_mutex = SDL_CreateMutex();

	/* Init SDL audio */
	std::memset(&fmt, 0, sizeof(fmt));
	fmt.freq      = 44100;
	fmt.format    = AUDIO_S16;
	fmt.channels  = 2;
	fmt.samples   = 1024;
	fmt.callback  = audio_callback;

	s_sdlAudioDev = SDL_OpenAudioDevice(NULL, 0, &fmt, &got, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
	CHERRYSODA_ASSERT_FORMAT(s_sdlAudioDev, "Error: failed to open audio device '%s'\n", SDL_GetError());

	/* Init library */
	cm_init(got.freq);
	cm_set_lock(lock_handler);
	cm_set_master_gain(ms_masterVolume);

	/* Start audio */
	SDL_PauseAudioDevice(s_sdlAudioDev, 0);

	ms_initialized = true;
}

void Audio::Terminate()
{
	for (auto source : s_sources) {
		cm_destroy_source(source.second);
	}
	STL::Clear(s_sources);
	s_sourceCount = 0;

	if (ms_initialized) {
		SDL_CloseAudioDevice(s_sdlAudioDev);
		s_sdlAudioDev = 0;

		SDL_DestroyMutex(audio_mutex);
		audio_mutex = nullptr;

		SDL_QuitSubSystem(SDL_INIT_AUDIO);

		ms_initialized = false;
	}
}

void Audio::MasterVolume(double volume)
{
	ms_masterVolume = volume;
	cm_set_master_gain(volume);
}

void Audio::LoadFile(const StringID& path, const String& filePath)
{
	s_descriptions[path] = Audio::EventDescription{ filePath };
}

void Audio::LoadFileFromMemory(const StringID& path, void* data, int size)
{
	s_descriptions[path] = Audio::EventDescription{ "", (cherrysoda::type::UInt8*)data, size };
}

Audio::EventInstance Audio::Play(const StringID& path, double volume/* = 1.0*/, double pitch/* = 1.0*/, double pan/* = 0.0*/)
{
	Audio::EventInstance instance = CreateInstance(path, volume, pitch, pan);
	cm_play(s_sources[instance.id]);
	return instance;
}

Audio::EventInstance Audio::Loop(const StringID& path, double volume/* = 1.0*/, double pitch/* = 1.0*/, double pan/* = 0.0*/)
{
	Audio::EventInstance instance = CreateLoopInstance(path, volume, pitch, pan);
	cm_play(s_sources[instance.id]);
	return instance;
}

Audio::EventInstance Audio::CreateInstance(const StringID& path, double volume/* = 1.0*/, double pitch/* = 1.0*/, double pan/* = 0.0*/)
{
	Audio::EventInstance instance = s_descriptions[path].CreateInstance();
	cm_Source* src = s_sources[instance.id];
	cm_set_gain(src, volume);
	cm_set_pitch(src, pitch);
	cm_set_pan(src, pan);
	return instance;
}

Audio::EventInstance Audio::CreateLoopInstance(const StringID& path, double volume/* = 1.0*/, double pitch/* = 1.0*/, double pan/* = 0.0*/)
{
	Audio::EventInstance instance = s_descriptions[path].CreateInstance();
	cm_Source* src = s_sources[instance.id];
	cm_set_gain(src, volume);
	cm_set_pitch(src, pitch);
	cm_set_pan(src, pan);
	cm_set_loop(src, 1);
	return instance;
}

double Audio::GetLength(Audio::EventInstance instance)
{
	cm_Source* src = s_sources[instance.id];
	return cm_get_length(src);
}

double Audio::GetPosition(Audio::EventInstance instance)
{
	cm_Source* src = s_sources[instance.id];
	return cm_get_position(src);
}

void Audio::SetParam(Audio::EventInstance instance, double volume, double pitch, double pan)
{
	cm_Source* src = s_sources[instance.id];
	cm_set_gain(src, volume);
	cm_set_pitch(src, pitch);
	cm_set_pan(src, pan);
}

void Audio::SetVolume(Audio::EventInstance instance, double volume)
{
	cm_Source* src = s_sources[instance.id];
	cm_set_gain(src, volume);
}

void Audio::SetPitch(Audio::EventInstance instance, double pitch)
{
	cm_Source* src = s_sources[instance.id];
	cm_set_pitch(src, pitch);
}

void Audio::SetPan(Audio::EventInstance instance, double pan)
{
	cm_Source* src = s_sources[instance.id];
	cm_set_pan(src, pan);
}

void Audio::SetLoop(Audio::EventInstance instance, bool loop)
{
	cm_Source* src = s_sources[instance.id];
	cm_set_loop(src, loop ? 1 : 0);
}

bool Audio::IsPlaying(Audio::EventInstance instance)
{
	cm_Source* src = s_sources[instance.id];
	return cm_get_state(src) == CM_STATE_PLAYING;
}

bool Audio::IsPaused(Audio::EventInstance instance)
{
	cm_Source* src = s_sources[instance.id];
	return cm_get_state(src) == CM_STATE_PAUSED;
}

bool Audio::IsStopped(Audio::EventInstance instance)
{
	cm_Source* src = s_sources[instance.id];
	return cm_get_state(src) == CM_STATE_STOPPED;
}

void Audio::Pause(Audio::EventInstance instance)
{
	cm_Source* src = s_sources[instance.id];
	cm_pause(src);
}

void Audio::Resume(Audio::EventInstance instance)
{
	cm_Source* src = s_sources[instance.id];
	cm_play(src);
}

void Audio::Stop(Audio::EventInstance instance)
{
	cm_Source* src = s_sources[instance.id];
	cm_stop(src);
}
