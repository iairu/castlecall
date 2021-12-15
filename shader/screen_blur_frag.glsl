#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor; 

in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform float horizontal;

uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
    vec2 tex_offset = 1.0 / textureSize(screenTexture, 0); // gets size of single texel
    vec3 result = texture(screenTexture, TexCoords).rgb * weight[0]; // current fragment's contribution
    if (horizontal > 0.5) {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(screenTexture, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(screenTexture, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    } else {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(screenTexture, TexCoords + vec2(tex_offset.y * i, 0.0)).rgb * weight[i];
            result += texture(screenTexture, TexCoords - vec2(tex_offset.y * i, 0.0)).rgb * weight[i];
        }
    }

    FragColor = vec4(result, 1.0);
} 