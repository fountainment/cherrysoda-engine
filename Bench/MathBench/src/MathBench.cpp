#include "MathBench.h"

#include <CherrySoda/CherrySoda.h>

using mathbench::MathBench;

using namespace cherrysoda;

MathBench::MathBench()
	: base()
{
	SetTitle("MathBench");
	SetClearColor(Color::Black);
}

void MathBench::Update()
{
	base::Update();
}

void MathBench::Initialize()
{
	base::Initialize();

	auto a = Time::GetSystemTime();

	Math::Vec4 start(1.f);
	for (int i = 0; i < 100000000; ++i) {
		float rotation = Calc::GetRandom()->NextAngle();
		const Math::Mat4 rotationMat = Math_Rotate(Math_Identity<Math::Mat4>(), rotation, Vec3_ZUp);
		start = rotationMat * start;
	}

	auto b = Time::GetSystemTime();

	CHERRYSODA_LOG_FORMAT("%lf\n", b - a);

	Exit();
}

void MathBench::LoadContent()
{
	base::LoadContent();

	// Load textures, sprites, shaders and other resources here
}

