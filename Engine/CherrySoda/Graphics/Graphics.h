#ifndef _CHERRYSODA_GRAPHICS_GRAPHICS_H_
#define _CHERRYSODA_GRAPHICS_GRAPHICS_H_

#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Literals.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

#if defined(_WIN32)
#	define CHERRYSODA_SHADER_PLATFORM "windows"
#elif defined(__ANDROID__)
#	define CHERRYSODA_SHADER_PLATFORM "android"
#elif defined(__arm__)
#	define CHERRYSODA_SHADER_PLATFORM "nacl"
#else
#	define CHERRYSODA_SHADER_PLATFORM "linux"
#endif

namespace cherrysoda {

class Camera;
class Effect;
class MeshInterface;
class Texture;

class Graphics
{
public:
	struct PosColorVertex
	{
		float m_x, m_y, m_z;
		type::UInt32 m_abgr;

		static void Init();
		static PosColorVertex MakeVertex(const Math::Vec3& p, type::UInt32 c)
		{
			return { p[0], p[1], p[2], c };
		}
	};

	struct PosColorNormalVertex
	{
		float m_x, m_y, m_z;
		type::UInt32 m_abgr;
		float m_nx, m_ny, m_nz;	

		static void Init();
		static PosColorNormalVertex MakeVertex(const Math::Vec3& p, type::UInt32 c, const Math::Vec3& n)
		{
			return { p[0], p[1], p[2], c, n[0], n[1], n[2] };
		}
	};

	struct PosColorTexCoord0Vertex
	{
		float m_x, m_y, m_z;
		type::UInt32 m_abgr;
		float m_u, m_v;

		static void Init();
		static PosColorTexCoord0Vertex MakeVertex(const Math::Vec3& p, type::UInt32 c, const Math::Vec2& uv)
		{
			return { p[0], p[1], p[2], c, uv[0], uv[1] };
		}
	};

	using HandleType = type::UInt16;
	using VertexBufferHandle = HandleType;
	using IndexBufferHandle  = HandleType;
	using ShaderHandle = HandleType;
	using UniformHandle = HandleType;
	using TextureHandle = HandleType;
	static constexpr HandleType InvalidHandle = UINT16_MAX;

	static void Init();
	static void Terminate();

	void RenderFrame();

	inline void RenderPass(type::UInt16 renderPassId) { m_renderPassId = renderPassId; }
	inline type::UInt16 RenderPass() { return m_renderPassId; }
	void UpdateView();
	void SetClearColor(const Color& color);
	void SetClearDiscard();
	void Touch();
	void SetVsyncEnabled(bool vsyncEnabled);
	void SetViewport(int x, int y, int w, int h);
	void SetCamera(Camera* camera);
	void SetTransformMatrix(const Math::Mat4& transformMatrix);
	void SetMesh(MeshInterface* mesh);
	void SetVertexBuffer(VertexBufferHandle vertexBuffer);
	void SetIndexBuffer(IndexBufferHandle indexBuffer);
	void Submit();
	void Submit(Effect* effect);
	void Submit(type::UInt16 renderPass, Effect* effect);

	void ScreenSpaceQuad(float _textureWidth, float _textureHeight, bool _originBottomLeft = false, float _width = 1.0f, float _height = 1.0f);

	static VertexBufferHandle CreateVertexBuffer(STL::Vector<PosColorVertex>& vertices);
	static VertexBufferHandle CreateVertexBuffer(STL::Vector<PosColorNormalVertex>& vertices);
	static IndexBufferHandle CreateIndexBuffer(STL::Vector<type::UInt16>& indices);

	static ShaderHandle CreateShaderProgram(const String& vs, const String& fs);
	static TextureHandle CreateTexture(const String& texture);

	static UniformHandle CreateUniformVec4(const String& uniform, type::UInt16 num = 1U);
	static UniformHandle CreateUniformMat4(const String& uniform);
	static UniformHandle CreateUniformSampler(const String& sampler);

	static void SetShader(ShaderHandle shader) { ms_defaultShaderOverride = shader; }
	static void SetTexture(UniformHandle uniform, TextureHandle texture);
	static void SetTexture(type::UInt8 stage, UniformHandle uniform, TextureHandle texture);

	static void SetEffect(Effect* effect);
	static void SetTexture(Texture* texture);

	static void SetUniform(UniformHandle uniform, const void* value, type::UInt16 size = 1U);
	static void SetUniformCamPos(const Math::Vec3& camPos);
	static void SetUniformMaterial(const Math::Vec3& albedo, float metallics, float roughness, float ao);
	static void SetUniformLight(int index, const Math::Vec3& lightPos, const Math::Vec3& lightColor);

	static void SetSamplerTexCube(Texture* texture);
	static void SetSamplerTexCubeIrr(Texture* texture);

	static Graphics* Instance() { return ms_instance; };

private:
	friend class Engine;

	Graphics();

	type::UInt16 m_renderPassId = 0;	
	bool m_vsyncEnabled = true;

	static ShaderHandle ms_defaultShader;
	static ShaderHandle ms_defaultShaderOverride;
	
	static UniformHandle ms_samplerTexCube;
	static UniformHandle ms_samplerTexCubeIrr;

	static UniformHandle ms_uniformCamPos;
	static UniformHandle ms_uniformLights;
	static UniformHandle ms_uniformMaterial;
	static UniformHandle ms_uniformParams;

	static Graphics* ms_instance;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_GRAPHICS_H_