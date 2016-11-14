#version 150

in vec4 vPosition;
in vec3 vNormal;

out vec4 fragColor;

// these are set in the OF app
//uniform vec3 uLightPosition;
uniform vec4 uMaterialColor;
uniform vec3 uLightPosition;

void main () {
    //basic lambertian lighting
    vec3 lightDirection = normalize(vPosition.xyz - uLightPosition);
    float dProd = max(0.3, dot(vNormal, lightDirection));
    vec4 col = vec4( vec3( dProd ) * vec3( uMaterialColor ), 1.0 );

    //color
    fragColor = vec4(vec3(col.rgb), 1.0);
}