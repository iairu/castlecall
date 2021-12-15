#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float bloom;
uniform float exposure;
uniform float gamma;

void main()
{             
    // BLEND SHADER
    vec3 hdrColor = texture(scene, TexCoords).rgb;      
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    if(bloom > 0.5) {
        // hdrColor = hdrColor * 0.5;
        // hdrColor += bloomColor; // additive blending
        hdrColor += bloomColor; // additive blending
    }
    // tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // also gamma correct while we're at it       
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);

}

// void main()
// {             
//     // BRIGHT SHADER BUILTIN
//     vec3 col = texture(scene, TexCoords).rgb;
//     FragColor = vec4(col, 1.0);
//     // check whether fragment output is higher than threshold, if so output as brightness color
//     float brightness = dot(FragColor.rgb, vec3(1.0,1.0,1.0));
//     // float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
//     if(brightness > 1.0) {
//         // BLEND SHADER
//         const float gamma = 2.2;
//         vec3 hdrColor = texture(scene, TexCoords).rgb;      
//         vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
//         if(bloom > 0.5)
//             hdrColor += bloomColor; // additive blending
//         // tone mapping
//         vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
//         // also gamma correct while we're at it       
//         result = pow(result, vec3(1.0 / gamma));
//         FragColor = vec4(result, 1.0);
//     }
//     // else
//     //     FragColor = vec4(0.0, 0.0, 0.0, 1.0);

// }