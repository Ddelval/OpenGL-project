#version 330 core

in vec2 OTexCoord;
in float  OTexId;
in vec3 FragPos;
in vec3 ONormal;
in vec4 FragPosLightSpace;
//in vec4 FragPosLightSpaceClose;

out  vec4 FragColor;

struct DirLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};
uniform sampler2D textures[15];
uniform DirLight dirLight;
uniform vec3 viewPos;
uniform float shininess;
uniform sampler2D shadowSampler;


float ShadowCalculation (vec4 FragPosLightSpace){
    vec3 projCoords= FragPosLightSpace.xyz/FragPosLightSpace.w;
    projCoords=projCoords*0.5+0.5;
    
    float currentDepth= projCoords.z;

    float shadow = 0.0;
    float bias = max(0.005 * (1.0 - dot(normalize(ONormal),normalize(-dirLight.direction))), 0.001);  
    vec2 texelSize = 1.0 / textureSize(shadowSampler, 0);

    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowSampler, projCoords.xy + vec2(x, y) * 2*texelSize).r; 
            shadow += (currentDepth - bias > pcfDepth ? 1.0 : 0.0);        
        }    
    }
    shadow /= 9.0;


    return shadow;
}

void main(){
    int tid=int(OTexId);

    vec3 c_ambient=vec3(0);
    vec3 c_diffuse=vec3(0);
    vec3 c_specular=vec3(0);

    vec3 norm = normalize(ONormal);
    vec3 viewDir = normalize(viewPos - FragPos);

    //// Directional light

    c_ambient+= dirLight.ambient;

    vec3 lightDir = normalize(-dirLight.direction);
    c_diffuse+=max(dot(norm,lightDir),0.0)* dirLight.diffuse;

    vec3 reflectDir = reflect(-lightDir,norm);
    c_specular += pow(max(dot(viewDir,reflectDir),0),shininess)* dirLight.specular;
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
    
    
    vec3 coeff = (1-ShadowCalculation(FragPosLightSpace))*(c_diffuse+c_specular)+c_ambient;
    //vec3 coeff = (1-ShadowCalculation(FragPosLightSpace))*vec3(1,1,1);
    vec4 inter=vec4(coeff*vec3(color),color.a);
    if(inter.a<0.5)discard;
    FragColor = inter;
} 