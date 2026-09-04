#include <CherrySoda/Audio/Audio.h>

#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

#include <SDL3/SDL.h>
#include <cmixer.h>

#include <algorithm>
#include <cstring>

namespace cherrysoda {

double Audio::ms_masterVolume = 0.5;
bool Audio::ms_initialized = false;

static SDL_AudioStream* s_sdlAudioStream;

static int s_sourceCount = 0;
static STL::HashMap<StringID, Audio::EventDescription> s_descriptions;
static STL::HashMap<int, cm_Source*> s_sources;
// Instances fired by Audio::Play(), reclaimed by Audio::Update() when finished
static STL::HashSet<int> s_autoRelease;

static cm_Source* GetSource(int id)
{
	cm_Source* source = nullptr;
	STL::TryGetValue(s_sources, id, source);
	return source;
}

Audio::EventInstance Audio::EventDescription::CreateInstance() const
{
	cm_Source* source = nullptr;
	if (data != nullptr) {
		source = cm_new_source_from_mem(data, size);
	}
	else {
		source = cm_new_source_from_file(filename.c_str());
	}
	if (source == nullptr) {
		CHERRYSODA_LOG(CHERRYSODA_FORMAT("Audio: failed to create source from \"%s\"!\n",
										 filename.empty() ? "<memory>" : filename.c_str()));
		return {-1};
	}
	int id = s_sourceCount++;
	s_sources[id] = source;
	return {id};
}

static SDL_Mutex* audio_mutex;

static void lock_handler(cm_Event* e)
{
	if (e->type == CM_EVENT_LOCK) {
		SDL_LockMutex(audio_mutex);
	}
	if (e->type == CM_EVENT_UNLOCK) {
		SDL_UnlockMutex(audio_mutex);
	}
}

static type::Int16 audio_buffer[4096];

static void SDLCALL audio_callback(void* userdata, SDL_AudioStream* stream, int additional_amount, int total_amount)
{
	(void)userdata;
	(void)total_amount;
	while (additional_amount > 0) {
		int bytes = additional_amount;
		bytes = std::min(bytes, (int)sizeof(audio_buffer));
		cm_process(audio_buffer, bytes / 2);
		SDL_PutAudioStreamData(stream, audio_buffer, bytes);
		additional_amount -= bytes;
	}
}

void Audio::Initialize()
{
	SDL_AudioSpec fmt;

	/* Init SDL */
	SDL_InitSubSystem(SDL_INIT_AUDIO);
	audio_mutex = SDL_CreateMutex();

	/* Init SDL audio */
	fmt.format = SDL_AUDIO_S16;
	fmt.channels = 2;
	fmt.freq = 44100;

	s_sdlAudioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &fmt, audio_callback, nullptr);
	CHERRYSODA_ASSERT_FORMAT(s_sdlAudioStream, "Error: failed to open audio device '%s'\n", SDL_GetError());

	/* Init library */
	cm_init(fmt.freq);
	cm_set_lock(lock_handler);
	cm_set_master_gain(ms_masterVolume);

	/* Start audio */
	SDL_ResumeAudioStreamDevice(s_sdlAudioStream);

	ms_initialized = true;
}

void Audio::Update()
{
	if (!ms_initialized || STL::IsEmpty(s_autoRelease)) {
		return;
	}
	STL::Vector<int> finished;
	for (int id : s_autoRelease) {
		cm_Source* source = GetSource(id);
		if (source != nullptr && cm_get_state(source) == CM_STATE_STOPPED) {
			STL::Add(finished, id);
		}
	}
	for (int id : finished) {
		Destroy({id});
	}
}

void Audio::Destroy(EventInstance instance)
{
	cm_Source* source = GetSource(instance.id);
	if (source == nullptr) {
		return;
	}
	STL::RemoveKey(s_sources, instance.id);
	STL::Remove(s_autoRelease, instance.id);
	cm_destroy_source(source);
}

void Audio::Terminate()
{
	for (auto source : s_sources) {
		cm_destroy_source(source.second);
	}
	STL::Clear(s_sources);
	STL::Clear(s_autoRelease);
	s_sourceCount = 0;

	if (ms_initialized) {
		SDL_DestroyAudioStream(s_sdlAudioStream);
		s_sdlAudioStream = nullptr;

		SDL_DestroyMutex(audio_mutex);
		audio_mutex = nullptr;

		SDL_QuitSubSystem(SDL_INIT_AUDIO);

		ms_initialized = false;
	}

	// Buffers must outlive the sources created from them, free them last
	for (auto& description : s_descriptions) {
		delete[] description.second.data;
		description.second.data = nullptr;
	}
	STL::Clear(s_descriptions);
}

void Audio::MasterVolume(double volume)
{
	ms_masterVolume = volume;
	cm_set_master_gain(volume);
}

void Audio::LoadFile(const StringID& path, const String& filePath)
{
	s_descriptions[path] = Audio::EventDescription{.filename = filePath};
}

