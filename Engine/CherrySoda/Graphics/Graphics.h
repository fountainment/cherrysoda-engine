#ifndef _CHERRYSODA_GRAPHICS_GRAPHICS_H_
#define _CHERRYSODA_GRAPHICS_GRAPHICS_H_

#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Literals.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>


namespace cherrysoda {

class Camera;
class Effect;
class MeshInterface;
class RenderTarget2D;
class Texture;
class Texture2D;
class TextureCube;

class Graphics
{
public:
	enum class BlendFunction : type::UInt8
	{
		Default = 0, // support premultiplied-alpha
		Alpha,
		Add,
		Max,
		Min,
		Multiply,
		Count
	};

	enum class PrimitiveType : type::UInt8
	{
		Triangles,
		Lines,
		Points,
		Count
	};

	enum class BufferType : type::UInt8
	{
		Static,
		Dynamic,
		Transient
	};

	struct VertexInfo
	{
		Math::Vec3 position;
		Math::Vec4 color;
		Math::Vec3 normal;
		Math::Vec2 texcoord0;
	};

	struct MeshInfo
	{
		STL::Vector<VertexInfo> vertices;
		STL::Vector<type::UInt16> indices;
	};

	static constexpr type::UInt32 EncodeNormalU32(const Math::Vec3& v)
	{
		type::UInt32 a = static_cast<type::UInt32>((v.x + 1.0f) * 0.5f * 255.f + 0.5f);
		type::UInt32 b = static_cast<type::UInt32>((v.y + 1.0f) * 0.5f * 255.f + 0.5f);
		type::UInt32 c = static_cast<type::UInt32>((v.z + 1.0f) * 0.5f * 255.f + 0.5f);
		return a | b << 8 | c << 16;
	}

	struct PosColorVertex
	{
		float m_x, m_y, m_z;
		type::UInt32 m_abgr;

		static void Init();

		static inline const PosColorVertex MakeVertex(const Math::Vec3& p, type::UInt32 c)
		{
			return { p[0], p[1], p[2], c };
		}
		static inline const PosColorVertex MakeVertex(const Math::Vec3& p, const Color& c = Color::White)
		{
			return { p[0], p[1], p[2], c.U32ABGR() };
		}

		static inline const PosColorVertex MakeVertex(const VertexInfo& vertex)
		{
			return MakeVertex(vertex.position, Color(vertex.color));
		}
	};

	struct PosColorNormalVertex
	{
		float m_x, m_y, m_z;
		type::UInt32 m_abgr;
		type::UInt32 m_normal;

		static void Init();

		static inline const PosColorNormalVertex MakeVertex(const Math::Vec3& p, type::UInt32 c, const Math::Vec3& n = Vec3_ZUp)
		{
			return { p[0], p[1], p[2], c, EncodeNormalU32(n) };
		}
		static inline const PosColorNormalVertex MakeVertex(const Math::Vec3& p, const Color& c, const Math::Vec3& n = Vec3_ZUp)
		{
			return { p[0], p[1], p[2], c.U32ABGR(), EncodeNormalU32(n) };
		}

		static inline const PosColorNormalVertex MakeVertex(const VertexInfo& vertex)
		{
			return MakeVertex(vertex.position, Color(vertex.color), vertex.normal);
		}
	};

	struct PosColorTexCoord0Vertex // default SpriteBatch vertex
	{
		float m_x, m_y, m_z;
		type::UInt32 m_abgr;
		float m_u, m_v;

		static void Init();

		static inline const PosColorTexCoord0Vertex MakeVertex(const Math::Vec3& p, type::UInt32 c, const Math::Vec2& uv = Vec2_Zero)
		{
			return { p[0], p[1], p[2], c, uv[0], uv[1] };
		}
		static inline const PosColorTexCoord0Vertex MakeVertex(const Math::Vec3& p, const Color& c, const Math::Vec2& uv = Vec2_Zero)
		{
			return { p[0], p[1], p[2], c.U32ABGR(), uv[0], uv[1] };
		}

		static inline const PosColorTexCoord0Vertex MakeVertex(const VertexInfo& vertex)
		{
			return MakeVertex(vertex.position, Color(vertex.color), vertex.texcoord0);
		}
	};

	struct PosNormalTexCoord0Vertex // default Model vertex
	{
		float m_x, m_y, m_z;
		type::UInt32 m_normal;	
		float m_u, m_v;

