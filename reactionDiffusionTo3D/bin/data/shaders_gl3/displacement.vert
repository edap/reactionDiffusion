#version 150

uniform mat4 modelViewProjectionMatrix;

uniform sampler2DRect tex0;

in vec4 position;
in vec2 texcoord;

out vec2 vTexCoord;

void main() {
    vTexCoord = texcoord;
    gl_Position = modelViewProjectionMatrix * position;
}