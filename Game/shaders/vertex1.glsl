#version 330 core

layout (location = 0) in vec3       Pos;
layout (location = 1) in vec2       TexCoord;
layout (location = 2) in float      TexId;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2    OTexCoord;
out float   OTexId;

void main(){
    OTexCoord=TexCoord;
    OTexId=TexId;
    gl_Position = projection * view * model * vec4(Pos,1.0);
}