		static void Init();

		static inline const PosNormalTexCoord0Vertex MakeVertex(const Math::Vec3& p, const Math::Vec3& n = Vec3_ZUp, const Math::Vec2& uv = Vec2_Zero)
		{
			return { p[0], p[1], p[2], EncodeNormalU32(n), uv[0], uv[1] };
		}

		static inline const PosNormalTexCoord0Vertex MakeVertex(const VertexInfo& vertex)
		{
			return MakeVertex(vertex.position, vertex.normal, vertex.texcoord0);
		}
	};

	struct PosColorNormalTexCoord0Vertex
	{
		float m_x, m_y, m_z;
		type::UInt32 m_abgr;
		type::UInt32 m_normal;	
		float m_u, m_v;

		static void Init();

		static inline const PosColorNormalTexCoord0Vertex MakeVertex(const Math::Vec3& p, type::UInt32 c, const Math::Vec3& n = Vec3_ZUp, const Math::Vec2& uv = Vec2_Zero)
		{
			return { p[0], p[1], p[2], c, EncodeNormalU32(n), uv[0], uv[1] };
		}
		static inline const PosColorNormalTexCoord0Vertex MakeVertex(const Math::Vec3& p, const Color& c, const Math::Vec3& n = Vec3_ZUp, const Math::Vec2& uv = Vec2_Zero)
		{
			return { p[0], p[1], p[2], c.U32ABGR(), EncodeNormalU32(n), uv[0], uv[1] };
		}

		static inline const PosColorNormalTexCoord0Vertex MakeVertex(const VertexInfo& vertex)
		{
			return MakeVertex(vertex.position, Color(vertex.color), vertex.normal, vertex.texcoord0);
		}
	};

	struct ImGuiVertex
	{
		float m_x, m_y;
		float m_u, m_v;
		type::UInt32 m_abgr;

		static void Init();
	};

	struct TextureInfo
	{
		int m_width;
		int m_height;
		bool m_cubeMap;
	};

	using HandleType = type::UInt16;
	using BufferHandle = HandleType;
	using FrameBufferHandle = HandleType;
	using VertexBufferHandle = HandleType;
	using IndexBufferHandle  = HandleType;
	using DynamicVertexBufferHandle = HandleType;
	using DynamicIndexBufferHandle  = HandleType;
	using ShaderHandle = HandleType;
	using UniformHandle = HandleType;
	using TextureHandle = HandleType;
	using TransientIndexBufferHandle = HandleType;
	using TransientVertexBufferHandle = HandleType;
	static constexpr HandleType InvalidHandle = UINT16_MAX;

	static void SetPlatformData(void* platformData);

	static void Initialize();
	static void Terminate();

	void RenderFrame();

	static inline void BeginRenderPass(type::UInt16 renderPassId) { Instance()->RenderPass(renderPassId); }
	static inline void EndRenderPass(type::UInt16 renderPassId)
	{
		CHERRYSODA_ASSERT_FORMAT(Instance()->RenderPass() == renderPassId, "Current RenderPass %u != %u!\n", Instance()->RenderPass(), renderPassId);
		Discard();
		Instance()->RenderPass(0);
	}
	static inline type::UInt16 CurrentRenderPass() { return Instance()->m_renderPassId; }
	static inline Graphics* UseRenderPass(type::UInt16 renderPassId) { ms_renderPassHelperInstance->RenderPass(renderPassId); return ms_renderPassHelperInstance; }
	static inline Graphics* UseCurrentRenderPass() { return Instance(); }
	static inline type::UInt16 MaxRenderPassCount() { return ms_maxRenderPassCount; }

	static Math::IVec2 GetRenderTargetSize(RenderTarget2D* renderTarget);

