#version 150

uniform sampler2DRect tex0;
uniform float discardRed;
uniform vec3 lightPos;

in vec2 vTexCoord;
in vec4 vPosition;
in vec3 vNormal;

out vec4 vFragColor;

void main() {
    vec4 texColor = texture(tex0, vTexCoord);
    if(texColor.r > discardRed) {
        discard;
    }else{
        vec3 lightDirection = normalize(lightPos - vPosition.xyz);
        float dProd = max(0.3, dot(vNormal, lightDirection));
        vec4 colorWithLight = vec4( vec3( dProd ) * vec3( texColor ), 1.0 );
        vFragColor = colorWithLight;
    }
}