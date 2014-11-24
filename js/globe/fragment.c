uniform sampler2D s_day;
uniform sampler2D s_night;
uniform sampler2D s_normal;
uniform sampler2D s_specular;

struct light {
    float brightness;
    vec3 position;  // In world coords.
};

//#define N_LIGHTS 2
//uniform light lights[N_LIGHTS];
uniform float light_intensity;

varying vec2 vUv;
varying vec3 vNormal;
varying vec3 vTangent;
varying vec3 vBinormal;
varying vec3 vSurfaceToEye;

//varying vec3 vLightPositions[N_LIGHTS];
varying vec3 vLightPosition;

void main() {
    vec4 tColor = texture2D(s_day, vUv);
    vec4 tColor2 = texture2D(s_night, vUv);
    vec3 normal_map = (texture2D(s_normal, vUv).xyz * 2.0) - 1.0;
    normal_map.xy *= 0.05;
    normal_map = normalize(normal_map);
    float kSpecular = texture2D(s_specular, vUv).x * 0.5 + 0.0;

    mat3 tbn = mat3(vTangent, vBinormal, vNormal);

    vec3 normal = normalize(tbn * normal_map.xyz);
    vec3 surfaceToLightDir = normalize(vec3(2.0, 0.3, 1.0));

    float nDotL = dot(normal, surfaceToLightDir);
    // R = the reflection of surfaceToLightDir (L) about the normal
    vec3 R = (2.0 * normal * nDotL) - surfaceToLightDir;
    float rDotV = dot(R, normalize(vSurfaceToEye));
    vec3 specular = vec3(kSpecular * pow(max(0.0, rDotV), 20.0)) * 0.5;
    float night = (1.0 - smoothstep(-0.3, 0.2, nDotL)) * 0.1;
    float day = max(0.0, nDotL);

    vec3 color = (tColor2.rgb * night) + (tColor.rgb * day) + specular;
    //color *= 0.0;

    // light_position: (0, 0, 1.1) -> vLightPosition: (0, -0.0001, -1.5)
    // vSurfaceToEye: (-.9:.9, -.9:.9, 1.6:2.2)
    //  center: (0, 0, 1.6)
    // => surfaceToLightDir = (0, 0, 0.1)
    vec3 surfaceToLight = vSurfaceToEye + vLightPosition;
    float dist = length(surfaceToLight);
    // TODO: Limit range?
    //float intensity = 20.0 * min(0.02 / dist, 1.0);
    float intensity = light_intensity * 0.8 / dist;
    surfaceToLightDir = normalize(surfaceToLight);
    nDotL = dot(normal, surfaceToLightDir);
    color += tColor.rgb * vec3(0.9, 0.9, 1.0) * (max(nDotL, 0.0) * intensity);

    gl_FragColor = vec4(color * 1.5, 1.0);
}
