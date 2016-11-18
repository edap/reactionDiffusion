#version 150

uniform mat4 modelViewProjectionMatrix;

//used in the normal map
float xOffset = 1.0;
float yOffset = 1.0;

uniform sampler2DRect tex0;
uniform float displaceAmount;

in vec4 position;
in vec2 texcoord;
in vec3 normal;

out vec2 vTexCoord;
out vec4 vPosition;
out vec3 vNormal;

vec3 vFromNormalMap(){
    float sCoord		= texcoord.s;
    float tCoord		= texcoord.t;

    float center		= texture(tex0, vec2( sCoord, tCoord ) ).r;
    float topLeft	= texture(tex0, vec2(sCoord - xOffset	, tCoord - yOffset	) ).r;
    float left		= texture(tex0, vec2(sCoord - xOffset	, tCoord			) ).r;
    float bottomLeft	= texture(tex0, vec2(sCoord - xOffset	, tCoord + yOffset	) ).r;
    float top		= texture(tex0, vec2(sCoord			, tCoord - yOffset	) ).r;
    float bottom		= texture(tex0, vec2(sCoord			, tCoord + yOffset	) ).r;
    float topRight	= texture(tex0, vec2(sCoord + xOffset	, tCoord - yOffset	) ).r;
    float right		= texture(tex0, vec2(sCoord + xOffset	, tCoord			) ).r;
    float bottomRight= texture(tex0, vec2(sCoord + xOffset	, tCoord + yOffset	) ).r;

    float dX = topRight + 2.0 * right + bottomRight - topLeft - 2.0 * left - bottomLeft;
    float dY = bottomLeft + 2.0 * bottom + bottomRight - topLeft - 2.0 * top - topRight;

    vec3 N = normalize( vec3( dX, dY, 0.01) );

    // this is needed to transform a vector that goes from 0 to 1 to a vector that
    // goes from -1 to 1. Normals vector, goes from -1 to 1, not from 0 to 1
    // It is like to say
    // vec3 N = N * 0.5 + 0.5; // transforms from [-1,1] to [0,1]
    N *= 0.5;
    N += 0.5;
    return N;
}

void main() {
    vTexCoord = texcoord;
    vNormal = vFromNormalMap();
    // vertex displacement based on the color
    vec4 color = texture(tex0, vTexCoord);
    float displacement = + displaceAmount * (color.g + color.b);

    // move the position along the normal and transform it
    vec3 newPosition = position.xyz + normal * displacement;
    vPosition = modelViewProjectionMatrix * vec4( newPosition, 1.0 );
    gl_Position = vPosition;
    //gl_Position = modelViewProjectionMatrix * position;
}