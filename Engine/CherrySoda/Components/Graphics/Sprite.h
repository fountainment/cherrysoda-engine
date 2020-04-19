#ifndef _CHERRYSODA_COMPONENTS_GRAPHICS_SPRITE_H_
#define _CHERRYSODA_COMPONENTS_GRAPHICS_SPRITE_H_

#include <CherrySoda/Components/Graphics/Image.h>
#include <CherrySoda/Graphics/Atlas.h>
#include <CherrySoda/Graphics/MTexture.h>
#include <CherrySoda/Util/Chooser.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

namespace cherrysoda {

class Sprite : Image
{
public:
	void Update() override {}

private:
	STL::Action<StringID> m_onFinish;
	STL::Action<StringID> m_onLoop;
	STL::Action<StringID> m_onFrameChange;
	STL::Action<StringID> m_onLastFrame;
	STL::Action<StringID,StringID> m_onChage;

	Atlas m_atlas;
	String m_path;

	// TODO: void Update() override;
	// TODO: void AddLoop(const String& id, const String& path, float delay)
	// TODO: void Add(const String& id, const String& path)

	class Animation
	{
	public:
		float m_delay;
		STL::Vector<MTexture> m_frames;
		Chooser<StringID> m_goto;

	};
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_GRAPHICS_SPRITE_H_
