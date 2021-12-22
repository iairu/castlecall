#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor; 

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    vec3 col = texture(screenTexture, TexCoords).rgb;
    FragColor = vec4(col, 1.0);

    // check whether fragment output is higher than threshold, if so output as brightness color
    // float brightness = dot(FragColor.rgb, vec3(1.0,1.0,1.0));
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    float mult = 2;
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.r * mult, FragColor.g * mult, FragColor.b * mult, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

    FragColor = BrightColor;
} 