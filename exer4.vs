/******************************************************************************
 * This is a vertex shader that assumes that each vertex has a position and
 * color. The color of the vertex is further manipulated via a uniform, and
 * this color is passed to the fragment shader as an output.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec3 vertexNormal;
layout (location = 3) in vec2 vertexTexCoord;
layout (location = 4) in int vertexTextureID;

out vec3 worldSpaceNormal, worldSpacePosition;
out vec3 objectColor;
out vec2 shaderTexCoord;
flat out int shaderTextureID;

uniform mat4 projView, model, normal;
uniform vec3 lightPos;

void main()
{

    worldSpacePosition = (model * vec4(vertexPosition, 1.0f)).xyz;
    worldSpaceNormal = (normal * vec4(vertexNormal, 1.0f)).xyz;
    gl_Position = projView * vec4(worldSpacePosition, 1.0f);

    objectColor = vertexColor;
    shaderTexCoord = vertexTexCoord;
    shaderTextureID = vertexTextureID;
}