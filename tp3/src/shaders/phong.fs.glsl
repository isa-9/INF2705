#version 330 core

in ATTRIB_VS_OUT
{
    vec2 texCoords;
    vec3 normal;
    vec3 lightDir[3];
    vec3 spotDir[3];
    vec3 obsPos;
} attribIn;

struct Material
{
    vec3 emission;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct UniversalLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    vec3 spotDirection;
};

layout (std140) uniform LightingBlock
{
    Material mat;
    UniversalLight lights[3];
    vec3 lightModelAmbient;
    bool useBlinn;
    bool useSpotlight;
    bool useDirect3D;
    float spotExponent;
    float spotOpeningAngle;
} lightingBlock;

uniform sampler2D diffuseSampler;
uniform sampler2D specularSampler;

out vec4 FragColor;


float calculerSpot( in vec3 D, in vec3 L, in vec3 N )
{
    float spotFacteur = 0.0;
    if ( dot( D, N ) >= 0 )
    {
        float spotDot = dot( L, D );
        float cosDelta = cos(radians(lightingBlock.spotOpeningAngle));
        if ( spotDot > cosDelta ) {
            spotFacteur = lightingBlock.useDirect3D ? 
                smoothstep( pow( cosDelta, 1.01 + lightingBlock.spotExponent / 2), cosDelta, spotDot )
            : pow( spotDot, lightingBlock.spotExponent );
        }
    }
    return( spotFacteur );
}


vec4 calculerReflexion( in vec3 L, in vec3 N, in vec3 O, in int i, in vec4 texDiffuse, in vec4 texSpecular)
{    
    vec3 coul = vec3(0);

    // calculer l'éclairage seulement si le produit scalaire est positif
    float NdotL = max( 0.0, dot( N, L ) );
    if ( NdotL > 0.0 )
    {
        // calculer la composante diffuse
        vec3 diffuse = lightingBlock.mat.diffuse * lightingBlock.lights[i].diffuse * NdotL;
        diffuse = vec3(texDiffuse * vec4(diffuse, 1.0));
        coul += diffuse;

        // calculer la composante spéculaire (Blinn ou Phong : spec = BdotN ou RdotO )
        float spec = ( lightingBlock.useBlinn ?
                       dot( normalize( L + O ), N ) :
                       dot( reflect( -L, N ), O ) );


        vec3 coulSpec = lightingBlock.mat.specular * lightingBlock.lights[i].specular * pow( spec, lightingBlock.mat.shininess );
        coulSpec = vec3(texSpecular * vec4(coulSpec, 1.0));
        if ( spec > 0 ) coul += coulSpec;
    }

    return( vec4(coul, 0.0f) );
}


vec4 calculerAmbient() {
    vec3 ambient = lightingBlock.mat.ambient * lightingBlock.lights[0].ambient;
    ambient += lightingBlock.mat.ambient * lightingBlock.lights[1].ambient;
    ambient += lightingBlock.mat.ambient * lightingBlock.lights[2].ambient;
    ambient += lightingBlock.mat.ambient * lightingBlock.lightModelAmbient;
    return vec4(ambient, 1.0);
}


void main()
{
    // TODO
    vec4 texDiffuse = texture(diffuseSampler, attribIn.texCoords);
    vec4 texSpecular = texture(specularSampler, attribIn.texCoords);

    vec3 N = normalize( gl_FrontFacing ? attribIn.normal : -attribIn.normal );

    vec3 O = normalize(attribIn.obsPos);

    vec3 D[3];
    D[0] = normalize( attribIn.spotDir[0] );
    D[1] = normalize( attribIn.spotDir[1] );
    D[2] = normalize( attribIn.spotDir[2] );

    vec4 col = vec4(lightingBlock.mat.emission, 1.0f);
    col += texDiffuse * calculerAmbient();
   
    vec3 L = normalize(attribIn.lightDir[0]);
    col += calculerReflexion( L, N, O, 0, texDiffuse, texSpecular ) * (lightingBlock.useSpotlight ? calculerSpot( D[0], L, N ) : 1 );
    
    L = normalize(attribIn.lightDir[1]);
    col += calculerReflexion( L, N, O, 1, texDiffuse, texSpecular ) * (lightingBlock.useSpotlight ? calculerSpot( D[1], L, N ) : 1 );
    
    L = normalize(attribIn.lightDir[2]);
    col += calculerReflexion( L, N, O, 2, texDiffuse, texSpecular ) * (lightingBlock.useSpotlight ? calculerSpot( D[2], L, N ) : 1 );


    FragColor = clamp( col, 0.0, 1.0 );
}
