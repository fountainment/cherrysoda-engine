#ifndef _CHERRYSODA_AUDIO_AUDIO_H_
#define _CHERRYSODA_AUDIO_AUDIO_H_

#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

namespace cherrysoda {

class Audio
{
public:
	struct EventInstance
	{
		int id = -1;

		inline bool IsValid() const { return id != -1; }

		inline double GetLength() { return Audio::GetLength(*this); }
		inline double GetPosition() { return Audio::GetPosition(*this); }
		inline void SetVolume(double volume) { Audio::SetVolume(*this, volume); }
		inline void SetPitch(double pitch) { Audio::SetPitch(*this, pitch); }
		inline void SetPan(double pan) { Audio::SetPan(*this, pan); }
		inline void SetLoop(bool loop) { Audio::SetLoop(*this, loop); }
		inline bool IsPlaying() { return Audio::IsPlaying(*this); }
		inline bool IPaused() { return Audio::IsPaused(*this); }
		inline bool IsStopped() { return Audio::IsStopped(*this); }
		inline void Play() { Audio::Resume(*this); }
		inline void Pause() { Audio::Pause(*this); }
		inline void Stop() { Audio::Stop(*this); }
		inline void Replay() { Audio::Stop(*this); Audio::Resume(*this); }
	};

	struct EventDescription
	{
		String filename = "";
		type::UInt8* data = nullptr;
		int size = 0;

		EventInstance CreateInstance();
	};

	static void Initialize();
	static void Terminate();

	static void LoadFile(const StringID& path, const String& filePath);
	static void LoadFileFromMemory(const StringID& path, void* data, int size);

	static void MasterVolume(double volume);
	static inline double MasterVolume() { return ms_masterVolume; }

	static EventInstance Play(const StringID& path, double volume = 1.0, double pitch = 1.0, double pan = 0.0);
	static EventInstance Loop(const StringID& path, double volume = 1.0, double pitch = 1.0, double pan = 0.0);

	static EventInstance CreateInstance(const StringID& path, double volume = 1.0, double pitch = 1.0, double pan = 0.0);
	static EventInstance CreateLoopInstance(const StringID& path, double volume = 1.0, double pitch = 1.0, double pan = 0.0);

	static double GetLength(EventInstance instance);
	static double GetPosition(EventInstance instance);
	static void SetParam(EventInstance instance, double volume, double pitch, double pan);
	static void SetVolume(EventInstance instance, double volume);
	static void SetPitch(EventInstance instance, double pitch);
	static void SetPan(EventInstance instance, double pan);
	static void SetLoop(EventInstance instance, bool loop);
	static bool IsPlaying(EventInstance instance);
	static bool IsPaused(EventInstance instance);
	static bool IsStopped(EventInstance instance);
	static void Pause(EventInstance instance);
	static void Resume(EventInstance instance);
	static void Stop(EventInstance instance);

private:
	static double ms_masterVolume;
	static bool ms_initialized;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_AUDIO_AUDIO_H_
