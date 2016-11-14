#version 150

in vec4 normal;
in vec4 position;
in vec2 texcoord;

out vec4 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;

uniform mat4 modelViewProjectionMatrix;

void main (){
    vNormal = normal.xyz;
    vPosition = modelViewProjectionMatrix * position;
    vTexCoord = texcoord; 
    gl_Position = vPosition;
}