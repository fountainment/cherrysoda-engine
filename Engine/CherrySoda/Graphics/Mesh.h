#ifndef _CHERRYSODA_GRAPHICS_MESH_H_
#define _CHERRYSODA_GRAPHICS_MESH_H_

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/Literals.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

namespace cherrysoda {

class MeshInterface
{
public:
	virtual Graphics::VertexBufferHandle GetVertexBuffer() const = 0;
	virtual Graphics::IndexBufferHandle GetIndexBuffer() const = 0;
};

template <class VERTEX_T>
class Mesh : public MeshInterface
{
public:
	friend class Graphics;

	// void LoadObj(const String& objFile);
	inline void AddVertex(const VERTEX_T& vertex) { STL::Add(m_vertices, vertex); }
	inline void AddIndex(type::UInt16 index) { STL::Add(m_indices, index); }
	inline type::UInt16 VertexAmount() const { return static_cast<type::UInt16>(STL::Count(m_vertices)); }
	inline type::UInt16 IndexAmount() const { return STL::Count(m_indices); }
	inline void AddPoint(VERTEX_T v) { STL::Add(m_indices, VertexAmount()); STL::Add(m_vertices, v); }
	inline void AddLine(VERTEX_T v1, VERTEX_T v2) { AddPoint(v1); AddPoint(v2); }
	inline void AddTriangle(VERTEX_T v1, VERTEX_T v2, VERTEX_T v3) { AddPoint(v1); AddPoint(v2); AddPoint(v3); }
	inline void AddQuad(VERTEX_T v1, VERTEX_T v2, VERTEX_T v3, VERTEX_T v4)
	{
		const type::UInt16 i = VertexAmount();
		STL::AddRange(m_vertices, {v1, v2, v3, v4});
		STL::AddRange(m_indices,  {i, i+1_su, i+2_su, i+1_su, i+3_su, i+2_su});
	}
	inline void AddCube(VERTEX_T v1, VERTEX_T v2, VERTEX_T v3, VERTEX_T v4, VERTEX_T v5, VERTEX_T v6, VERTEX_T v7, VERTEX_T v8)
	{
		const type::UInt16 i = VertexAmount();
		STL::AddRange(m_vertices, {v1, v2, v3, v4, v5, v6, v7, v8});
		STL::AddRange(m_indices,  {i     , i+1_su, i+2_su, i+1_su, i+3_su, i+2_su});
		STL::AddRange(m_indices,  {i+4_su, i+6_su, i+5_su, i+5_su, i+6_su, i+7_su});
		STL::AddRange(m_indices,  {i     , i+2_su, i+4_su, i+4_su, i+2_su, i+6_su});
		STL::AddRange(m_indices,  {i+1_su, i+5_su, i+3_su, i+5_su, i+7_su, i+3_su});
		STL::AddRange(m_indices,  {i     , i+4_su, i+1_su, i+4_su, i+5_su, i+1_su});
		STL::AddRange(m_indices,  {i+2_su, i+3_su, i+6_su, i+6_su, i+3_su, i+7_su});
	}

	void InitBuffer()
	{
		m_vertexBuffer = Graphics::CreateVertexBuffer(m_vertices);
		m_indexBuffer = Graphics::CreateIndexBuffer(m_indices);
	}

	Graphics::VertexBufferHandle GetVertexBuffer() const { return m_vertexBuffer; }
	Graphics::IndexBufferHandle GetIndexBuffer() const { return m_indexBuffer; }

private:
	Graphics::VertexBufferHandle m_vertexBuffer = Graphics::InvalidHandle;
	Graphics::IndexBufferHandle m_indexBuffer = Graphics::InvalidHandle;

	STL::Vector<VERTEX_T> m_vertices;
	STL::Vector<type::UInt16> m_indices;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_MESH_H_