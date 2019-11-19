#include "MainScene.h"

#include <CherrySoda/Entity.h>
#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Renderers/Renderer.h>
#include <CherrySoda/Renderers/EverythingRenderer.h>
#include <CherrySoda/Util/Math.h>

#include "Voxel/Chunk.h"

using cherrysoda::Component;
using cherrysoda::Entity;
using cherrysoda::EverythingRenderer;
using cherrysoda::Math;
using cherrysoda::Renderer;

void MainScene::Begin()
{
	auto renderer = new EverythingRenderer;
	auto entity = new Chunk;

	renderer->GetCamera()->Position(Math::Vec3(0.f, 0.f, 3.f));

	Add(renderer);
	Add(entity);
}