#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;

uniform mat4 lightSpaceMatrix;
uniform mat4 ModelMatrix; // only thing needed from any individual object for this shader
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

void main()
{
    gl_Position = lightSpaceMatrix * ModelMatrix * vec4(aPos, 1.0);
}