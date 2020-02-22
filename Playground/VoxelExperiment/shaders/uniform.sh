// camera position
uniform vec4 u_camPos;

// material parameters
uniform vec4 u_material[2];
#define u_albedo    u_material[0].xyz
#define u_metallic  u_material[1].x
#define u_roughness u_material[1].y
#define u_ao        u_material[1].z

// lights
uniform vec4 u_lights[8];
#define u_lightPositions(i) u_lights[i*2].xyz
#define u_lightColors(i)    u_lights[i*2+1].xyz