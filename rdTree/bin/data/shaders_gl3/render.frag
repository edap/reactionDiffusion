#version 150

uniform sampler2DRect tex0;
uniform float discardRed;
uniform vec3 lightPos;
uniform vec4 lightColor;
uniform float lightPower;
uniform vec4 materialColor;
uniform vec4 pointColor;

in vec2 vTexCoord;
in vec4 vPosition;
in vec3 vNormal;

out vec4 vFragColor;

void main() {
    vec4 texColor = texture(tex0, vTexCoord);
    if (texColor.r > discardRed) {
        discard;
    } else {
        vec4 modCol = mix(pointColor, materialColor, texColor.r);
        vec3 lightDirection = normalize(lightPos - vPosition.xyz);
        float dProd = max(0.6, dot(vNormal, lightDirection));
        vec4 colorWithLight = vec4( vec3( dProd ) * vec3( modCol ), 1.0 );
        // adding a bit of ambient light
        vec4 ambientLight = lightColor * lightPower;
        vFragColor = mix(ambientLight, colorWithLight, 0.5);
        //vFragColor = colorWithLight;
    }
}