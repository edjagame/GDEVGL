/******************************************************************************
 * This fragment shader is exactly the same as in the previous demo. ;)
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

// in vec4 shaderColor;
// in vec2 shaderTexCoord;
// uniform sampler2D shaderTexture;
// out vec4 fragmentColor;

// void main()
// {
//     fragmentColor = shaderColor * texture(shaderTexture, shaderTexCoord);
// }


in vec4 shaderColor;
in vec2 shaderTexCoord;
uniform sampler2D tailsTexture;
uniform sampler2D chessTexture;
uniform sampler2D sonicTexture;
uniform sampler2D knucklesTexture;
out vec4 fragmentColor;

// Uniform to determine which texture to use
uniform int useTexture; // 0 for tailsTexture, 1 for sonicTexture, 2 for chessTexture, 3 for knucklesTexture

void main()
{
    if (useTexture == 1)
        fragmentColor = texture(sonicTexture, shaderTexCoord);
    else if (useTexture == 2)
        fragmentColor = texture(chessTexture, shaderTexCoord);
    else if (useTexture == 3)
        fragmentColor = texture(knucklesTexture, shaderTexCoord);
    else
        fragmentColor = texture(tailsTexture, shaderTexCoord);
}
