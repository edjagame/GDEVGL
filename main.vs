/******************************************************************************
 * This is a vertex shader that improves the previous demo's shader by
 * supporting transformation matrices: projection, view, and model transform.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec4 vertexColor;
layout (location = 2) in vec3 vertexNormalCoord;
layout (location = 3) in vec2 vertexTexCoord;
layout (location = 4) in vec3 vertexTangentCoord;

uniform mat4 projectionTransform;
uniform mat4 viewTransform;
uniform mat4 modelTransform;

uniform vec3 pointLightPos;
uniform vec3 spotLightPos;
uniform vec3 spotLightDir;

out vec4 shaderColor;
out vec2 shaderTexCoord;
out mat3 shaderTBN;
out vec3 shaderPointLightPosition;
out vec3 shaderSpotLightPosition;
out vec3 shaderSpotLightDirection;
out vec3 shaderPosition;

void main()
{
    // combine the model and view transforms to get the camera space transform
    mat4 modelViewTransform = viewTransform * modelTransform;
    
    shaderColor = vertexColor;

    // compute the vertex's attributes in camera space
    shaderPosition = vec3(modelViewTransform * vec4(vertexPosition, 1.0f));
    shaderTexCoord = vertexTexCoord;

    // compute the normal transform as the transpose of the inverse of the camera transform,
    // then compute a TBN matrix using this transform
    mat3 normalTransform = mat3(transpose(inverse(modelViewTransform)));
    vec3 normal = normalize(normalTransform * vertexNormalCoord);
    vec3 tangent = normalize(normalTransform * vertexTangentCoord);
    vec3 bitangent = cross(normal, tangent);
    shaderTBN = mat3(tangent, bitangent, normal);

    // also compute the light position in camera space
    // (we want all lighting calculations to be done in camera space to avoid losing precision)
    shaderPointLightPosition = vec3(viewTransform * vec4(pointLightPos, 1.0f));
    shaderSpotLightPosition = vec3(viewTransform * vec4(spotLightPos, 1.0f));
    shaderSpotLightDirection = vec3(viewTransform * vec4(spotLightDir, 0.0f));

    gl_Position = projectionTransform * vec4(shaderPosition, 1.0f);
}
