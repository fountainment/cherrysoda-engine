#ifndef _CHERRYSODASCENETEMPLATESCENE_H_
#define _CHERRYSODASCENETEMPLATESCENE_H_

#include <CherrySoda/Scene.h>
#include <CherrySoda/Util/BitTag.h>

namespace cherrysoda {
class Entity;
class EverythingRenderer;
class SingleTagRenderer;
} // namespace cherrysoda

namespace cherrysodascenetemplate {

class CherrySodaSceneTemplateScene : public cherrysoda::Scene
{
public:
	typedef cherrysoda::Scene base;

	void Begin() override;
	void Update() override;
};

} // namespace cherrysodascenetemplate

#endif // _CHERRYSODASCENETEMPLATESCENE_H_
