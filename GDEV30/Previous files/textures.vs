/******************************************************************************
 * This is a really simple vertex shader that simply sets the output vertex's
 * position to be the same as the input.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec2 vertexTexCoord;

out vec3 shaderColor;
out vec2 shaderTexCoord;

uniform float wave;

void main()
{
    gl_Position = vec4(vertexPosition.x, vertexPosition.y, vertexPosition.z, 1.0f);
    gl_Position.xy = gl_Position.xy;
    shaderColor = vertexColor;
    shaderTexCoord = vertexTexCoord;
}
