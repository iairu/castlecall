#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor; 

in vec2 TexCoords;

uniform sampler2D screenTexture;


void main()
{
    vec3 col = texture(screenTexture, TexCoords).rgb;
    FragColor = vec4(col, 1.0);

    // no change long ver.
    // vec3 col = texture(screenTexture, TexCoords).rgb;
    // FragColor = vec4(col, 1.0);

    FragColor = texture(screenTexture, TexCoords); // no change short ver.
    
    // FragColor = 1 - texture(screenTexture, TexCoords); // invert short ver.

    // grayscale long ver.
    // FragColor = texture(screenTexture, TexCoords);
    // float avg = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    // FragColor.rgb = vec3(avg);
} 