#version 150

uniform sampler2DRect tex0;
float xOffset = 1.0;
float yOffset = 1.0;

in vec2 vTexCoord;
out vec4 fragColor;

void main(){
   float sCoord		= vTexCoord.s;
   float tCoord		= vTexCoord.t;

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

   fragColor.rgb = N;
   fragColor.a = 1.0;
}