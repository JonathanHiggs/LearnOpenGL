#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;

uniform mat4 uModel;
uniform mat3 uNormal;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 vFragPosition;
out vec3 vNormal;

void main()
{
    vec4 modelPosition = uModel * vec4(inPosition, 1.0);

    gl_Position = uProjection * uView * modelPosition;
    vFragPosition = vec3(modelPosition);

    vNormal = uNormal * inNormal;
}