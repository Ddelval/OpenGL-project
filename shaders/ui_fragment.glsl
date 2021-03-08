#version 330 core

in vec2 OTexCoord;
in float OTexId;


out  vec4 FragColor;

uniform sampler2D textures[15];




void main(){
    int tid=int(OTexId);


    vec4 color=vec4(0,0,0,0);
    if(tid==0) color= vec4(texture(textures[0], OTexCoord));
    else if(tid==1) color= vec4(texture(textures[1], OTexCoord));
    else if(tid==2) color= vec4(texture(textures[2], OTexCoord));
    else if(tid==3) color= vec4(texture(textures[3], OTexCoord));
    else if(tid==4) color= vec4(texture(textures[4], OTexCoord));
    else if(tid==5) color= vec4(texture(textures[5], OTexCoord));
    else if(tid==6) color= vec4(texture(textures[6], OTexCoord));
    else if(tid==7) color= vec4(texture(textures[7], OTexCoord));
    else if(tid==8) color= vec4(texture(textures[8], OTexCoord));
    else if(tid==9) color= vec4(texture(textures[9], OTexCoord));
    

    if(color.a<0.01)discard;
    FragColor = color;
} 
