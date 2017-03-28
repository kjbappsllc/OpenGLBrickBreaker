#version 330 core

in vec2 TextCoords;
in vec4 ParticleColor;

out vec4 color;
uniform sampler2D sprite;

void main() {
    color = (texture(sprite, TextCoords) * ParticleColor);
}
