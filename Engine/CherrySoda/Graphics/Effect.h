#ifndef _CHERRYSODA_GRAPHICS_EFFECT_H_
#define _CHERRYSODA_GRAPHICS_EFFECT_H_

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/String.h>

namespace cherrysoda {

class Effect
{
public:
	void Load(const String& name)
	{
		Load("vs_" + name, "fs_" + name);
	}

	static const Effect LoadEffect(const String& name)
	{
		Effect effect;
		effect.Load(name);
		return effect;
	}

private:
	friend class Graphics;

	void Load(const String& vs, const String& fs)
	{
		m_program = Graphics::CreateShaderProgram(vs, fs);
	}

	inline Graphics::ShaderHandle GetShader() const { return m_program; }

	Graphics::ShaderHandle m_program = Graphics::InvalidHandle;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_EFFECT_H_
