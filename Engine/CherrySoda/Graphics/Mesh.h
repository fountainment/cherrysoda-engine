#ifndef _CHERRYSODA_GRAPHICS_MESH_H_
#define _CHERRYSODA_GRAPHICS_MESH_H_

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

namespace cherrysoda {

template <class VERTEX_T>
class Mesh
{
public:
	friend class Graphics;

	// void LoadObj(const String& objFile);
	inline void AddVertex(const VERTEX_T& vertex) { STL::Add(m_vertices, vertex); }
	inline void AddIndex(type::UInt16 index) { STL::Add(m_indices, index); }
	inline type::UInt16 VertexAmount() const { return STL::Count(m_vertices); }
	inline type::UInt16 IndexAmount() const { return STL::Count(m_indices); }
	inline void AddPoint(VERTEX_T v) { STL::Add(m_indices, VertexAmount()); STL::Add(m_vertices, v); }
	inline void AddLine(VERTEX_T v1, VERTEX_T v2) { AddPoint(v1); AddPoint(v2); }
	inline void AddTriangle(VERTEX_T v1, VERTEX_T v2, VERTEX_T v3) { AddPoint(v1); AddPoint(v2); AddPoint(v3); }
	inline void AddQuad(VERTEX_T v1, VERTEX_T v2, VERTEX_T v3, VERTEX_T v4)
	{
		const type::UInt16 i = VertexAmount();
		STL::AddRange(m_vertices, {v1, v2, v3, v4});
		STL::AddRange(m_indices,  {i, i+1, i+2, i+1, i+3, i+2});
	}
	inline void AddCube(VERTEX_T v1, VERTEX_T v2, VERTEX_T v3, VERTEX_T v4, VERTEX_T v5, VERTEX_T v6, VERTEX_T v7, VERTEX_T v8)
	{
		const type::UInt16 i = VertexAmount();
		STL::AddRange(m_vertices, {v1, v2, v3, v4, v5, v6, v7, v8});
		STL::AddRange(m_indices,  {i+0, i+1, i+2, i+1, i+3, i+2});
		STL::AddRange(m_indices,  {i+4, i+6, i+5, i+5, i+6, i+7});
		STL::AddRange(m_indices,  {i+0, i+2, i+4, i+4, i+2, i+6});
		STL::AddRange(m_indices,  {i+1, i+5, i+3, i+5, i+7, i+3});
		STL::AddRange(m_indices,  {i+0, i+4, i+1, i+4, i+5, i+1});
		STL::AddRange(m_indices,  {i+2, i+3, i+6, i+6, i+3, i+7});
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