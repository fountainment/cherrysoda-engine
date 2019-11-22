$input v_wpos, v_color0, v_normal

#include <bgfx_shader.sh>

vec3 PointLight(vec3 texColor, vec3 wpos, vec3 normal, vec3 lightPos, float lightRadius, vec3 lightColor, float lightIntensity)
{
    const vec3 gold = vec3(1.0, 0.72, 0.29);
    const vec3 silver = vec3(0.95, 0.93, 0.88);
    const vec3 copper = vec3(0.95, 0.63, 0.54);
    const vec3 iron = vec3(0.56, 0.57, 0.58);
    const vec3 al = vec3(0.56, 0.57, 0.58);
    const vec3 porcelain = vec3(0.0277, 0.0277, 0.0277);
    const vec3 plastic = vec3(0.0505, 0.0505, 0.0505);
    const float Pi = 3.14159265358979323846264;
    float m = 10.0;
    float gamma = 2.2;
    float gammaInv = 1.0 / 2.2;
    float specular = 1.0;
    float metallic = 0.9;
    vec3 RF0 = pow(silver, vec3(gamma, gamma, gamma));
    if (lightIntensity == 0.0) {
        return vec3(0.0, 0.0, 0.0);
    }
    vec3 pl = mul(u_view, vec4(lightPos, 1.0)).xyz;
    vec3 El = lightColor * lightIntensity;
    vec3 p = wpos;

    vec3 n = normal;
    vec3 l = pl - p;
    float len = length(l);
    l = l / len;
    vec3 v = -normalize(wpos);
    vec3 h = normalize(l + v);

    float cosThetai = max(dot(normalize(n), l), 0.0);
    float cosThetah = 0.0;
    if (cosThetai != 0.0) {
        cosThetah = max(dot(n, h), 0.0);
    }
    vec3 RF = RF0 + (vec3(1.0, 1.0, 1.0) - RF0) * pow((1.0 - max(dot(normalize(n), v), 0.0)), 5.0);
    float attenuation = 1.0 / pow((len / lightRadius) + 1.0, 2.0);
    vec3 L0 = (vec3(1.0, 1.0, 1.0) / Pi * texColor * (vec3(1.0, 1.0, 1.0) - RF) * (1.0 - metallic) + (metallic + specular) * vec3(0.5, 0.5, 0.5) * RF * (m + 8.0) / (8.0 * Pi) * pow(cosThetah, m)) * (El * cosThetai * attenuation);
    L0 += RF0 * 0.1;
    return L0;
}


void main()
{
    float gamma = 2.2;
    float gammaInv = 1.0 / 2.2;
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
    color.xyz += PointLight(vec3(1.0, 1.0, 1.0), v_wpos, normalize(v_normal), vec3(0.0, 0.0, 20.0), 50.0, vec3(1.0, 1.0, 1.0), 1.0);
    color.xyz = pow(color.xyz, vec3(gammaInv, gammaInv, gammaInv));
    gl_FragColor = color;
}