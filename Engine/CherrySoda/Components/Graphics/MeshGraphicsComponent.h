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
		if (m_tex.IsValid()) {
			Graphics::SetTexture(&m_tex);
		}

		Graphics::SetTransformMatrix(GetTransformMatrix());
		Graphics::SubmitMesh(GetMesh());
	}

	Mesh<VERTEX_T>* GetMesh() { return &m_mesh; }

	void SetTexture(const Texture2D& tex) { m_tex = tex; }

private:
	Mesh<VERTEX_T> m_mesh;
	Texture2D m_tex;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_MESHGRAPHICSCOMPONENT_H_
