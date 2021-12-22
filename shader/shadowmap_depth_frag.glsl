#version 330 core

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

void main()
{             
    // gl_FragDepth = gl_FragCoord.z;
}