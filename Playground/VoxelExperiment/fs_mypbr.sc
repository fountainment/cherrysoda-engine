$input v_worldPos, v_color0, v_normal

#include <bgfx_shader.sh>

#include "uniform.sh"

SAMPLERCUBE(s_texCube, 0);
SAMPLERCUBE(s_texCubeIrr, 1);

void main()
{
    const float gamma = 2.2;
    const float gammaInv = 1.0 / 2.2;
    const float Pi = 3.14159265359;

    const float lightRadius = 50.0;
    const float lightIntensity = 0.5;

    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
    vec3 V = normalize(u_camPos.xyz - v_worldPos.xyz); 
    vec3 N = normalize(v_normal.xyz);
    float specular = 1.0 - u_roughness;
    float m = specular * 500.0;
    vec3 RF0 = pow(u_albedo, vec3(gamma, gamma, gamma));
    vec3 RF90 = vec3(1.0, 1.0, 1.0);
    float CosThetaI = max(dot(V, N), 0.0);
    vec3 RF = RF0 + (RF90 - RF0) * pow((1.0 - CosThetaI), 5.0); 
    vec3 R = reflect(-V, N);
    vec3 diffTex = pow(textureCube(s_texCubeIrr, N).xyz, vec3(gamma, gamma, gamma)); 
    vec3 specTex = pow(textureCube(s_texCube, R).xyz, vec3(gamma, gamma, gamma)); 
    vec3 Cdiff = diffTex * (1.0 - u_metallic) / Pi;
    vec3 Cspec = specTex * u_metallic;
    float EL = 2.0;
    color.xyz += RF * Cspec * EL;
    color.xyz += (1.0 - RF) * Cdiff * (EL - u_ao);
    color.xyz += v_color0.xyz * u_ao;
    // for (int i = 0; i < 4; ++i) {
    //     vec3 El = u_lightColors(i) * lightIntensity;

    //     vec3 L = u_lightPositions(i) - v_worldPos;
    //     float len = length(L);
    //     L = L / len;
    //     vec3 H = normalize(L + V);

    //     float cosThetai = max(dot(N, L), 0.0);
    //     float cosThetah = 0.0;
    //     if (cosThetai != 0.0) {
    //         cosThetah = max(dot(N, H), 0.0);
    //     }
    //     vec3 RF = RF0 + (vec3(1.0, 1.0, 1.0) - RF0) * pow((1.0 - max(dot(N, V), 0.0)), 5.0);
    //     float attenuation = 1.0 / pow((len / lightRadius) + 1.0, 2.0);
    //     vec3 L0 = (1.0 / Pi * v_color0.xyz * (vec3(1.0, 1.0, 1.0) - RF) * (1.0 - u_metallic) + (u_metallic + specular) * vec3(0.5, 0.5, 0.5) * RF * (m + 8.0) / (8.0 * Pi) * pow(cosThetah, m)) * (El * cosThetai * attenuation);

    //     color.xyz += L0;
    // }
    color.xyz = pow(color.xyz, vec3(gammaInv, gammaInv, gammaInv));
    gl_FragColor = color;
}