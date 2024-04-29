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
flat in int shaderTexture;
uniform sampler2D marbleTexture, glassTexture, noiseTexture;
uniform float time;
out vec4 fragmentColor;

void main()
{
    vec4 color = vec4(shaderColor, 1.0f);
    vec4 noise = texture(noiseTexture, shaderTexCoord + time/2.0);

    if(shaderTexture == 0){
        color *= texture(marbleTexture, shaderTexCoord) * vec4((sin(time)+1.0)/2.0,(sin(time+6.28/3.0)+1.0)/2.0,(sin(time+12.57/3.0)+1.0)/2.0,1.0f);
    }

    else{
        color *= texture(glassTexture, mix(shaderTexCoord.xy, shaderTexCoord + (noise.xy - 0.5) * 0.5, time));
    }

    fragmentColor = color;

    // fragmentColor = vec4(shaderColor, 1.0f) *
    //                 textureA;
}