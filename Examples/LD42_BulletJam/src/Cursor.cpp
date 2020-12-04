#include "Cursor.h"

#include "Program.h"

#include <CherrySoda/CherrySoda.h>

using namespace cherrysoda;
using namespace ld42_bulletjam;

Cursor* Cursor::Instance()
{
	static Cursor* s_instance = nullptr;
	if (s_instance == nullptr) {
		s_instance = Create();
	}
	return s_instance;
}

Cursor* Cursor::Create()
{
	Cursor* cursor = new Cursor();
	Image* image = new Image(GameApp::GetAtlas()->GetAtlasSubtextureFromAtlasAt("mouse"));
	image->CenterOrigin();
	cursor->Add(image);
	cursor->Depth(-100000);
	return cursor;
}

void Cursor::Update()
{
	base::Update();

	auto camera = GetScene()->FirstRenderer()->GetCamera();
	float height = camera->Position().z * 2.f;
	auto pos = MInput::Mouse()->RawPosition();
	float scale = Engine::Instance()->GetWindowSize().y / height;
	pos.x /= scale;
	pos.y /= scale;
	pos.x += camera->Position().x - height * 0.5f * camera->Ratio();
	pos.y -= camera->Position().y - height * 0.5f;
	pos.y = height - pos.y;
	Position(pos);
}
