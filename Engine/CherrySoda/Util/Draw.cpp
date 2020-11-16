#include <CherrySoda/Util/Draw.h>

#include <CherrySoda/Graphics/SpriteBatch.h>

using cherrysoda::Draw;

cherrysoda::Renderer* Draw::ms_renderer = nullptr;
cherrysoda::SpriteBatch* Draw::ms_spriteBatch = nullptr;

void Draw::Initialize()
{
	ms_spriteBatch = new SpriteBatch();
}

void Draw::Terminate()
{
	delete ms_spriteBatch;
}
