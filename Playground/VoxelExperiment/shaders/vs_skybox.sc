$input a_position, a_texcoord0
$output v_dir

#include <bgfx_shader.sh>

uniform mat4 u_mtx;

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
	float fov = radians(60.0);
	float height = tan(fov*0.5);
	float aspect = height*(u_viewRect.z / u_viewRect.w);
	vec2 tex = (2.0*a_texcoord0-1.0) * vec2(aspect, height);
	v_dir = mul(u_mtx, vec4(tex, -1.0, 0.0)).xyz;
}
