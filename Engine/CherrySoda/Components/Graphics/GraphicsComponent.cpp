#include <CherrySoda/Components/Graphics/GraphicsComponent.h>

using cherrysoda::GraphicsComponent;
using cherrysoda::Math;

const Math::Mat4 GraphicsComponent::GetTransformMatrix() const
{
	return Math::TranslateMat4(
		Math::ScaleMat4(
			Math::RotateMat4(
				Math::TranslateMat4(
					Mat4_Identity,
				RenderPosition()),
			ZRotation(), Vec3_ZUp + Vec3_XUp),
		Scale()),
	-Origin());
}