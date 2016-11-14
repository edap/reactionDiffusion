#version 150
#define KERNEL_SIZE 9

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

float kernel[KERNEL_SIZE];
vec2 offset[KERNEL_SIZE];

uniform sampler2DRect tex0; // <-- ping pong source texture
uniform sampler2DRect prevTexture; // U := r, V := g, other channels ignored
uniform float ru;          // rate of diffusion of U
uniform float rv;          // rate of diffusion of V
uniform float f;           // some coupling parameter
uniform float k;           // another coupling parameter

out vec4 fragColor;

// these are set in the OF app
//uniform vec3 uLightPosition;
uniform vec4 uMaterialColor;
uniform vec3 uLightPosition;


vec4 scott(void)
{
    kernel[0] = 0.707106781;
    kernel[1] = 1.0;
    kernel[2] = 0.707106781;
    kernel[3] = 1.0;
    kernel[4] =-6.82842712;
    kernel[5] = 1.0;
    kernel[6] = 0.707106781;
    kernel[7] = 1.0;
    kernel[8] = 0.707106781;

    offset[0] = vec2( -1.0, -1.0);
    offset[1] = vec2(  0.0, -1.0);
    offset[2] = vec2(  1.0, -1.0);

    offset[3] = vec2( -1.0, 0.0);
    offset[4] = vec2(  0.0, 0.0);
    offset[5] = vec2(  1.0, 0.0);

    offset[6] = vec2( -1.0, 1.0);
    offset[7] = vec2(  0.0, 1.0);
    offset[8] = vec2(  1.0, 1.0);

    vec2 texColor      = texture(prevTexture, vTexCoord).rb;
    float srcTexColor  = texture(tex0, vTexCoord ).r;

    vec2 laplace        = vec2( 0.0, 0.0 );

    for( int i=0; i<KERNEL_SIZE; i++ ){
        vec2 tmp    = texture( prevTexture, vTexCoord + offset[i] ).rb;
        laplace     += tmp * kernel[i];
    }

    float F     = f + srcTexColor * 0.025 - 0.0005;
    float K     = k + srcTexColor * 0.025 - 0.0005;

    float u     = texColor.r;
    float v     = texColor.g  + srcTexColor * 0.5;
    float uvv   = u * v * v;
    float du    = ru * laplace.r - uvv + F * (1.0 - u);
    float dv    = rv * laplace.g + uvv - (F + K) * v;
    u += du*0.6;
    v += dv*0.6;

    return vec4( clamp( u, 0.0, 1.0 ), 1.0 - u/v, clamp( v, 0.0, 1.0 ), 1.0 );
}

void main () {
    //basic lambertian lighting
    vec3 lightDirection = normalize(vPosition.xyz - uLightPosition);
    float dProd = max(0.3, dot(vNormal, lightDirection));
    vec4 col = vec4( vec3( dProd ) * vec3( uMaterialColor ), 1.0 );

    //fragColor = vec4(vec3(col.rgb), 1.0);
    fragColor = mix(col, scott(), 0.9);
}

