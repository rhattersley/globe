// Implicit attributes:
//  vec3 position   // The location of the vertex in object coords
//  vec3 normal     // The normal in object frame
//  vec2 uv         // The texture coords
// Implicit uniforms:
//  mat4 projectionMatrix   // From camera to "screen"
//  mat4 modelMatrix        // Object to world
//  mat4 viewMatrix         // World to camera
//  mat4 modelViewMatrix    // Object to camera
//  mat4 projectionmatrix
//  mat3 normalMatrix       // Upper 3x3 of modelViewMatrix (obj -> camera)
//  vec3 cameraPosition

attribute vec4 tangent;

varying vec2 vUv;
varying vec3 vNormal;
varying vec3 vTangent;
varying vec3 vBinormal;
varying vec3 vSurfaceToEye;

void main() {
    vUv = uv;

    vNormal = normalize(normalMatrix * normal);
    vTangent = normalize(normalMatrix * tangent.xyz);
    vBinormal = normalize(cross(vNormal, vTangent) * tangent.w);

    vec4 mvPosition = modelViewMatrix * vec4(position, 1.0);
    vSurfaceToEye = -mvPosition.xyz;

    gl_Position = projectionMatrix * mvPosition;
}
