#ifndef _CHERRYSODA_UTIL_MODEL_H_
#define _CHERRYSODA_UTIL_MODEL_H_

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/String.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class Model
{
public:
	const STL::Vector<Graphics::MeshInfo>& Meshes() const { return m_meshes; }

	inline void AddMesh(const Graphics::MeshInfo& mesh) { STL::Add(m_meshes, mesh); }

	static Model FromGltf(const String& gltfFile);
	// static Model FromObj(const String& objFile);

private:
	STL::Vector<Graphics::MeshInfo> m_meshes;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_CGLTFUTIL_H_
