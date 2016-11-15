#version 150

uniform mat4 modelViewProjectionMatrix;

uniform sampler2DRect tex0;

in vec4 position;
in vec2 texcoord;
in vec3 normal;

out vec2 vTexCoord;

void main() {
    vTexCoord = texcoord;
    // vertex displacement based on the color
    vec4 color = texture(tex0, vTexCoord);
    float displacement = + 50. * (color.g + color.b);

    // move the position along the normal and transform it
    vec3 newPosition = position.xyz + normal * displacement;
    //gl_Position = modelViewProjectionMatrix * vec4( newPosition, 1.0 );
    gl_Position = modelViewProjectionMatrix * position;
}