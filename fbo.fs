/******************************************************************************
 * This fragment shader inverts the colors of the framebuffer.
 *
 * Modify this file to experiment with different post-processing effects!
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

in vec2 shaderTexCoord;
uniform sampler2D colorTexture, velocityTexture;
uniform vec2 screenSize; 
out vec4 fragmentColor;

void main()
{
    float offset[3] = { -1.0f, 0.0f, 1.0f };
    float kernel[9] =
    {
        -1.0f, -1.0f, -1.0f,
        -1.0f, 8.0f, -1.0f,
        -1.0f, -1.0f, -1.0f
    };
    vec4 edgeColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    for (int j = 0; j < 3; j++)
    {
        for (int i = 0; i < 3; i++)
        {
        vec2 offsetVec = vec2(offset[i], offset[j]) / screenSize;
        vec3 color = vec3(texture(colorTexture, shaderTexCoord + offsetVec));
        edgeColor += vec4(color * kernel[j * 3 + i], 0.0f);
        }
    }


    int numSamples = 8;
    vec2 velocity = (texture(velocityTexture, shaderTexCoord).xy - 0.5) * 2.0;
    vec4 color = texture(colorTexture, shaderTexCoord);// + edgeColor;

    for (int i=1; i < numSamples; i++) {
        vec2 offset = - vec2(float(i) / float(numSamples), 0.0) * velocity ;
        vec4 currentColor = texture(colorTexture, shaderTexCoord + offset);
        color += currentColor;
    }
    color /= float(numSamples);
    fragmentColor = vec4(color.rgb, 1.0); 
}
