#version 330 core

out vec4 FragColor;

in vec2 vertTexCoord;

uniform sampler2D myTexture;
  
uniform vec3 objectColor;

void main()
{
    vec4 texColor = texture(myTexture, vertTexCoord); 
    FragColor = vec4(objectColor.xyz, 1) * texColor;
}