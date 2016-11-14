#version 150

in vec4 normal;
in vec4 position;

out vec4 vPosition;
out vec3 vNormal;

uniform mat4 modelViewProjectionMatrix;

void main (){
    vNormal = normal.xyz;
    vPosition = modelViewProjectionMatrix * position;
    gl_Position = vPosition;
}