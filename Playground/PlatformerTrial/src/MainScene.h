#ifndef _MAINSCENE_H_
#define _MAINSCENE_H_

#include <CherrySoda/Graphics/Texture.h>
#include <CherrySoda/Scene.h>
#include <CherrySoda/Util/BitTag.h>

namespace cherrysoda {
class Entity;
class SingleTagRenderer;
class TagExcludeRenderer;
class RenderTarget2D;
} // namespace cherrysoda

namespace main {

class MainScene : public cherrysoda::Scene
{
public:
	typedef cherrysoda::Scene base;

	void Begin() override;
	void Update() override;

	~MainScene();

	cherrysoda::RenderTarget2D* m_mainScreenTarget = nullptr;
	cherrysoda::Texture2D m_backgroundTexture;
	cherrysoda::SingleTagRenderer* m_backgroundRenderer = nullptr;
	cherrysoda::TagExcludeRenderer* m_mainRenderer = nullptr;
	cherrysoda::SingleTagRenderer* m_screenTexRenderer = nullptr;
};

} // namespace main

#endif // _MAINSCENE_H_