void Audio::LoadFileFromMemory(const StringID& path, void* data, int size)
{
	// cmixer keeps a pointer into the buffer without copying it,
	// so Audio has to own a copy to keep it valid for every instance
	auto* owned = new type::UInt8[size];
	std::memcpy(owned, data, size);
	auto& description = s_descriptions[path];
	delete[] description.data;
	description.filename = "";
	description.data = owned;
	description.size = size;
}

Audio::EventInstance Audio::Play(const StringID& path, double volume /* = 1.0*/, double pitch /* = 1.0*/,
								 double pan /* = 0.0*/)
{
	Audio::EventInstance instance = CreateInstance(path, volume, pitch, pan);
	if (instance.IsValid()) {
		STL::Add(s_autoRelease, instance.id);
		cm_play(GetSource(instance.id));
	}
	return instance;
}

Audio::EventInstance Audio::Loop(const StringID& path, double volume /* = 1.0*/, double pitch /* = 1.0*/,
								 double pan /* = 0.0*/)
{
	Audio::EventInstance instance = CreateLoopInstance(path, volume, pitch, pan);
	if (instance.IsValid()) {
		cm_play(GetSource(instance.id));
	}
	return instance;
}

Audio::EventInstance Audio::CreateInstance(const StringID& path, double volume /* = 1.0*/, double pitch /* = 1.0*/,
										   double pan /* = 0.0*/)
{
	Audio::EventInstance instance = s_descriptions[path].CreateInstance();
	cm_Source* src = GetSource(instance.id);
	if (src == nullptr) {
		return {-1};
	}
	cm_set_gain(src, volume);
	cm_set_pitch(src, pitch);
	cm_set_pan(src, pan);
	return instance;
}

Audio::EventInstance Audio::CreateLoopInstance(const StringID& path, double volume /* = 1.0*/, double pitch /* = 1.0*/,
											   double pan /* = 0.0*/)
{
	Audio::EventInstance instance = s_descriptions[path].CreateInstance();
	cm_Source* src = GetSource(instance.id);
	if (src == nullptr) {
		return {-1};
	}
	cm_set_gain(src, volume);
	cm_set_pitch(src, pitch);
	cm_set_pan(src, pan);
	cm_set_loop(src, 1);
	return instance;
}

double Audio::GetLength(Audio::EventInstance instance)
{
	cm_Source* src = GetSource(instance.id);
	return src != nullptr ? cm_get_length(src) : 0.0;
}

double Audio::GetPosition(Audio::EventInstance instance)
{
	cm_Source* src = GetSource(instance.id);
	return src != nullptr ? cm_get_position(src) : 0.0;
}

void Audio::SetParam(Audio::EventInstance instance, double volume, double pitch, double pan)
{
	cm_Source* src = GetSource(instance.id);
	if (src == nullptr) return;
	cm_set_gain(src, volume);
	cm_set_pitch(src, pitch);
	cm_set_pan(src, pan);
}

void Audio::SetVolume(Audio::EventInstance instance, double volume)
{
	cm_Source* src = GetSource(instance.id);
	if (src == nullptr) return;
	cm_set_gain(src, volume);
}

void Audio::SetPitch(Audio::EventInstance instance, double pitch)
{
	cm_Source* src = GetSource(instance.id);
	if (src == nullptr) return;
	cm_set_pitch(src, pitch);
}

void Audio::SetPan(Audio::EventInstance instance, double pan)
{
	cm_Source* src = GetSource(instance.id);
	if (src == nullptr) return;
	cm_set_pan(src, pan);
}

void Audio::SetLoop(Audio::EventInstance instance, bool loop)
{
	cm_Source* src = GetSource(instance.id);
	if (src == nullptr) return;
	cm_set_loop(src, loop ? 1 : 0);
}

bool Audio::IsPlaying(Audio::EventInstance instance)
{
	cm_Source* src = GetSource(instance.id);
	return src != nullptr && cm_get_state(src) == CM_STATE_PLAYING;
}

bool Audio::IsPaused(Audio::EventInstance instance)
{
	cm_Source* src = GetSource(instance.id);
	return src != nullptr && cm_get_state(src) == CM_STATE_PAUSED;
}

bool Audio::IsStopped(Audio::EventInstance instance)
{
	cm_Source* src = GetSource(instance.id);
	return src != nullptr && cm_get_state(src) == CM_STATE_STOPPED;
}

void Audio::Pause(Audio::EventInstance instance)
{
	cm_Source* src = GetSource(instance.id);
	if (src == nullptr) return;
	cm_pause(src);
}

void Audio::Resume(Audio::EventInstance instance)
{
	cm_Source* src = GetSource(instance.id);
	if (src == nullptr) return;
	cm_play(src);
}

void Audio::Stop(Audio::EventInstance instance)
{
	cm_Source* src = GetSource(instance.id);
	if (src == nullptr) return;
	cm_stop(src);
}

} // namespace cherrysoda
