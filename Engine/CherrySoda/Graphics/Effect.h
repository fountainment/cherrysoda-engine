#ifndef _CHERRYSODA_GRAPHICS_EFFECT_H_
#define _CHERRYSODA_GRAPHICS_EFFECT_H_

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/String.h>

namespace cherrysoda {

class Effect
{
public:
	Effect() = default;

	Effect(Graphics::ShaderHandle shader)
	{
		m_program = shader;
	}

	void LoadFromFile(const String& name)
	{
		m_program = Graphics::CreateShaderProgramFromFile("vs_" + name, "fs_" + name);
	}

	void LoadFromEmbedded(const String& name)
	{
		m_program = Graphics::CreateShaderProgramFromEmbedded("vs_" + name, "fs_" + name);
	}

	static const Effect LoadEffectFromFile(const String& name)
	{
		Effect effect;
		effect.LoadFromFile(name);
		return effect;
	}

	static const Effect LoadEffectFromEmbedded(const String& name)
	{
		Effect effect;
		effect.LoadFromEmbedded(name);
		return effect;
	}

	inline Graphics::ShaderHandle GetShader() const { return m_program; }
private:
	friend class Graphics;

	Graphics::ShaderHandle m_program = Graphics::InvalidHandle;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_EFFECT_H_
