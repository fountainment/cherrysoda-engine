#ifndef _CHERRYSODA_COMPONENTS_MESHGRAPHICSCOMPONENT_H_
#define _CHERRYSODA_COMPONENTS_MESHGRAPHICSCOMPONENT_H_

#include <CherrySoda/Components/Graphics/GraphicsComponent.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Graphics/Mesh.h>

namespace cherrysoda {

template <class VERTEX_T>
class MeshGraphicsComponent : public cherrysoda::GraphicsComponent
{
public:
	typedef cherrysoda::GraphicsComponent base;
	typedef VERTEX_T VertexType;

	MeshGraphicsComponent(bool active) : base(active) {}

	void Render() override
	{
		cherrysoda::Graphics::SetTransformMatrix(GetTransformMatrix());
		cherrysoda::Graphics::SetMesh(GetMesh());
		cherrysoda::Graphics::SetStateDefault();
		cherrysoda::Graphics::Instance()->Submit();
	}

	cherrysoda::Mesh<VERTEX_T>* GetMesh() { return &m_mesh; }

private:
	cherrysoda::Mesh<VERTEX_T> m_mesh;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_MESHGRAPHICSCOMPONENT_H_
