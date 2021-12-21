#version 330 core
out vec4 FragColor;

in vec3 vertPos;
in vec2 vertTexCoord;
in vec3 vertNormal;

uniform sampler2D Texture;

uniform vec3 lightPos;
uniform vec3 camPos;

uniform float attenuationA;
uniform float attenuationB;
uniform float specLight;
uniform int specAmountPow;
uniform vec3 matAmbient;
uniform vec3 matDiffuse;
uniform vec3 matSpecular;
uniform vec3 tintColor;

void main()
{
    // Light direction
    vec3 lightDir = normalize(lightPos - vertPos);

    // Light attenuation
    float distance = length(lightDir);
    float attenuation = 1.0f / (attenuationA * pow(distance, 2) + attenuationB * distance);

    // PHONG

    // Ambient component
    float ambient = 0.2f; // todo extract to input

    // Diffuse component
    // float diffuse = max(dot(vec3(vertTexCoord, 1.0f), lightDir), 0.0f);
    float diffuse = max(dot(vertNormal, lightDir), 0.0f);

    // Specular component
    vec3 viewDir = normalize(camPos - vertPos);
    vec3 reflectDir = reflect(-lightDir, vertNormal);
    float specAmount = pow(max(dot(viewDir, reflectDir), 0.0f), specAmountPow);
    float specular = specAmount * specLight;

    // Combined light and material components
    vec4 combAmbient = vec4(matAmbient * ambient, 1.0f);
    vec4 combDiffuse = vec4(matDiffuse * diffuse, 1.0f);
    vec4 combSpecular = vec4(matSpecular * specular, 1.0f);

    // Color calc from texture, attenuation, diffuse, ambient, specular
    vec4 tex = texture(Texture, vec2(vertTexCoord.x, 1.0f - vertTexCoord.y));
    FragColor = (
    tex * (combDiffuse * attenuation + combAmbient) + tex * (combSpecular * attenuation)
    ) * vec4(tintColor, 1.0f);
} 