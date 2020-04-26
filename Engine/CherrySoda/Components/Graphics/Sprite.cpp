#include <CherrySoda/Components/Graphics/Sprite.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Util/Math.h>

using cherrysoda::Sprite;

using cherrysoda::Engine;

void Sprite::Update()
{
	if (m_animating) {
		// Timer
		if (m_useRawDeltaTime) {
			m_animationTimer += Engine::Instance()->RawDeltaTime() * Rate();
		}
		else {
			m_animationTimer += Engine::Instance()->DeltaTime() * Rate();
		}

		// Next Frame
		if (Math_Abs(m_animationTimer) > m_currentAnimation->m_delay) {
			m_currentAnimationFrame += Math_Sign(m_animationTimer);
			m_animationTimer -= Math_Sign(m_animationTimer) * m_currentAnimation->m_delay;

			// End of Animation
			if (m_currentAnimationFrame < 0 || m_currentAnimationFrame >= STL::Count(m_currentAnimation->m_frames)) {
				auto was = m_currentAnimationID;
				if (m_onLastFrame) {
					m_onLastFrame(m_currentAnimationID);
				}

				// only do stuff if OnLastFrame didn't just change the animation
				if (was == m_currentAnimationID) {
					// Looped
					if (!m_currentAnimation->m_goto.IsEmpty()) {
						m_currentAnimationID = m_currentAnimation->m_goto.Choose();
						if (m_onChange) {
							m_onChange(m_lastAnimationID, m_currentAnimationID);	
						}
						m_lastAnimationID = m_currentAnimationID;
						m_currentAnimation = &m_animations[m_lastAnimationID];
						if (m_currentAnimationFrame < 0) {
							m_currentAnimationFrame = STL::Count(m_currentAnimation->m_frames) - 1;
						}
						else {
							m_currentAnimationFrame = 0;
						}
						SetFrame(m_currentAnimation->m_frames[m_currentAnimationFrame]);
						if (m_onLoop) {
							m_onLoop(m_currentAnimationID);
						}
					}
					else {
						// Ended
						if (m_currentAnimationFrame < 0) {
							m_currentAnimationFrame = 0;
						}
						else {
							m_currentAnimationFrame = STL::Count(m_currentAnimation->m_frames) - 1;
						}
						m_animating = false;
						auto id = m_currentAnimationID;
						m_currentAnimationID = "";
						m_currentAnimation = nullptr;
						m_animationTimer = 0.f;
						if (m_onFinish) {
							m_onFinish(id);
						}

					}
				}
			}
			else {
				// Continue Animation
				SetFrame(m_currentAnimation->m_frames[m_currentAnimationFrame]);
			}
		}
	}	
}

void Sprite::SetFrame(MTexture texture)
{
	Texture(texture);
	Origin(Math::Vec2(Texture().Size()) * m_justify);
	if (m_onFrameChange) {
		m_onFrameChange(m_currentAnimationID);
	}
}

void Sprite::Play(StringID id, bool restart/* = false*/, bool randomizeFrame/* = false*/)
{
	if (m_currentAnimationID != id || restart) {
		CHERRYSODA_ASSERT_FORMAT(STL::ContainsKey(m_animations, id), "No Animation defined for ID: %s\n", id.GetStr().c_str()); 

		if (m_onChange) {
			m_onChange(m_lastAnimationID, id);
		}
		m_lastAnimationID = m_currentAnimationID = id;
		m_currentAnimation = &m_animations[id];
		m_animating = m_currentAnimation->m_delay > 0;
		if (randomizeFrame) {
			// TODO: Randomize Frame
		}
		else {
			m_animationTimer = 0.f;
			m_currentAnimationFrame = 0;
		}
		SetFrame(m_currentAnimation->m_frames[m_currentAnimationFrame]);
	}
}