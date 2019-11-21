#ifndef _SCENES_MAINSCENE_H_
#define _SCENES_MAINSCENE_H_

#include <CherrySoda/Scene.h>

class Chunk;
namespace cherrysoda {
class EverythingRenderer;
} // namespace cherrysoda

class MainScene : public cherrysoda::Scene
{
public:
	typedef cherrysoda::Scene base;

	void Begin() override;
	void BeforeRender() override;

private:
	cherrysoda::EverythingRenderer* m_renderer = nullptr;
	Chunk* m_chunk = nullptr;
};

#endif // _SCENES_MAINSCENE_H_