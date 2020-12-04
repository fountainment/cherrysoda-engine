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

	Math::Vec4 v(1.f);
	int times = Calc::GetRandom()->NextInt(10000000, 100000000);
	for (int i = 0; i < times; ++i) {
		float rotation = Calc::GetRandom()->NextAngle();
		const Math::Mat4 rotationMat = Math_Rotate(Math_Identity<Math::Mat4>(), rotation, Vec3_ZUp);
		v = rotationMat * v;
	}

	auto b = Time::GetSystemTime();

	CHERRYSODA_LOG_FORMAT("Result: Vec4(%f, %f, %f, %f)\n", v.x, v.y, v.z, v.w);
	CHERRYSODA_LOG_FORMAT("Cost: %lf seconds\n", b - a);

	Exit();
}

void MathBench::LoadContent()
{
	base::LoadContent();

	// Load textures, sprites, shaders and other resources here
}

