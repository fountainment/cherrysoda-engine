#ifndef _CHERRYSODA_GRAPHICS_MESH_H_
#define _CHERRYSODA_GRAPHICS_MESH_H_

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/Literals.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

#define MK_VERT VertexType::MakeVertex

namespace cherrysoda {

class MeshInterface
{
public:
	enum class BufferType
	{
		Static,
		Dynamic,
		Transient
	};

	virtual void SetBufferType(BufferType bufferType) { CHERRYSODA_ASSERT(false, "There is no SetBufferType override!\n"); }
	virtual BufferType GetBufferType() const { return BufferType::Static; }

	virtual size_t VertexBufferSize() const = 0;
	virtual size_t IndexBufferSize() const = 0;
	virtual Graphics::BufferHandle GetVertexBuffer() const = 0;
	virtual Graphics::BufferHandle GetIndexBuffer() const = 0;

	virtual void SetBuffer() const = 0;
};


template <class VERTEX_T>
class Mesh : public MeshInterface
{
public:
	friend class Graphics;

	typedef VERTEX_T VertexType;

	inline VERTEX_T* VertexBufferData() { return STL::Data(m_verticesFront); }
	inline type::UInt16* IndexBufferData() { return STL::Data(m_indicesFront); }
	inline size_t VertexBufferSize() const { return STL::Count(m_verticesFront); }
	inline size_t IndexBufferSize() const { return STL::Count(m_indicesFront); }

	// void LoadObj(const String& objFile);
	inline void AddVertex(const VERTEX_T& vertex) { STL::Add(m_vertices, vertex); }
	inline void AddIndex(type::UInt16 index) { STL::Add(m_indices, index); }
	inline size_t VertexAmount() const { return STL::Count(m_vertices); }
	inline size_t IndexAmount() const { return STL::Count(m_indices); }
	inline void AddPoint(const VERTEX_T& v) { AddIndex(static_cast<type::UInt16>(VertexAmount())); AddVertex(v);}
	inline void AddPointNoIndex(const VERTEX_T& v) { AddVertex(v); }
	inline void AddLine(const VERTEX_T& v1, const VERTEX_T& v2) { AddPoint(v1); AddPoint(v2); }
	inline void AddLineNoIndex(const VERTEX_T& v1, const VERTEX_T& v2) { AddPointNoIndex(v1); AddPointNoIndex(v2); }
	inline void AddTriangle(const VERTEX_T& v1, const VERTEX_T& v2, const VERTEX_T& v3) { AddPoint(v1); AddPoint(v2); AddPoint(v3); }
	inline void AddTriangleNoIndex(const VERTEX_T& v1, const VERTEX_T& v2, const VERTEX_T& v3) { AddPointNoIndex(v1); AddPointNoIndex(v2); AddPointNoIndex(v3); }
	inline void AddQuad(const VERTEX_T& v1, const VERTEX_T& v2, const VERTEX_T& v3, const VERTEX_T& v4)
	{
		CHERRYSODA_ASSERT(VertexAmount() + 4 <= UINT16_MAX, "Vertex amount beyond UINT16_MAX!\n");
		const type::UInt16 i = static_cast<type::UInt16>(VertexAmount());
		STL::AddRange(m_vertices, { v1, v2, v3, v4 });
		STL::AddRange(m_indices,  { i, i+1_su, i+2_su, i+1_su, i+3_su, i+2_su });
	}
	inline void AddQuadNoIndex(const VERTEX_T& v1, const VERTEX_T& v2, const VERTEX_T& v3, const VERTEX_T& v4)
	{
		CHERRYSODA_ASSERT(VertexAmount() + 6 <= UINT16_MAX, "Vertex amount beyond UINT16_MAX!\n");
		AddTriangleNoIndex(v1, v2, v3);
		AddTriangleNoIndex(v2, v4, v3);
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
		STL::Reserve(m_vertices, VertexAmount() + v);
		STL::Reserve(m_indices, IndexAmount() + i);
	}

	inline BufferType GetBufferType() const
	{
		return m_bufferType;
	}

	inline void SetBufferType(BufferType buffertype)
	{
		if (m_bufferType != buffertype) {
			DestroyBuffer();
			m_bufferType = buffertype;
			InitBuffer();
		}
	}

