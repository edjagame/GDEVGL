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

in vec3 shaderColor;
in vec2 shaderTexCoord;
out vec4 fragmentColor;
uniform sampler2D keqing, effect;
uniform float time, loopEnd;

void main()
{   
    vec4 textureB = texture(effect, shaderTexCoord + time/2.0);
    vec4 textureA = texture(keqing, shaderTexCoord);
    if (time>=3.0 && time <=6.0){
        textureA = texture(keqing, mix(shaderTexCoord.xy, shaderTexCoord + (textureB.xy - 0.5) * 0.1, sin((time-3.0)*3.14/3.0)));
    }
    fragmentColor = vec4(shaderColor, 1.0f) *
                    textureA;
}
