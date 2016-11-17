#version 150

uniform mat4 modelViewProjectionMatrix;

uniform sampler2DRect tex0;
uniform float displaceAmount;

in vec4 position;
in vec2 texcoord;
in vec3 normal;

out vec2 vTexCoord;
out vec4 vPosition;
out vec3 vNormal;

void main() {
    vTexCoord = texcoord;
    vNormal = normal;
    // vertex displacement based on the color
    vec4 color = texture(tex0, vTexCoord);
    float displacement = + displaceAmount * (color.g + color.b);

    // move the position along the normal and transform it
    vec3 newPosition = position.xyz + normal * displacement;
    vPosition = modelViewProjectionMatrix * vec4( newPosition, 1.0 );
    gl_Position = vPosition;
    //gl_Position = modelViewProjectionMatrix * position;
}