#version 330 core
out vec4 FragColor;

in vec3 vertPos;
in vec2 vertTexCoord;
in vec3 vertNormal;

uniform sampler2D Texture;

uniform vec3 camPos;

uniform vec3 lightPos1;
uniform vec3 lightPos2;
uniform vec3 lightPos3;
uniform vec3 tintColor1;
uniform vec3 tintColor2;
uniform vec3 tintColor3;

uniform float diffusePower;
uniform float specularPower;

uniform float ambient;

uniform float specLight;
uniform int specAmountPow;

uniform vec3 matAmbient;
uniform vec3 matDiffuse;
uniform vec3 matSpecular;

vec4 calcColorForLight(vec3 lightPos, vec3 tintColor) {
    // Light direction + lightDist
    vec3 lightDir = normalize(lightPos - vertPos);
    float lightDist = length(lightDir);

    // PHONG

    // Ambient component - extracted to input
    // float ambient = 0.2f;

    // Diffuse component
    // float diffuse = max(dot(vec3(vertTexCoord, 1.0f), lightDir), 0.0f);
    float diffuse = max(dot(vertNormal, lightDir), 0.0f);

    // Specular component
    vec3 viewDir = normalize(camPos - vertPos);
    vec3 reflectDir = reflect(-lightDir, vertNormal);
    float specIntensity = pow(max(dot(viewDir, reflectDir), 0.0f), specAmountPow);
    float specular = specIntensity * specLight;

    // Combined light and material components
    vec4 combAmbient = vec4(matAmbient * ambient, 1.0f);
    vec4 combDiffuse = vec4(matDiffuse * diffuse, 1.0f) * diffusePower / lightDist;
    vec4 combSpecular = vec4(matSpecular * specular, 1.0f) * specularPower / lightDist;

    // Color calc from texture, combined diffuse, ambient, specular
    vec4 tex = texture(Texture, vec2(vertTexCoord.x, 1.0f - vertTexCoord.y));
    return (tex * (combDiffuse + combAmbient) + tex * combSpecular) * vec4(tintColor, 1.0f);
}

void main()
{
    // Calculate FragColor for each of the three lights
    vec4 fc1 = calcColorForLight(lightPos1, tintColor1);
    vec4 fc2 = calcColorForLight(lightPos2, tintColor2);
    vec4 fc3 = calcColorForLight(lightPos3, tintColor3);

    // Add all the lights together on output
    FragColor = fc1 + fc2 + fc3;
} 