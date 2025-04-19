#version 330 core
layout (location = 0) in vec3 vertexPosition;

out vec3 textureCoords;

uniform mat4 projectionTransform;
uniform mat4 viewTransform;

void main()
{
    textureCoords = -vertexPosition;
    vec4 pos = projectionTransform * viewTransform * vec4(vertexPosition, 1.0);
    gl_Position = pos.xyww;
}