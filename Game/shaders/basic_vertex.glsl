#version 330 core
layout (location = 0) in vec3       Pos;
layout (location = 1) in vec3       Normal;
layout (location = 2) in vec2       TexCoord;
layout (location = 3) in float      TexId;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main(){
    gl_Position = lightSpaceMatrix * model* vec4(Pos,1.0);
}