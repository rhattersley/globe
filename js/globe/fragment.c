uniform sampler2D s_day;
uniform sampler2D s_night;
uniform sampler2D s_normal;
uniform sampler2D s_specular;

varying vec2 vUv;
varying vec3 vNormal;
varying vec3 vTangent;
varying vec3 vBinormal;
varying vec3 vSurfaceToEye;

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
    gl_FragColor = vec4(color * 1.5, 1.0);
}
