$input v_worldPos, v_color0, v_normal

#include <bgfx_shader.sh>

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
#define u_lightPositions(i) u_lights[i]
#define u_lightColors(i)    u_lights[i+4]

const float PI = 3.14159265358979323846264;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
    
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
    
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{       
    vec3 N = normalize(v_normal);
    vec3 V = normalize(u_camPos.xyz - v_worldPos);

    vec3 F0 = vec3(0.04, 0.04, 0.04); 
    F0 = mix(F0, u_albedo, u_metallic);
               
    // reflectance equation
    vec3 Lo = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < 4; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(u_lightPositions(i).xyz - v_worldPos);
        vec3 H = normalize(V + L);
        float distance    = length(u_lightPositions(i).xyz - v_worldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance     = u_lightColors(i).xyz * attenuation;        
        
        // cook-torrance brdf
        float NDF = DistributionGGX(N, H, u_roughness);        
        float G   = GeometrySmith(N, V, L, u_roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
        
        vec3 kS = F;
        vec3 kD = vec3(1.0, 1.0, 1.0) - kS;
        kD *= 1.0 - u_metallic;     
        
        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular     = numerator / max(denominator, 0.001);  
            
        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);                
        Lo += (kD * u_albedo / PI + specular) * radiance * NdotL; 
    }   
  
    vec3 ambient = vec3(0.03, 0.03, 0.03) * u_albedo * u_ao;
    vec3 color = ambient + Lo;
    
    color = color / (color + vec3(1.0, 1.0, 1.0));
    color = pow(color, vec3(1.0/2.2, 1.0/2.2, 1.0/2.2));  
   
    gl_FragColor = vec4(color, 1.0);
}