$input v_color0, v_normal

#include <bgfx_shader.sh>

void main()
{
    gl_FragColor = vec4((v_normal + 1.0) * 0.5, 1.0);
}