	static void UpdateView();
	static void SetLinearTextureSampling();
	static void SetPointTextureSampling();
	void SetRenderTarget(RenderTarget2D* renderTarget);
	void SetClearColor(const Color& color);
	void SetClearDiscard();
	static void SetRenderPassOrder(STL::Vector<type::UInt16> renderPassOrder);
	void Touch();
	static void SetVsyncEnabled(bool vsyncEnabled);
	static inline bool IsOriginBottomLeft() { return ms_originBottomLeft; }
	static inline float TexelHalf() { return ms_texelHalf; }
	void SetViewport(int x, int y, int w, int h);
	void SetCamera(Camera* camera);
	void SetViewProjectionMatrix(const Math::Mat4& viewMatrix, const Math::Mat4& projMatrix);
	static void SetTransformMatrix(const Math::Mat4& transformMatrix);
	static void SetMesh(const MeshInterface* mesh);
	static void SubmitMesh(const MeshInterface* mesh);
	static void SetVertexBuffer(VertexBufferHandle vertexBuffer);
	static void SetIndexBuffer(IndexBufferHandle indexBuffer);
	static void SetDynamicVertexBuffer(DynamicVertexBufferHandle vertexBuffer, size_t vertexAmount);
	static void SetDynamicIndexBuffer(DynamicIndexBufferHandle indexBuffer, size_t indexAmount);
	static void SetTransientVertexBuffer(TransientVertexBufferHandle vertexBuffer);
	static void SetTransientIndexBuffer(TransientIndexBufferHandle indexBuffer, size_t startIndex, size_t indexAmount);
	static void SetStateDefault(BlendFunction blendFunc = BlendFunction::Default, PrimitiveType = PrimitiveType::Triangles);
	static void SetStateNoDepth(BlendFunction blendFunc = BlendFunction::Default, PrimitiveType = PrimitiveType::Triangles);
	void Submit();
	void Submit(const Effect* effect);
	static void Submit(type::UInt16 renderPass);
	static void Submit(type::UInt16 renderPass, const Effect* effect);
	static void SubmitOnCurrentRenderPass();
	static void SubmitOnCurrentRenderPass(const Effect* effect);

	static void Discard();

	static void ScreenSpaceQuad(float _width = 1.0f, float _height = 1.0f, bool _originBottomLeft = false);

	static IndexBufferHandle CreateIndexBuffer(STL::Vector<type::UInt16>& indices);
	static DynamicIndexBufferHandle CreateDynamicIndexBuffer(STL::Vector<type::UInt16>& indices);
	static TransientIndexBufferHandle CreateTransientIndexBuffer(const STL::Vector<type::UInt16>& indices);
	static TransientIndexBufferHandle CreateTransientIndexBuffer(const type::UInt16* indices, type::UInt32 indexAmount);

	static ShaderHandle CreateShaderProgramFromFile(const String& vs, const String& fs);
	static ShaderHandle CreateShaderProgramFromEmbedded(const String& vs, const String& fs);

	static const Effect GetEmbeddedEffect(StringID name);

	static TextureHandle CreateTexture(const String& texture, Graphics::TextureInfo* info = nullptr);
	static TextureHandle CreateTexture2DFromRGBA(void* data, int width, int height);
	static TextureHandle CreateTexture2DForColorBuffer(int width, int height);
	static TextureHandle CreateTexture2DForDepthBuffer(int width, int height);

	static FrameBufferHandle CreateFrameBuffer(int num, const TextureHandle* handles);

	static UniformHandle CreateUniformVec4(const String& uniform, type::UInt16 num = 1U);
	static UniformHandle CreateUniformMat4(const String& uniform);
	static UniformHandle CreateUniformSampler(const String& sampler);

	static void UpdateDynamicIndexBuffer(DynamicIndexBufferHandle handle, int index, const STL::Vector<cherrysoda::type::UInt16>& indices);

	static void DestroyVertexBuffer(VertexBufferHandle vertexBuffer);
	static void DestroyDynamicVertexBuffer(DynamicVertexBufferHandle vertexBuffer);
	static void DestroyIndexBuffer(IndexBufferHandle indexBuffer);
	static void DestroyDynamicIndexBuffer(DynamicIndexBufferHandle indexBuffer);
	static void DestroyShader(ShaderHandle shader);
	static void DestroyTexture(TextureHandle texture);

	static void SetScissor(int x, int y, int w, int h);
	static void SetShader(ShaderHandle shader) { ms_defaultShaderOverride = shader; }
	static void SetTexture(UniformHandle uniform, TextureHandle texture);
	static void SetTexture(type::UInt8 stage, UniformHandle uniform, TextureHandle texture);

	static void SetEffect(const Effect* effect);
	static void SetTexture(const Texture* texture);

	static void SetUniform(UniformHandle uniform, const void* value, type::UInt16 size = 1U);
	static void SetUniform(StringID uniformName, const void* value, type::UInt16 size = 1U);

