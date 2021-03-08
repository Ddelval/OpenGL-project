#version 330 core

out  vec4 FragColor;

in vec2 OTexCoord;
in float  OTexId;
uniform sampler2D textures[16];

void main(){
    int tid=int(OTexId);
    FragColor = texture(textures[tid], OTexCoord);
    //FragColor= vec4(1,1,1,1);
} 