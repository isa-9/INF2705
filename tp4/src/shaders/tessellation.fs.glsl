#version 330 core

in ATTRIB_GS_OUT
{
    float height;
    vec2 texCoords;
    vec4 patchDistance;
    vec3 barycentricCoords;
} attribIn;

uniform sampler2D groundSampler;
uniform sampler2D sandSampler;
uniform sampler2D snowSampler;
uniform bool viewWireframe;

out vec4 FragColor;

float edgeFactor(vec3 barycentricCoords, float width)
{
    vec3 d = fwidth(barycentricCoords);
    vec3 f = step(d * width, barycentricCoords);
    return min(min(f.x, f.y), f.z);
}

float edgeFactor(vec4 barycentricCoords, float width)
{
    vec4 d = fwidth(barycentricCoords);
    vec4 f = step(d * width, barycentricCoords);
    return min(min(min(f.x, f.y), f.z), f.w);
}

const vec3 WIREFRAME_COLOR = vec3(0.5f);
const vec3 PATCH_EDGE_COLOR = vec3(1.0f, 0.0f, 0.0f);

const float WIREFRAME_WIDTH = 0.5f;
const float PATCH_EDGE_WIDTH = 0.5f;

void main()
{
	// TODO
    // Calcul des facteurs de mélange pour les différentes textures
    float sandFactor = smoothstep(0.0, 0.3, attribIn.height);
    float grassFactor = smoothstep(0.3, 0.6, attribIn.height);
    float snowFactor = smoothstep(0.6, 1.0, attribIn.height);

    // Mélange entre le sable et le gazon
    vec4 sandTex = texture(sandSampler, attribIn.texCoords);
    vec4 grassTex = texture(groundSampler, attribIn.texCoords);
    vec4 sandGrassMix = mix(sandTex, grassTex, smoothstep(0.3, 0.35, attribIn.height));

    // Mélange entre le gazon et la neige
    vec4 snowTex = texture(snowSampler, attribIn.texCoords);
    vec4 grassSnowMix = mix(grassTex, snowTex, smoothstep(0.6, 0.65, attribIn.height));

    // Calcul final du mélange en utilisant les facteurs
    vec4 color = mix(sandTex, sandGrassMix, clamp(sandFactor - grassFactor, 0.0, 1.0));
    color = mix(color, grassTex, clamp(grassFactor - snowFactor, 0.0, 1.0));
    color = mix(color, grassSnowMix, clamp(snowFactor, 0.0, 1.0));

    FragColor = color;
}
