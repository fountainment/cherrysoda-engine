#ifndef _CHERRYSODA_GRAPHICS_GRAPHICS_H_
#define _CHERRYSODA_GRAPHICS_GRAPHICS_H_

#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class Camera;
class MeshInterface;

class Graphics
{
public:
	struct PosColorVertex
	{
		float m_x, m_y, m_z;
		type::UInt32 m_abgr;

		static void Init();
	};

	struct PosColorNormalVertex
	{
		float m_x, m_y, m_z;
		type::UInt32 m_abgr;
		float m_nx, m_ny, m_nz;	

		static void Init();
	};

	using HandleType = type::UInt16;
	using VertexBufferHandle = HandleType;
	using IndexBufferHandle  = HandleType;
	static constexpr HandleType InvalidHandle = UINT16_MAX;

	static void Init();
	static void Terminate();

	void RenderFrame();

	inline void RenderPass(type::UInt16 renderPassId) { m_renderPassId = renderPassId; }
	inline type::UInt16 RenderPass() { return m_renderPassId; }
	void UpdateView();
	void SetClearColor(const Color& color);
	void Touch();
	void SetVsyncEnabled(bool vsyncEnabled);
	void SetViewport(int x, int y, int w, int h);
	void SetCamera(Camera* camera);
	void SetTransformMatrix(const Math::Mat4& transformMatrix);
	void SetMesh(MeshInterface* mesh);
	void SetVertexBuffer(VertexBufferHandle vertexBuffer);
	void SetIndexBuffer(IndexBufferHandle indexBuffer);
	void Submit();

	static VertexBufferHandle CreateVertexBuffer(STL::Vector<PosColorVertex>& vertices);
	static VertexBufferHandle CreateVertexBuffer(STL::Vector<PosColorNormalVertex>& vertices);
	static IndexBufferHandle CreateIndexBuffer(STL::Vector<type::UInt16>& indices);

	static Graphics* Instance() { return ms_instance; };

private:
	friend class Engine;

	Graphics();

	type::UInt16 m_renderPassId = 0;	
	bool m_vsyncEnabled = true;

	static Graphics* ms_instance;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_GRAPHICS_H_