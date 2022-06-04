#ifndef _CHERRYSODA_COMPONENTS_MESHGRAPHICSCOMPONENT_H_
#define _CHERRYSODA_COMPONENTS_MESHGRAPHICSCOMPONENT_H_

#include <CherrySoda/Components/Graphics/GraphicsComponent.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Graphics/Texture.h>
#include <CherrySoda/Graphics/Mesh.h>

namespace cherrysoda {

template <class VERTEX_T>
class MeshGraphicsComponent : public GraphicsComponent
{
public:
	// TODO: It needs some workaround here for the template name
	typedef VERTEX_T VertexType;
	CHERRYSODA_DECLARE_COMPONENT(MeshGraphicsComponent<VertexType>, GraphicsComponent);

	MeshGraphicsComponent(bool active) : base(active) {}

	void Render() override
	{
		// TODO: Make mesh state setting more general purpose
		if (m_baseColorTexture != Graphics::InvalidHandle) {
			Graphics::SetTexture(m_baseColorTexture);
		}

		Graphics::SetTransformMatrix(GetTransformMatrix());
		Graphics::SubmitMesh(GetMesh());
	}

	void InitWithMeshInfo(const Graphics::MeshInfo& meshInfo)
	{
		m_baseColorTexture = meshInfo.baseColorTexture;
		m_normalTexture = meshInfo.normalTexture;
		m_metallicRoughness = meshInfo.metallicRoughnessTexture;
		GetMesh()->SubmitBufferWithMeshInfo(meshInfo);
	}

	Mesh<VERTEX_T>* GetMesh() { return &m_mesh; }

private:
	Mesh<VERTEX_T> m_mesh;
	Graphics::TextureHandle m_baseColorTexture = Graphics::InvalidHandle;
	Graphics::TextureHandle m_normalTexture = Graphics::InvalidHandle;
	Graphics::TextureHandle m_metallicRoughness = Graphics::InvalidHandle;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_MESHGRAPHICSCOMPONENT_H_
