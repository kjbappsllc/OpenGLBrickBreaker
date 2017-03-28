#version 330 core
in vec2 TexCoords;

uniform int subTexture;
uniform sampler2D image;

out vec4 fragmentColor;

void main() {
    int i = subTexture % 6;
    int j  = subTexture / 6;
    fragmentColor = texture(image, (vec2(i, j) + TexCoords) / 6.0);
}
