#ifndef _CHERRYSODA_COMPONENTS_GRAPHICS_SPRITE_H_
#define _CHERRYSODA_COMPONENTS_GRAPHICS_SPRITE_H_

#include <CherrySoda/Components/Graphics/Image.h>
#include <CherrySoda/Graphics/Atlas.h>
#include <CherrySoda/Graphics/MTexture.h>
#include <CherrySoda/Util/Chooser.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/Pool.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

namespace cherrysoda {

class Sprite : public Image
{
public:
	CHERRYSODA_DECLARE_COMPONENT(Sprite, Image);

	Sprite(const String& atlasPath, const String& path = "") : Sprite()
	{
		m_ownAtlas = true;
		m_atlas = Atlas::FromAtlas(atlasPath);
		m_path = path;
	}

	Sprite(Atlas* atlas, const String& path = "") : Sprite()
	{
		m_atlas = atlas;
		m_path = path;
	}

	virtual ~Sprite()
	{
		if (m_ownAtlas && m_atlas != nullptr) {
			delete m_atlas;
			m_atlas = nullptr;
		}
	}

	CHERRYSODA_GETTER_SETTER_EX_OF_VEC2(Justify, m_justify, CHERRYSODA_NONE_OP, m_justifyHasValue = true);
	void ClearJustify() { m_justifyHasValue = false; }

	void operator = (const Sprite& sprite) = delete;

	void Update() override;
	void SetFrame(const MTexture& texture);

	inline STL::Vector<MTexture> GetFrames(const String& path)
	{
		auto ret = m_atlas->GetAtlasSubtextures(m_path + path);
		CHERRYSODA_ASSERT_FORMAT(STL::IsNotEmpty(ret), "No frames found for animation path '%s'!\n", (m_path + path).c_str());
		m_width = Math_Max(m_width, ret[0].Width());
		m_height = Math_Max(m_height, ret[0].Height());
		return ret;
	}

	inline STL::Vector<MTexture> GetFrames(const String& path, const STL::Vector<int>& frames)
	{
		STL::Vector<MTexture> ret;
		auto fullPath = m_path + path;
		for (int frameIndex : frames) {
			auto frame = m_atlas->GetAtlasSubtextureAt(fullPath, frameIndex);
			CHERRYSODA_ASSERT_FORMAT(frame.IsValid(), "Can't find sprite %s with index %d\n", fullPath.c_str(), frameIndex);
			STL::Add(ret, frame);	
		}	
		CHERRYSODA_ASSERT_FORMAT(STL::IsNotEmpty(ret), "No frames found for animation path '%s'!\n", (m_path + path).c_str());
		m_width = Math_Max(m_width, ret[0].Width());
		m_height = Math_Max(m_height, ret[0].Height());
		return ret;	
	}

	inline void Add(const StringID& id, const String& path, float delay = 1.f / 15.f, Chooser<StringID> into = Chooser<StringID>())
	{
		m_animations[id] = { delay, GetFrames(path), into };
	}

	void Add(const StringID& id, const String& path, float delay, const STL::Vector<int>& frames)
	{
		m_animations[id] = { delay, GetFrames(path, frames), Chooser<StringID>() };
	}

	inline void Add(const StringID& id, const String& path, float delay, Chooser<StringID> into, const STL::Vector<int>& frames)
	{
		m_animations[id] = { delay, GetFrames(path, frames), into };
	}

	inline void AddLoop(const StringID& id, const String& path, float delay = 1.f / 15.f)
	{
		m_animations[id] = { delay, GetFrames(path), Chooser<StringID>(id, 1.f) };
	}

	inline void AddLoop(const StringID& id, const String& path, float delay, const STL::Vector<int>& frames)
	{
		m_animations[id] = { delay, GetFrames(path, frames), Chooser<StringID>(id, 1.f) };
	}

	void Play(const StringID& id, bool restart = false, bool randomizeFrame = false);

	inline bool Has(const StringID& id) const
	{
		return STL::ContainsKey(m_animations, id);
	}

	inline bool IsPlaying(const StringID& id) const
	{
		CHERRYSODA_ASSERT_FORMAT(Has(id), "No Animation defined for ID: %s\n", id.GetStr().c_str());
		return id == m_currentAnimationID;
	}

	inline void Stop()
	{
		m_animating = false;
		m_currentAnimation = nullptr;
		m_currentAnimationID = "";
	}

	inline float Rate() const { return m_rate; }

	inline void OnFinish(STL::Action<StringID> onFinish) { m_onFinish = onFinish; }
	inline void OnLoop(STL::Action<StringID> onLoop) { m_onLoop = onLoop; }
	inline void OnFrameChange(STL::Action<StringID> onFrameChange) { m_onFrameChange = onFrameChange; }
	inline void OnLastFrame(STL::Action<StringID> onLastFrame) { m_onLastFrame = onLastFrame; }
	inline void OnChange(STL::Action<StringID,StringID> onChange) { m_onChange = onChange; }

	float Width() const override { return m_width; }
	float Height() const override { return m_height; }

	inline Sprite* CreateClone()
	{
		auto sprite = new Sprite();
		sprite->AutoDeleteWhenRemoved();
		return CloneInto(sprite);
	}

	Sprite* CloneInto(Sprite* sprite);

private:
	CHERRYSODA_FRIEND_CLASS_POOL;

	Sprite() : base(MTexture(), true) {}

	struct Animation
	{
		float m_delay = 0.f;
		STL::Vector<MTexture> m_frames;
		Chooser<StringID> m_goto;
	};

	float m_rate = 1.f;
	Math::Vec2 m_justify = Vec2_Zero;
	bool m_justifyHasValue = false;
	bool m_useRawDeltaTime = false;
	bool m_animating = false;
	bool m_ownAtlas = false;
	STL::Action<StringID> m_onFinish;
	STL::Action<StringID> m_onLoop;
	STL::Action<StringID> m_onFrameChange;
	STL::Action<StringID> m_onLastFrame;
	STL::Action<StringID,StringID> m_onChange;

	Atlas* m_atlas = nullptr;
	String m_path;
	STL::HashMap<StringID,Animation> m_animations;
	Animation* m_currentAnimation = nullptr;
	float m_animationTimer = 0.f;
	int m_width = 0;
	int m_height = 0;

	StringID m_currentAnimationID;
	StringID m_lastAnimationID;
	int m_currentAnimationFrame = 0;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_GRAPHICS_SPRITE_H_
