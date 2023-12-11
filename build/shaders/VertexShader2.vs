#version 330 core

layout (location = 0) in vec3 VertexPosV;
layout (location = 1) in vec3 NormalV;
layout (location = 2) in vec3 TexturePosV;

uniform mat4 ProjectionViewModelMatrix;

void main()
{
    gl_Position = ProjectionViewModelMatrix * vec4(VertexPosV, 1.0);
}