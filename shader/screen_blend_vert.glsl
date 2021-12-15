#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    // no changes needed here, most are done in the fragment shader
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0); 
}  