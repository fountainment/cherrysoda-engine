$input v_dir

#include <bgfx_shader.sh>

SAMPLERCUBE(s_texCube, 0);

void main()
{
	vec3 dir = normalize(v_dir);
	gl_FragColor = vec4(textureCube(s_texCube, dir).xyz, 1.0);
}
