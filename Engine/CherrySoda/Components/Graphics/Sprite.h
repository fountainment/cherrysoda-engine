#ifndef _CHERRYSODA_COMPONENTS_GRAPHICS_SPRITE_H_
#define _CHERRYSODA_COMPONENTS_GRAPHICS_SPRITE_H_

#include <CherrySoda/Components/Graphics/Image.h>
#include <CherrySoda/Graphics/Atlas.h>
#include <CherrySoda/Graphics/MTexture.h>
#include <CherrySoda/Util/Chooser.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

namespace cherrysoda {

class Sprite : public Image
{
public:
	typedef Image base;

	Sprite() : base(MTexture(), true) {}

	Sprite(const String& atlasPath, const String& path = "") : Sprite()
	{
		m_atlas = Atlas::FromAtlas(atlasPath);
		m_path = path;
	}

	void Update() override;
	void SetFrame(MTexture texture);


	STL::Vector<MTexture> GetFrames(const String& path)
	{
		return STL::ToVector(m_atlas.GetAtlasSubtextures(m_path + path, 1));
	}

	void AddLoop(const String& id, const String& path, float delay = 0.f)
	{
		m_animations[id] = { delay, GetFrames(path), Chooser<StringID>(id, 1.f) };
	}

	// TODO: void Add(const String& id, const String& path)

	void Play(StringID id, bool restart = false, bool randomizeFrame = false);

	bool Has(StringID id)
	{
		return STL::ContainsKey(m_animations, id);
	}

	void Stop()
	{
		m_animating = false;
		m_currentAnimation = nullptr;
		m_currentAnimationID = "";
	}

	inline float Rate() const { return m_rate; }

private:
	struct Animation
	{
		float m_delay;
		STL::Vector<MTexture> m_frames;
		Chooser<StringID> m_goto;
	};

	float m_rate = 1.f;
	bool m_useRawDeltaTime = false;
	STL::Action<StringID> m_onFinish;
	STL::Action<StringID> m_onLoop;
	STL::Action<StringID> m_onFrameChange;
	STL::Action<StringID> m_onLastFrame;
	STL::Action<StringID,StringID> m_onChange;

	Atlas m_atlas;
	String m_path;
	STL::HashMap<StringID,Animation> m_animations;
	Animation* m_currentAnimation = nullptr;
	float m_animationTimer = 0.f;
	int m_width = 0;
	int m_height = 0;

	bool m_animating = false;
	StringID m_currentAnimationID;
	StringID m_lastAnimationID;
	int m_currentAnimationFrame = 0;
	int m_currentAnimationTotalFrame = 0;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_GRAPHICS_SPRITE_H_
