#ifndef _CHERRYSODA_COMPONENTS_GRAPHICS_SPRITESHEET_H_
#define _CHERRYSODA_COMPONENTS_GRAPHICS_SPRITESHEET_H_

#include <CherrySoda/Components/Graphics/Image.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

template <class T>
class SpriteSheet : public Image
{
public:
	CHERRYSODA_DECLARE_COMPONENT(SpriteSheet<T>, Image);

	CHERRYSODA_GETTER_SETTER_OF_TYPE(float, CurrentFrame, m_currentFrame);

	SpriteSheet(const MTexture& texture, int frameWidth, int frameHeight, int frameSep = 0)
		: base(texture, true)
	{
		SetFrames(texture, frameWidth, frameHeight, frameSep);
	}

	void SetFrames(const MTexture& texture, int frameWidth, int frameHeight, int frameSep = 0)
	{
		STL::Clear(m_frames);
		int x = 0, y = 0;
		while (y <= texture.Height() - frameHeight) {
			while (x <= texture.Width() - frameWidth) {
				STL::Add(m_frames, texture.GetSubtexture(x, y, frameWidth, frameHeight));
				x += frameWidth + frameSep;
			}

			y += frameHeight + frameSep;
			x = 0;
		}
	}

	void Update() override
	{
		if (m_animating && m_currentAnimation.m_delay > 0.f) {
			// Timer
			if (m_useRawDeltaTime)
				m_animationTimer += Engine::Instance()->RawDeltaTime() * m_rate;
			else
				m_animationTimer += Engine::Instance()->DeltaTime() * m_rate;

			// Next Frame
			if (Math_Abs(m_animationTimer) >= m_currentAnimation.m_delay) {
				m_currentAnimationFrame += Math_Sign(m_animationTimer);
				m_animationTimer -= Math_Sign(m_animationTimer) * m_currentAnimation.m_delay;	

				// End of Animation
				if (m_currentAnimationFrame < 0 || m_currentAnimationFrame >= static_cast<int>(STL::Count(m_currentAnimation.m_frames))) {
					// Looped
					if (m_currentAnimation.m_loop) {
						m_currentAnimationFrame -= Math_Sign(m_currentAnimationFrame) * STL::Count(m_currentAnimation.m_frames);
						m_currentFrame = m_currentAnimation.m_frames[m_currentAnimationFrame];

						if (m_onAnimate != nullptr)
							m_onAnimate(m_currentAnimationID);
						if (m_onLoop != nullptr)
							m_onLoop(m_currentAnimationID);
					}
					else {
						// Ended
						if (m_currentAnimationFrame < 0)
							m_currentAnimationFrame = 0;
						else
							m_currentAnimationFrame = STL::Count(m_currentAnimation.m_frames) - 1;

						m_animating = false;
						m_animationTimer = 0.f;
						if (m_onFinish != nullptr)
							m_onFinish(m_currentAnimationID);
					}
				}
				else {
					// Continue Animation
					m_currentFrame = m_currentAnimation.m_frames[m_currentAnimationFrame];
					if (m_onAnimate != nullptr) {
						m_onAnimate(m_currentAnimationID);
					}
				}
			}
		}
	}

	void Render() override
	{
		Texture(m_frames[m_currentFrame]);
		base::Render();
	}

	void Add(const T& id, bool loop, float delay, const STL::Vector<int>& frames)
	{
		m_animations[id] = Animation{ delay, frames, loop };
	}

	void Add(const T& id, float delay, const STL::Vector<int>& frames)
	{
		Add(id, true, delay, frames);
	}

	void Add(const T& id, int frame)
	{
		Add(id, false, 0, { frame });
	}

	void ClearAnimation()
	{
		STL::Clear(m_animations);
	}

	bool IsPlaying(const T& id)
	{
		if (!m_played)
			return false;
		else
			return m_currentAnimationID == id;
	}

	void Play(const T& id, bool restart = false)
	{
		if (!IsPlaying(id) || restart) {
			CHERRYSODA_ASSERT_FORMAT(STL::ContainsKey(m_animations, id), "No Animation Defined For ID: %s\n", StringUtil::ToString(id).c_str());
			m_currentAnimationID = id;
			m_currentAnimation = m_animations[id];
			m_animationTimer = 0.f;
			m_currentAnimationFrame = 0;
			m_played = true;

			m_animating = STL::Count(m_currentAnimation.m_frames) > 1;
			m_currentFrame = m_currentAnimation.m_frames[0];
		}
	}

	void Reverse(const T& id, bool restart = false)
	{
		Play(id, restart);
		if (m_rate > 0.f) {
			m_rate *= -1.f;
		}
	}

	void Stop()
	{
		m_animating = false;
		m_played = false;
	}

	float Width() const override
	{
		if (STL::IsNotEmpty(m_frames))
			return m_frames[0].Width();
		else
			return 0.f;
	}

	float Height() const override
	{
		if (STL::IsNotEmpty(m_frames))
			return m_frames[0].Height();
		else
			return 0.f;
	}

private:
	struct Animation
	{
		float m_delay = 0.f;
		STL::Vector<int> m_frames;
		bool m_loop = false;
	};

	int m_currentFrame = -1;
	float m_rate = 1.f;
	bool m_useRawDeltaTime = false;
	STL::Action<T> m_onFinish = nullptr;
	STL::Action<T> m_onLoop = nullptr;
	STL::Action<T> m_onAnimate = nullptr;

	STL::HashMap<T, Animation> m_animations;
	Animation m_currentAnimation;
	float m_animationTimer = 0.f;
	bool m_played = false;

	STL::Vector<MTexture> m_frames;
	bool m_animating = false;
	T m_currentAnimationID = T{};
	int m_currentAnimationFrame = -1;
};

} // namespace cherrysoda
#endif // _CHERRYSODA_COMPONENTS_GRAPHICS_SPRITESHEET_H_
