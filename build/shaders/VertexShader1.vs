#version 330 core

layout (location = 0) in vec3 VertexPosV;
layout (location = 1) in vec3 NormalV;
layout (location = 2) in vec3 TexturePosV;

uniform mat3 NormalMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ProjectionViewModelMatrix;

out vec3 NormalF;
out vec3 FragPos;

void main()
{
    gl_Position = ProjectionViewModelMatrix * vec4(VertexPosV, 1.0);
    FragPos = vec3(ModelMatrix * vec4(VertexPosV, 1.0));
    NormalF = NormalMatrix * NormalV;
}