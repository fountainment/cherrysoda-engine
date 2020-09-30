#ifndef _SCENES_MAINSCENE_H_
#define _SCENES_MAINSCENE_H_

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Scene.h>
#include <CherrySoda/Util/BitTag.h>

class Chunk;
class World;
namespace cherrysoda {
class Entity;
class SingleTagRenderer;
} // namespace cherrysoda

class MainScene : public cherrysoda::Scene
{
public:
	typedef cherrysoda::Scene base;

	void Begin() override;
	void BeforeRender() override;

	void Update() override;

	static const cherrysoda::BitTag ms_skyboxTag;
	static const cherrysoda::BitTag ms_voxelTag;

private:
	cherrysoda::SingleTagRenderer* m_voxelRenderer = nullptr;
	cherrysoda::SingleTagRenderer* m_skyboxRenderer = nullptr;
	World* m_voxelWorld = nullptr;
	cherrysoda::Entity* m_player = nullptr;
	cherrysoda::Entity* m_skybox = nullptr;
	cherrysoda::Graphics::UniformHandle m_uniformMtx = cherrysoda::Graphics::InvalidHandle;
};

#endif // _SCENES_MAINSCENE_H_
