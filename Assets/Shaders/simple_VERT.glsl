#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

uniform mat4 MVP;

out vec2 vertTexCoord;

void main()
{
    gl_Position = MVP * vec4(aPos, 1.0);
    vertTexCoord = aTexCoord;
}