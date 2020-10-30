$input v_color0, v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_tex, 0);

void main()
{
    vec4 color = v_color0 * texture2D(s_tex, v_texcoord0);
    if (color.w < 1.0/255.0) {
        discard;
    }
    gl_FragColor = color;
}
