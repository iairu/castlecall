#version 330 core
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;

out vec3 vertPos;
out vec2 vertTexCoord;
out vec3 vertNormal;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

void main()
{
    // diffuse light strikes objects with a gradient depending on the light source
    // and its angle towards the surface
    // does not relate to camera coords

    // Copy the input to the fragment shader
    vertTexCoord = TexCoord;

    // world space vertex coords
    vertPos = vec3(ModelMatrix * vec4(Pos, 1.0));

    // world space normal vector
    vertNormal = normalize(Normal);
    // Normal = normalize(mat3(ModelMatrix) * aNormal);
    // Normal = normalize(transpose(inverse(mat3(ModelMatrix))) * aNormal);

    // gl_Position = ProjectionMatrix * ViewMatrix * vec4(FragPos, 1.0);
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Pos, 1.0);
}  