	void DestroyBuffer()
	{
		if (m_vertexBuffer != Graphics::InvalidHandle) {
			switch (GetBufferType()) {
			case BufferType::Static:
				Graphics::DestroyVertexBuffer(m_vertexBuffer);
				break;
			case BufferType::Dynamic:
				Graphics::DestroyDynamicVertexBuffer(m_vertexBuffer);
				break;
			case BufferType::Transient:
				break;
			}
			m_vertexBuffer = Graphics::InvalidHandle;
		}
		if (m_indexBuffer != Graphics::InvalidHandle) {
			switch (GetBufferType()) {
			case BufferType::Static:
				Graphics::DestroyIndexBuffer(m_indexBuffer);
				break;
			case BufferType::Dynamic:
				Graphics::DestroyDynamicIndexBuffer(m_vertexBuffer);
				break;
			case BufferType::Transient:
				break;
			}
			m_indexBuffer = Graphics::InvalidHandle;
		}
	}

	void SubmitBuffer()
	{
		switch (GetBufferType()) {
		case BufferType::Static:
			DestroyBuffer();
			InitBuffer();
			break;
		case BufferType::Dynamic:
			if (!IsValid()) {
				InitBuffer();
			}
			else if (STL::Count(m_vertices) > 0) {
				UpdateVertexBuffer(0, m_vertices);
				UpdateIndexBuffer(0, m_indices);
			}
			break;
		case BufferType::Transient:
			break;
		}
		SwapBuffer();
	}

	inline void UpdateVertexBuffer(int index, const STL::Vector<VERTEX_T>& vertices)
	{
		CHERRYSODA_ASSERT(GetBufferType() == BufferType::Dynamic, "Only dynamic buffer type can use UpdateVertexBuffer!\n");
		CHERRYSODA_ASSERT(IsValid(), "Only initialized buffer can be updated!\n");
		Graphics::UpdateDynamicVertexBuffer(m_vertexBuffer, index, m_vertices);
	}

	inline void UpdateIndexBuffer(int index, const STL::Vector<type::UInt16>& indices)
	{
		CHERRYSODA_ASSERT(GetBufferType() == BufferType::Dynamic, "Only dynamic buffer type can use UpdateIndexbBuffer!\n");
		CHERRYSODA_ASSERT(IsValid(), "Only initialized buffer can be updated!\n");
		Graphics::UpdateDynamicIndexBuffer(m_indexBuffer, index, indices);
	}

	inline bool IsValid() const
	{
		return m_vertexBuffer != Graphics::InvalidHandle;
	}

	inline Graphics::BufferHandle GetVertexBuffer() const { return m_vertexBuffer; }
	inline Graphics::BufferHandle GetIndexBuffer() const { return m_indexBuffer; }

	void SetBuffer() const
	{
		switch (GetBufferType()) {
		case MeshInterface::BufferType::Static:
			Graphics::SetVertexBuffer(GetVertexBuffer());
			Graphics::SetIndexBuffer(GetIndexBuffer());
			break;
		case MeshInterface::BufferType::Dynamic:
			Graphics::SetDynamicVertexBuffer(GetVertexBuffer(), VertexBufferSize());
			Graphics::SetDynamicIndexBuffer(GetIndexBuffer(), IndexBufferSize());
			break;
		case MeshInterface::BufferType::Transient:
			Graphics::SetTransientVertexBuffer(m_verticesFront);
			Graphics::SetTransientIndexBuffer(m_indicesFront);
			break;
		}	
	}

private:
	void InitBuffer()
	{
		CHERRYSODA_ASSERT(!IsValid(), "Mesh already initialized!\n");
		switch (GetBufferType()) {
		case BufferType::Static:
			if (STL::Count(m_vertices) > 0) {
				m_vertexBuffer = Graphics::CreateVertexBuffer(m_vertices);
				if (STL::Count(m_indices) > 0) {
					m_indexBuffer = Graphics::CreateIndexBuffer(m_indices);
				}
			}
			break;
		case BufferType::Dynamic:
			if (STL::Count(m_vertices) > 0) {
				m_vertexBuffer = Graphics::CreateDynamicVertexBuffer(m_vertices);
				if (STL::Count(m_indices) > 0) {
					m_indexBuffer = Graphics::CreateDynamicIndexBuffer(m_indices);
				}
			}
			break;
		case BufferType::Transient:
			break;
		}
	}

	void SwapBuffer()
	{
		STL::Swap(m_vertices, m_verticesFront);
		STL::Swap(m_indices, m_indicesFront);
	}

	Graphics::BufferHandle m_vertexBuffer = Graphics::InvalidHandle;
	Graphics::BufferHandle m_indexBuffer = Graphics::InvalidHandle;

	STL::Vector<VERTEX_T> m_vertices;
	STL::Vector<VERTEX_T> m_verticesFront;
	STL::Vector<type::UInt16> m_indices;
	STL::Vector<type::UInt16> m_indicesFront;

	BufferType m_bufferType = BufferType::Static;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_MESH_H_
