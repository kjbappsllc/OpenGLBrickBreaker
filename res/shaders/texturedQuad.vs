#version 330 core
layout (location = 0) in vec4 vertex; //Both the vec2 position and vec2 textureCoords

out vec2 TextCoords;

uniform mat4 model;
uniform mat4 projection; //the projection transforms world space coordinates into view space.

void main(){
    TextCoords = vertex.zw;
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}
