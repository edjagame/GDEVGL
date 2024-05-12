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

uniform sampler2D texA, texB, texC;
uniform float time;
uniform vec3 lightPos, lightColor, eyePosition;

out vec4 fragmentColor;

void main()
{
    vec4 mainTexture = texture(texA, shaderTexCoord);
    vec4 displacement = texture(texC, vec2(shaderTexCoord.x - time/80, shaderTexCoord.y + time/40));
    vec4 effect1 = texture(texB, (shaderTexCoord + (displacement.xy - 0.5) * 0.1) + vec2(shaderTexCoord.x - time/40.0, shaderTexCoord.y - time/20.0));

    vec3 surfaceToLight = normalize(lightPos - worldSpacePosition);
    vec3 ambientLight = vec3(0.2f);
    vec3 diffuseLight = max(dot(normalize(worldSpaceNormal),surfaceToLight), 0.0f) * lightColor;
    
    float specularStrength = 0.1;
    vec3 surfaceToEye = normalize(worldSpacePosition - eyePosition);
    vec3 reflectDir = surfaceToLight - 2 * dot(worldSpaceNormal, surfaceToLight) * worldSpaceNormal;
    float spec = pow(max(dot(surfaceToEye, reflectDir), 0.0), 4);
    vec3 specularLight = specularStrength * spec * lightColor;

    vec3 lighting = ambientLight + diffuseLight + specularLight;
    vec4 color = vec4(lighting, 1.0f) * objectColor;


    fragmentColor = color * mainTexture * effect1;
}