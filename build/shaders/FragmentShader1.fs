#version 330 core
#define PI 3.14159265359

out vec4 Exitance;

uniform vec3 ObjectColors;
uniform vec3 LightColors;
uniform vec3 LightPos;
uniform vec3 ViewPos;
uniform vec3 LightDirection;

in vec3 NormalF;
in vec3 FragPos;

void main()
{
    vec3 Normal   = normalize(NormalF);
    vec3 ViewVec  = normalize(ViewPos - FragPos);
    
    vec3 LightVec = normalize(LightPos - FragPos);
    vec3 DiffuseLight = max(dot(LightVec, Normal), 0.0) * LightColors / PI;
    
    vec3 HalfVec   = normalize(ViewVec + LightVec);
    float cosHalf  = clamp(dot(HalfVec, Normal), 0.0, 1.0);
    float cosLight = clamp(dot(LightVec, Normal), 0.0, 1.0);
    
    vec3  SpecularLight = ((32 + 8) / (8 * PI)) * pow(cosHalf, 32) * cosLight * LightColors;
    
    Exitance = vec4((DiffuseLight + SpecularLight) * ObjectColors, 1.0);
}