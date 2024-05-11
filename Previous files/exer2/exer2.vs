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
layout (location = 2) in vec2 vertexTexCoord;
layout (location = 3) in vec2 vertexDisplaced;

out vec3 shaderColor;
out vec2 shaderTexCoord;

uniform float loopEnd, time;
uniform mat4 rotation;

void main()
{
    int ring1 = 0; 
    int ring2up = 33;
    int ring2down = 67;
    int ring3left = 101;
    int ring3right = 135;
    int topright = 169;
    int topleft = 177;
    int bottomleft = 185;
    int bottomright = 193;
    int finalvert = 208;

    vec3 white = vec3(1.0f, 1.0f, 1.0f);

    float angle = 0.0f;
    float contangle = 0.0f;

    //INITIAL COORDS
    gl_Position = vec4(vertexDisplaced.xy, vertexPosition.z, 1.0f);
    shaderColor = vertexColor;
    shaderTexCoord = vertexTexCoord;

    //SETTING THE COLORS
    if(gl_VertexID >= ring2up && gl_VertexID < ring2down){shaderColor.x = 0.5;};
    if(gl_VertexID >= ring2down && gl_VertexID < ring3left){shaderColor.y = 0.5;};
    if(gl_VertexID >= ring3left && gl_VertexID < ring3right){shaderColor.xy = vec2(0.5);}
    if(gl_VertexID >= ring3right && gl_VertexID < topright){shaderColor.yz = vec2(0.5);}
    if(gl_VertexID >= topright && gl_VertexID < topleft){shaderColor.xz = vec2(0.5);}
    if(gl_VertexID >= topleft && gl_VertexID < bottomleft){shaderColor.x = 0.5;}
    if(gl_VertexID >= bottomleft && gl_VertexID < bottomright){shaderColor.y = 0.5;}
    if(gl_VertexID >= bottomright && gl_VertexID <= finalvert){shaderColor.z = 0.5;}

    if(time<1.0){
        angle = radians((time - 0.0) * 180 / (1.0));
        if(gl_VertexID >= ring1 && gl_VertexID < ring2up){
            gl_Position.xy = mix(vertexDisplaced.xy, vertexPosition.xy, time-floor(time));
            shaderColor.rgb = mix(shaderColor.rgb, white, time-floor(time));
        }
        if(gl_VertexID >= ring2up && gl_VertexID < ring3left){
            mat4 rotationMatrix = mat4(
                cos(angle), -sin(angle),     0.0f, 0.0f,
                sin(angle),  cos(angle),     0.0f, 0.0f,
                0.0f,        0.0f,           1.0f, 0.0f,
                0.0f,        0.0f,           0.0f, 1.0f
            );
            gl_Position.xy = mix(vertexDisplaced.xy, vertexPosition.xy, time-floor(time));
            shaderColor.rgb = mix(shaderColor.rgb, white, time-floor(time));
            gl_Position = rotationMatrix * gl_Position;
        }
    }
    if (time >= 1.0 && time <= 2.0) {
        angle = radians(180.0 + ((time - 1.0) * 180 / (1.0)));
        contangle = radians((time - 1.0) * -360 / (1.0));
        if(gl_VertexID >= ring1 && gl_VertexID < ring2up){
            gl_Position.xy = vertexPosition.xy;
            shaderColor.rgb = white;
        }
        if(gl_VertexID >= ring2up && gl_VertexID < ring3left){
            mat4 rotationMatrix = mat4(
                cos(angle), -sin(angle),     0.0f, 0.0f,
                sin(angle),  cos(angle),     0.0f, 0.0f,
                0.0f,        0.0f,           1.0f, 0.0f,
                0.0f,        0.0f,           0.0f, 1.0f
            );
            gl_Position.xy = vertexPosition.xy;
            shaderColor.rgb = white;
            gl_Position = rotationMatrix * gl_Position;
        }
        if(gl_VertexID >= ring3left && gl_VertexID < topright){
            mat4 rotationMatrix = mat4(
                cos(contangle), -sin(contangle),     0.0f, 0.0f,
                sin(contangle),  cos(contangle),     0.0f, 0.0f,
                0.0f,        0.0f,           1.0f, 0.0f,
                0.0f,        0.0f,           0.0f, 1.0f
            );
            gl_Position.xy = mix(vertexDisplaced.xy, vertexPosition.xy, time-floor(time));
            gl_Position = rotationMatrix * gl_Position;
            shaderColor = mix(shaderColor, white, time-floor(time));
        }
    } 
    if(time >= 2.0 && time <= 3.0) {
        angle = radians((time - 1.0) * -360 / (1.0));
        if(gl_VertexID<topright){
            shaderColor = white;
            gl_Position.xy = vertexPosition.xy;
        }
        if(gl_VertexID >= ring3left && gl_VertexID < topright){
            mat4 rotationMatrix = mat4(
                cos(angle), -sin(angle),     0.0f, 0.0f,
                sin(angle),  cos(angle),     0.0f, 0.0f,
                0.0f,        0.0f,           1.0f, 0.0f,
                0.0f,        0.0f,           0.0f, 1.0f
            );
            gl_Position = rotationMatrix * gl_Position;
        }
        if(gl_VertexID >= topright){
            gl_Position.xy = mix(vertexDisplaced.xy, vertexPosition.xy, time-floor(time));
            shaderColor = mix(shaderColor, white, time-floor(time));
        }
    }

    if (time >= 3.0 && time <= 6.0) {
        gl_Position.xy = vertexPosition.xy;
        shaderColor = white;
    }

    if(time >= 6.0 && time <= 7.0){
        angle = radians((time - 1.0) * 360 / (1.0));
        if(gl_VertexID < topright){
            shaderColor = white;
            gl_Position.xy = vertexPosition.xy;
        }
        if(gl_VertexID >= topright){
            gl_Position.xy = mix(vertexPosition.xy, vertexDisplaced.xy, time-floor(time));
            shaderColor = mix(white, shaderColor, time-floor(time));
        }
        if(gl_VertexID >= ring3left && gl_VertexID < topright){
            mat4 rotationMatrix = mat4(
                cos(angle), -sin(angle),     0.0f, 0.0f,
                sin(angle),  cos(angle),     0.0f, 0.0f,
                0.0f,        0.0f,           1.0f, 0.0f,
                0.0f,        0.0f,           0.0f, 1.0f
            );
            gl_Position = rotationMatrix * gl_Position;
        }
    } 
    
    if(time >= 7.0 && time <= 8.0) {
        angle = radians((time - 1.0) * 360 / (1.0));
        contangle = radians((time - 1.0) * -180 / (1.0));
        if(gl_VertexID >= topright){
            gl_Position.xy = vertexDisplaced.xy;
        }
        if(gl_VertexID >= ring1 && gl_VertexID < ring2up){
            gl_Position.xy = vertexPosition.xy;
            shaderColor = white;
        }
        if(gl_VertexID >= ring2up && gl_VertexID < ring3left){
            mat4 rotationMatrix = mat4(
                cos(contangle), -sin(contangle),     0.0f, 0.0f,
                sin(contangle),  cos(contangle),     0.0f, 0.0f,
                0.0f,            0.0f,               1.0f, 0.0f,
                0.0f,            0.0f,               0.0f, 1.0f
            );
            gl_Position.xy = vertexPosition.xy;
            gl_Position = rotationMatrix * gl_Position;
            shaderColor = white;
        }
        if(gl_VertexID >= ring3left && gl_VertexID < topright){
            mat4 rotationMatrix = mat4(
                cos(angle), -sin(angle),     0.0f, 0.0f,
                sin(angle),  cos(angle),     0.0f, 0.0f,
                0.0f,        0.0f,           1.0f, 0.0f,
                0.0f,        0.0f,           0.0f, 1.0f
            );
            gl_Position.xy = mix(vertexPosition.xy, vertexDisplaced.xy, time-floor(time));
            gl_Position = rotationMatrix * gl_Position;
            shaderColor = mix(white, shaderColor, time-floor(time));
        }
    } 
    if(time >= 8.0 && time <= 9.0) {
        contangle = radians((time - 1.0) * -180 / (1.0));
        if(gl_VertexID >= ring3left){
            gl_Position.xy = vertexDisplaced.xy;
        }
        if(gl_VertexID >= ring1 && gl_VertexID < ring2up){
            gl_Position.xy = mix(vertexPosition.xy, vertexDisplaced.xy, time-floor(time));
            shaderColor = mix(white, shaderColor, time-floor(time));
        }
        if(gl_VertexID >= ring2up && gl_VertexID < ring3left){
            mat4 rotationMatrix = mat4(
                cos(contangle), -sin(contangle),     0.0f, 0.0f,
                sin(contangle),  cos(contangle),     0.0f, 0.0f,
                0.0f,        0.0f,           1.0f, 0.0f,
                0.0f,        0.0f,           0.0f, 1.0f
            );
            gl_Position.xy = mix(vertexPosition.xy, vertexDisplaced.xy, time-floor(time));
            gl_Position = rotationMatrix * gl_Position;
            shaderColor = mix(white, shaderColor, time-floor(time));
        }
    }
}