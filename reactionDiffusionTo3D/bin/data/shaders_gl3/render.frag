#version 150

// This fill the billboard made on the Geometry Shader with a texture

uniform sampler2DRect tex0;

in vec2 vTexCoord;
out vec4 vFragColor;

void main() {
    vec4 color = texture(tex0, vTexCoord);
    if(color.r > 0.9) {
        discard;
    }else{
        vFragColor = texture(tex0, vTexCoord);
    }
}