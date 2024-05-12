/******************************************************************************
 * This is a fragment shader that simply takes a color input (coming from the
 * vertex shader) and makes it the final color of the fragment.
 *
 * Note that the color coming from the vertex shader is an interpolated value
 * coming from three vertices that make up a given triangle, which explains
 * why a gradient is created in the final image.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core
in vec3 worldSpaceNormal, worldSpacePosition;
in vec4 objectColor;
in vec2 shaderTexCoord;

uniform sampler2D tileTexture;
uniform float time;
uniform vec3 lightPos, lightColor, eyePosition;

out vec4 fragmentColor;

void main()
{
    vec4 tiles = texture(tileTexture, shaderTexCoord);

    vec3 surfaceToLight = normalize(lightPos - worldSpacePosition);
    vec3 ambientLight = vec3(0.2f);
    vec3 diffuseLight = max(dot(normalize(worldSpaceNormal),surfaceToLight), 0.0f) * lightColor;
    
    float specularStrength = 0.25;
    vec3 surfaceToEye = normalize(worldSpacePosition - eyePosition);
    vec3 reflectDir = surfaceToLight - 2 * dot(worldSpaceNormal, surfaceToLight) * worldSpaceNormal;
    float spec = pow(max(dot(surfaceToEye, reflectDir), 0.0), 16);
    vec3 specularLight = specularStrength * spec * lightColor;

    vec3 lighting = ambientLight + diffuseLight + specularLight;
    vec4 color = vec4(lighting, 1.0f) * objectColor;


    fragmentColor = color * tiles;
}