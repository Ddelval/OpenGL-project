#version 330 core

layout (location = 0) in vec3       Pos;
layout (location = 1) in vec3       Normal;
layout (location = 2) in vec2       TexCoord;
layout (location = 3) in float      TexId;


out vec2    OTexCoord;
out float   OTexId;

uniform mat4 scale;
uniform vec3 offset;

void main(){
    OTexCoord=TexCoord;
    OTexId=TexId;
    gl_Position = vec4(offset,0)+scale*vec4(Pos,1.0);
}
