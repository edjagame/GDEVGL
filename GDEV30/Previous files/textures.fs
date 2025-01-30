/******************************************************************************
 * This is a really simple fragment shader that simply sets the output fragment
 * color to yellow.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

in vec3 shaderColor;
in vec2 shaderTexCoord;
uniform sampler2D shaderTextureA, shaderTextureB;
out vec4 fragmentColor;
uniform float wave;

void main()
{
    //vec4 colorA = texture(shaderTextureA, vec2(-shaderTexCoord.s, shaderTexCoord.t));
    vec4 colorA = texture(shaderTextureA, vec2(-shaderTexCoord.s * wave, shaderTexCoord.t));
    vec4 colorB = texture(shaderTextureB, vec2(-shaderTexCoord.s, shaderTexCoord.t));

    fragmentColor = mix(colorA, colorB, 0.5f);
        
    
}