	static void SetupEngineUniforms();

	static void SetUniformCamPos(const Math::Vec3& camPos);
	static void SetUniformMaterial(const Math::Vec3& albedo, float metallics, float roughness, float ao);
	static void SetUniformLight(int index, const Math::Vec3& lightPos, const Math::Vec3& lightColor, bool submit = true);
	static void SubmitUniformLight();

	static void SetTextureCube(const TextureCube* texture);
	static void SetTextureCubeIrr(const TextureCube* texture);

private:
	friend class Engine;

	Graphics() {}

	inline void RenderPass(type::UInt16 renderPassId) { m_renderPassId = renderPassId; }
	inline type::UInt16 RenderPass() { return m_renderPassId; }	

	static inline ShaderHandle CurrentShader() { return ms_defaultShaderOverride != Graphics::InvalidHandle ? ms_defaultShaderOverride : ms_defaultShader; }

	static inline Graphics* Instance() { return ms_instance; }

	type::UInt16 m_renderPassId = 0;

	static type::UInt64 ms_blendFunctions[(int)BlendFunction::Count];
	static type::UInt64 ms_primitiveTypes[(int)PrimitiveType::Count];
	static type::UInt16 ms_maxRenderPassCount;
	static bool ms_vsyncEnabled;

	static bool ms_originBottomLeft;
	static float ms_texelHalf;

	static ShaderHandle ms_defaultShader;
	static ShaderHandle ms_defaultShaderOverride;
	
	static STL::HashMap<StringID, UniformHandle> ms_uniformHashMap;

	static UniformHandle ms_samplerTex;
	static UniformHandle ms_samplerTexCube;
	static UniformHandle ms_samplerTexCubeIrr;

	static UniformHandle ms_uniformTime;
	static UniformHandle ms_uniformResolution;

	static UniformHandle ms_uniformCamPos;
	static UniformHandle ms_uniformLights;
	static UniformHandle ms_uniformMaterial;

	static Graphics* ms_instance;
	static Graphics* ms_renderPassHelperInstance;

public:
	#define CHERRYSODA_VERTEX_DECLARATION(VERTEX_T) \
	static VertexBufferHandle CreateVertexBuffer(const STL::Vector<VERTEX_T>& vertices); \
	static DynamicVertexBufferHandle CreateDynamicVertexBuffer(const STL::Vector<VERTEX_T>& vertices); \
	static void UpdateDynamicVertexBuffer(DynamicVertexBufferHandle handle, int index, const STL::Vector<VERTEX_T>& vertices); \
	static TransientVertexBufferHandle CreateTransientVertexBuffer(const STL::Vector<VERTEX_T>& vertices); \
	static TransientVertexBufferHandle CreateTransientVertexBuffer(const VERTEX_T* vertices, type::UInt32 vertexAmount);

	CHERRYSODA_VERTEX_DECLARATION(PosColorVertex);
	CHERRYSODA_VERTEX_DECLARATION(PosColorNormalVertex);
	CHERRYSODA_VERTEX_DECLARATION(PosColorTexCoord0Vertex);
	CHERRYSODA_VERTEX_DECLARATION(PosNormalTexCoord0Vertex);
	CHERRYSODA_VERTEX_DECLARATION(PosColorNormalTexCoord0Vertex);
	CHERRYSODA_VERTEX_DECLARATION(ImGuiVertex);

	#undef CHERRYSODA_VERTEX_DECLARATION
};

class MeshInterface
{
public:
	virtual void SetBufferType(Graphics::BufferType bufferType) = 0;
	virtual Graphics::BufferType GetBufferType() const = 0;
	virtual void SetPrimitiveType(Graphics::PrimitiveType primitiveType) = 0;
	virtual Graphics::PrimitiveType GetPrimitiveType() const = 0;

	virtual size_t VertexBufferSize() const = 0;
	virtual size_t IndexBufferSize() const = 0;
	virtual Graphics::BufferHandle GetVertexBuffer() const = 0;
	virtual Graphics::BufferHandle GetIndexBuffer() const = 0;
	virtual Graphics::TransientVertexBufferHandle CreateTransientVertexBuffer() const = 0;
	virtual Graphics::TransientIndexBufferHandle CreateTransientIndexBuffer() const = 0;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_GRAPHICS_H_
