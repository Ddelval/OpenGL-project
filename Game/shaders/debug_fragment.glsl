#version 330 core
out vec4 FragColor;
  
in vec2 tex;

uniform sampler2D ourTexture;

void main()
{
    float val = texture(ourTexture, tex).r;
    FragColor = vec4(vec3(val),1);
}