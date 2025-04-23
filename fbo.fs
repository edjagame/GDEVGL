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
    int numSamples = 16;

    vec2 velocity = (texture(velocityTexture, shaderTexCoord).xy - 0.5) * 2.0;
    vec4 color = texture(colorTexture, shaderTexCoord);

    for (int i=1; i < numSamples; i++) {
        vec2 offset = - vec2(float(i) / float(numSamples), 0.0) * velocity;
        vec4 currentColor = texture(colorTexture, shaderTexCoord + offset);
        color += currentColor;
    }
    color /= float(numSamples);
    fragmentColor = vec4(color.rgb, 1.0); 
}
