#version 330 core
out vec4 FragColor;

in vec2 theTexCoord;

uniform vec3 objectColor;
uniform sampler2D theTexture;

void main()
{
    //FragColor = vec4(objectColor.xyz, 1);
	FragColor = texture(theTexture, theTexCoord);
}