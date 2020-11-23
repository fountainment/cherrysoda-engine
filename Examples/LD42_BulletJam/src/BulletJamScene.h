#ifndef _BULLETJAMSCENE_H_
#define _BULLETJAMSCENE_H_

#include <CherrySoda/Scene.h>
#include <CherrySoda/Util/BitTag.h>

namespace cherrysoda {
class Entity;
class EverythingRenderer;
class SingleTagRenderer;
class Tween;
} // namespace cherrysoda

namespace ld42_bulletjam {

class BulletJamScene : public cherrysoda::Scene
{
public:
	typedef cherrysoda::Scene base;

	void Begin() override;
	void Update() override;

	void Start();
	void Restart();

private:
	cherrysoda::Tween* m_openUITween;
	bool m_inGameProgress = false;
	cherrysoda::EverythingRenderer* m_renderer;
};

} // namespace ld42_bulletjam

#endif // _BULLETJAMSCENE_H_
