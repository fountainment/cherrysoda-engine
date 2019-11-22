$input a_position, a_normal, a_color0
$output v_wpos, v_normal, v_color0 

#include <bgfx_shader.sh>

void main()
{
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
    v_wpos = mul(u_modelView, vec4(a_position, 1.0)).xyz;
    v_normal = normalize(mul(u_modelView, vec4(a_normal.xyz, 0.0)).xyz);
    v_color0 = a_color0;
}