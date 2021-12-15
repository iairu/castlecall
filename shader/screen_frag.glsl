#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    // no change long ver.
    // vec3 col = texture(screenTexture, TexCoords).rgb;
    // FragColor = vec4(col, 1.0);

    // FragColor = 1 - texture(screenTexture, TexCoords); // invert short ver.
    FragColor = texture(screenTexture, TexCoords); // no change short ver.

    // grayscale long ver.
    // FragColor = texture(screenTexture, TexCoords);
    // float avg = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    // FragColor.rgb = vec3(avg);
} 