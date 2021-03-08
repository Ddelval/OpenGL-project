#version 330 core

layout (location = 0) in vec3       Pos;
layout (location = 1) in vec3       Normal;
layout (location = 2) in vec2       TexCoord;
layout (location = 3) in float      TexId;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform mat4 lightSpaceMatrix;
//uniform mat4 lightSpaceMatrixClose;

out vec2    OTexCoord;
out float   OTexId;
out vec3    ONormal;
out vec3    FragPos;
out vec4    FragPosLightSpace;
//out vec4    FragPosLightSpaceClose;

void main(){
    OTexCoord=TexCoord;
    OTexId=TexId;
    ONormal=normalMatrix* Normal;
    FragPos = vec3 ( model * vec4(Pos,1.0));
    FragPosLightSpace= lightSpaceMatrix* vec4(FragPos,1);
    //FragPosLightSpaceClose= lightSpaceMatrixClose* vec4(FragPos,1);

    gl_Position = projection * view * model * vec4(Pos,1.0);
}