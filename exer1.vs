/******************************************************************************
 * This is a vertex shader that assumes that each vertex has a position and
 * color. The color of the vertex is further manipulated via a uniform, and
 * this color is passed to the fragment shader as an output.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;


uniform float glow, scale, scale2, moveX, moveY;
uniform vec3 colorChange;
uniform mat4 rotation;
out vec3 shaderColor;

void main()
{
    gl_Position = vec4(vertexPosition, 1.0f);
    shaderColor = vertexColor;

    if(gl_VertexID >= 0 && gl_VertexID <= 12){
        gl_Position.xy = gl_Position.xy * scale2;
        shaderColor.rgb = vec3(shaderColor.r * glow * 0.5 + 0.2, shaderColor.g * glow * 0.8 + 0.2, shaderColor.b * (glow/2.0 + 0.5));

        if (gl_VertexID == 0){
            shaderColor.rgb += 0.2;
        }
    }

    if(gl_VertexID >= 13 && gl_VertexID <= 100){

        shaderColor.r -= (gl_VertexID%3) * 0.1;

        gl_Position = rotation * gl_Position;
        gl_Position.xy = gl_Position.xy * scale;
    }

    if (gl_VertexID >= 101 && gl_VertexID <= 112) {
        shaderColor.rgb += glow * 0.5;
    }

    if (gl_VertexID >= 113 && gl_VertexID <= 124) {
        shaderColor.rgb += glow * 0.25;
    }

    if (gl_VertexID == 130 || gl_VertexID == 131 ||gl_VertexID == 133 ||gl_VertexID == 134) {
        shaderColor.rgb += glow * 0.25;
    }

    if(gl_VertexID >= 144 && gl_VertexID <= 163){

        shaderColor.rgb += 0.25 * (gl_VertexID%3);
        
        shaderColor.r = shaderColor.r * (colorChange.x/2 + 0.5);
        shaderColor.g = shaderColor.g * (colorChange.y/2 + 0.5);
        shaderColor.b = shaderColor.b * (colorChange.z/2 + 0.5);
    }
    
    gl_Position.x = gl_Position.x + moveX;
    gl_Position.y = gl_Position.y + moveY;
    gl_Position.xy = gl_Position.xy * 0.9;
}
