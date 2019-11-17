#include "MainScene.h"

#include <CherrySoda/Entity.h>
#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Renderers/Renderer.h>
#include <CherrySoda/Renderers/EverythingRenderer.h>

#include "Voxel/Chunk.h"

using cherrysoda::Component;
using cherrysoda::Entity;
using cherrysoda::EverythingRenderer;
using cherrysoda::Renderer;

void MainScene::Begin()
{
	Renderer* renderer = new EverythingRenderer;
	Entity* entity = new Chunk;
	Component* component = new Component(true, true);

	entity->Add(component);

	Add(renderer);
	Add(entity);
}