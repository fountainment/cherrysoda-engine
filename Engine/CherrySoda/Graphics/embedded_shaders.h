#ifndef _CHERRYSODA_EMBEDDED_SHADERS_H_
#define _CHERRYSODA_EMBEDDED_SHADERS_H_

#include "embedded_shaders/vs_basic.bin.h"
#include "embedded_shaders/fs_basic.bin.h"
#include "embedded_shaders/vs_model.bin.h"
#include "embedded_shaders/fs_model.bin.h"
#include "embedded_shaders/vs_sprite.bin.h"
#include "embedded_shaders/fs_sprite.bin.h"
#ifdef _WIN32
#include "embedded_shaders/vs_basic.dx.bin.h"
#include "embedded_shaders/fs_basic.dx.bin.h"
#include "embedded_shaders/vs_model.dx.bin.h"
#include "embedded_shaders/fs_model.dx.bin.h"
#include "embedded_shaders/vs_sprite.dx.bin.h"
#include "embedded_shaders/fs_sprite.dx.bin.h"
#endif // _WIN32

static const bgfx::EmbeddedShader s_embeddedShaders[] =
{
	BGFX_EMBEDDED_SHADER(vs_basic),
	BGFX_EMBEDDED_SHADER(fs_basic),
	BGFX_EMBEDDED_SHADER(vs_model),
	BGFX_EMBEDDED_SHADER(fs_model),
	BGFX_EMBEDDED_SHADER(vs_sprite),
	BGFX_EMBEDDED_SHADER(fs_sprite),

	BGFX_EMBEDDED_SHADER_END()
};

static const cherrysoda::STL::Vector<cherrysoda::String> s_embeddedShaderNameList =
{
	"basic",
	"model",
	"sprite"
};

#endif // _CHERRYSODA_EMBEDDED_SHADERS_H_
