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

// time value
uniform vec4 u_timev;
#define u_time     u_timev.x
#define u_delta    u_timev.y
#define u_rawTime  u_timev.z
#define u_rawDelta u_timev.w

// resolution value
uniform vec4 u_resolutionv;
#define u_resolution  u_resolutionv.xy
#define u_surfaceSize u_resolutionv.zw
#define u_ratio       u_resolutionv.z
