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
in vec3 objectColor;
in vec2 shaderTexCoord;
flat in int shaderTextureID;

uniform sampler2D marbleTexture, glassTexture, noiseTexture;
uniform float time;
uniform vec3 lightPos, lightColor;

out vec4 fragmentColor;

void main()
{
    vec4 noise = texture(noiseTexture, shaderTexCoord + time/2.0);
    vec4 marble = texture(marbleTexture, shaderTexCoord);
    vec4 glass = texture(glassTexture, mix(shaderTexCoord.xy, shaderTexCoord + (noise.xy - 0.5) * 0.5, time));

    vec3 lightVector = normalize(lightPos - worldSpacePosition);
    vec3 ambientLight = vec3(0.3f);
    vec3 diffuseLight = max(dot(normalize(worldSpaceNormal),lightVector), 0.0f) * lightColor;
    vec3 lighting = ambientLight + diffuseLight;
    
    vec4 color = vec4(lighting * objectColor, 1.0f);

    if (shaderTextureID == 0) color *= marble;
    else color *= glass;

    fragmentColor = color;
}