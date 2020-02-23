#ifndef _CHERRYSODA_GRAPHICS_MESH_H_
#define _CHERRYSODA_GRAPHICS_MESH_H_

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/Literals.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

namespace cherrysoda {

class MeshInterface
{
public:
	virtual bool IsDynamic() const = 0;

	virtual size_t VertexBufferSize() const = 0;
	virtual size_t IndexBufferSize() const = 0;
	virtual Graphics::BufferHandle GetVertexBuffer() const = 0;
	virtual Graphics::BufferHandle GetIndexBuffer() const = 0;
};

template <class VERTEX_T>
class Mesh : public MeshInterface
{
public:
	friend class Graphics;

	inline size_t VertexBufferSize() const { return STL::Count(m_verticesFront); }
	inline size_t IndexBufferSize() const { return STL::Count(m_indicesFront); }

	// void LoadObj(const String& objFile);
	inline void AddVertex(const VERTEX_T& vertex) { STL::Add(m_vertices, vertex); }
	inline void AddIndex(type::UInt16 index) { STL::Add(m_indices, index); }
	inline size_t VertexAmount() const { return STL::Count(m_vertices); }
	inline size_t IndexAmount() const { return STL::Count(m_indices); }
	inline void AddPoint(const VERTEX_T& v) { STL::Add(m_indices, static_cast<type::UInt16>(VertexAmount())); STL::Add(m_vertices, v); }
	inline void AddLine(const VERTEX_T& v1, const VERTEX_T& v2) { AddPoint(v1); AddPoint(v2); }
	inline void AddTriangle(const VERTEX_T& v1, const VERTEX_T& v2, const VERTEX_T& v3) { AddPoint(v1); AddPoint(v2); AddPoint(v3); }
	inline void AddQuad(const VERTEX_T& v1, const VERTEX_T& v2, const VERTEX_T& v3, const VERTEX_T& v4)
	{
		CHERRYSODA_ASSERT(VertexAmount() + 4 <= UINT16_MAX, "Vertex amount beyond UINT16_MAX!\n");
		const type::UInt16 i = static_cast<type::UInt16>(VertexAmount());
		STL::AddRange(m_vertices, { v1, v2, v3, v4 });
		STL::AddRange(m_indices,  { i, i+1_su, i+2_su, i+1_su, i+3_su, i+2_su });
	}
	inline void AddCube(const VERTEX_T& v1, const VERTEX_T& v2, const VERTEX_T& v3, const VERTEX_T& v4, const VERTEX_T& v5, const VERTEX_T& v6, const VERTEX_T& v7, const VERTEX_T& v8)
	{
		CHERRYSODA_ASSERT(VertexAmount() + 8 <= UINT16_MAX, "Vertex amount beyond UINT16_MAX!\n");
		const type::UInt16 i = static_cast<type::UInt16>(VertexAmount());
		STL::AddRange(m_vertices, { v1, v2, v3, v4, v5, v6, v7, v8 });
		STL::AddRange(m_indices,  { i     , i+1_su, i+2_su, i+1_su, i+3_su, i+2_su });
		STL::AddRange(m_indices,  { i+4_su, i+6_su, i+5_su, i+5_su, i+6_su, i+7_su });
		STL::AddRange(m_indices,  { i     , i+2_su, i+4_su, i+4_su, i+2_su, i+6_su });
		STL::AddRange(m_indices,  { i+1_su, i+5_su, i+3_su, i+5_su, i+7_su, i+3_su });
		STL::AddRange(m_indices,  { i     , i+4_su, i+1_su, i+4_su, i+5_su, i+1_su });
		STL::AddRange(m_indices,  { i+2_su, i+3_su, i+6_su, i+6_su, i+3_su, i+7_su });
	}
	inline void Clear()
	{
		STL::Clear(m_vertices);
		STL::Clear(m_indices);
	}

	inline void ReserverAdditional(int v, int i)
	{
		STL::Reserve(m_vertices, STL::Count(m_vertices) + v);
		STL::Reserve(m_indices, STL::Count(m_indices) + i);
	}

	inline bool IsDynamic() const
	{
		return m_isDynamic;
	}

	inline void SetIsDynamic(bool isDynamic)
	{
		if (m_isDynamic != isDynamic) {
			DestroyBuffer();
			m_isDynamic = isDynamic;
			InitBuffer();
		}
	}

	void DestroyBuffer()
	{
		if (!IsDynamic()) {
			if (m_vertexBuffer != Graphics::InvalidHandle) {
				Graphics::DestroyVertexBuffer(m_vertexBuffer);
				m_vertexBuffer = Graphics::InvalidHandle;
			}
			if (m_indexBuffer != Graphics::InvalidHandle) {
				Graphics::DestroyIndexBuffer(m_indexBuffer);
				m_indexBuffer = Graphics::InvalidHandle;
			}
		}
		else {
			if (m_vertexBuffer != Graphics::InvalidHandle) {
				Graphics::DestroyDynamicVertexBuffer(m_vertexBuffer);
				m_vertexBuffer = Graphics::InvalidHandle;
			}
			if (m_indexBuffer != Graphics::InvalidHandle) {
				Graphics::DestroyDynamicIndexBuffer(m_indexBuffer);
				m_indexBuffer = Graphics::InvalidHandle;
			}
		}
	}

	void SwapLocalBuffer()
	{
		STL::Swap(m_vertices, m_verticesFront);
		STL::Swap(m_indices, m_indicesFront);
	}

	void SubmitBuffer()
	{
		if (!IsDynamic()) {
			DestroyBuffer();
			InitBuffer();
		}
		else {
			if (!IsValid()) {
				InitBuffer();
			}
			else if (STL::Count(m_vertices) > 0) {
				Graphics::UpdateDynamicVertexBuffer(m_vertexBuffer, 0, m_vertices);
				Graphics::UpdateDynamicIndexBuffer(m_indexBuffer, 0, m_indices);
			}
		}
		SwapLocalBuffer();
	}

	inline bool IsValid()
	{
		return m_vertexBuffer != Graphics::InvalidHandle && m_indexBuffer != Graphics::InvalidHandle;
	}

	inline Graphics::BufferHandle GetVertexBuffer() const { return m_vertexBuffer; }
	inline Graphics::BufferHandle GetIndexBuffer() const { return m_indexBuffer; }

private:
	void InitBuffer()
	{
		CHERRYSODA_ASSERT(!IsValid(), "Mesh already initialized!\n");
		if (!IsDynamic()) {
			if (STL::Count(m_vertices) > 0) {
				m_vertexBuffer = Graphics::CreateVertexBuffer(m_vertices);
				m_indexBuffer = Graphics::CreateIndexBuffer(m_indices);
			}
		}
		else {
			if (STL::Count(m_vertices) > 0) {
				m_vertexBuffer = Graphics::CreateDynamicVertexBuffer(m_vertices);
				m_indexBuffer = Graphics::CreateDynamicIndexBuffer(m_indices);
			}
		}
	}

	Graphics::BufferHandle m_vertexBuffer = Graphics::InvalidHandle;
	Graphics::BufferHandle m_indexBuffer = Graphics::InvalidHandle;

	STL::Vector<VERTEX_T> m_vertices;
	STL::Vector<type::UInt16> m_indices;

	STL::Vector<VERTEX_T> m_verticesFront;
	STL::Vector<type::UInt16> m_indicesFront;

	bool m_isDynamic = false;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_MESH_H_
