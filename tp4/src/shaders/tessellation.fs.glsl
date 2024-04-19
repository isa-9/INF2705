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
    float sandFactor = smoothstep(0.0, 0.3, attribIn.height);
    float grassFactor = smoothstep(0.3, 0.6, attribIn.height);
    float snowFactor = smoothstep(0.6, 1.0, attribIn.height);

    vec4 sandTex = texture(sandSampler, attribIn.texCoords);
    vec4 grassTex = texture(groundSampler, attribIn.texCoords);
    vec4 sandGrassMix = mix(sandTex, grassTex, smoothstep(0.3, 0.35, attribIn.height));

    vec4 snowTex = texture(snowSampler, attribIn.texCoords);
    vec4 grassSnowMix = mix(grassTex, snowTex, smoothstep(0.6, 0.65, attribIn.height));

    vec4 color = mix(sandTex, sandGrassMix, clamp(sandFactor - grassFactor, 0.0, 1.0));
    color = mix(color, grassTex, clamp(grassFactor, 0.0, 1.0));
    color = mix(color, grassSnowMix, clamp(grassFactor - snowFactor, 0.0, 1.0));
    color = mix(color, snowTex, clamp(snowFactor, 0.0, 1.0));
    vec3 finalColor = vec3(color);
    if(viewWireframe) {
        float wireFrameFactor = edgeFactor(attribIn.barycentricCoords, WIREFRAME_WIDTH);
        finalColor = mix(color.rgb, WIREFRAME_COLOR, 1.0 - wireFrameFactor);

        float patchEdgeFactor = edgeFactor(attribIn.patchDistance, PATCH_EDGE_WIDTH);
        finalColor = mix(finalColor, PATCH_EDGE_COLOR, 1.0 - patchEdgeFactor);
    }


    FragColor = vec4(finalColor, 1);
}
