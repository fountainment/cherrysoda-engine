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
			if (m_currentAnimationFrame < 0 || m_currentAnimationFrame >= m_currentAnimationTotalFrame) {
				auto was = m_currentAnimationID;
				if (m_onLastFrame) {
					m_onLastFrame(m_currentAnimationID);
				}

				// only do stuff if OnLastFrame didn't just change the animation
				if (was == m_currentAnimationID) {
					// Looped
					// if (m_currentAnimation->m_goto.IsValid()) {
					// }
					// else {
					// }
				}
				else {
					// Continue Animation
					SetFrame(m_currentAnimation->m_frames[m_currentAnimationFrame]);
				}
			}
		}
	}	
}

void Sprite::SetFrame(MTexture texture)
{
	if (texture == Texture()) {
		return;
	}
	Texture(texture);
	if (m_onFrameChange) {
		m_onFrameChange(m_currentAnimationID);
	}
}
