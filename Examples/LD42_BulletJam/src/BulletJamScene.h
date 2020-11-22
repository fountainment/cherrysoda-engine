#ifndef _BULLETJAMSCENE_H_
#define _BULLETJAMSCENE_H_

#include <CherrySoda/Scene.h>
#include <CherrySoda/Util/BitTag.h>

namespace cherrysoda {
class Entity;
class EverythingRenderer;
class SingleTagRenderer;
} // namespace cherrysoda

namespace ld42_bulletjam {

class BulletJamScene : public cherrysoda::Scene
{
public:
	typedef cherrysoda::Scene base;

	void Begin() override;
	void Update() override;
};

} // namespace ld42_bulletjam

#endif // _BULLETJAMSCENE_H_
