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

	static EventInstance Play(const StringID& path);
	static EventInstance Play(const StringID& path, double volume, double pitch, double pan);
	static EventInstance Loop(const StringID& path);
	static EventInstance Loop(const StringID& path, double volume, double pitch, double pan);

	static double GetLength(EventInstance instance);
	static double GetPosition(EventInstance instance); 
	static void SetParam(EventInstance instance, double volume, double pitch, double pan);
	static bool IsPlaying(EventInstance instance);
	static void Pause(EventInstance instance);
	static void Resume(EventInstance instance);
	static void Stop(EventInstance instance);

private:
	static double ms_masterVolume;
	static bool ms_initialized;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_AUDIO_AUDIO_H_
