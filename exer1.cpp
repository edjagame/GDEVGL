/******************************************************************************
 * This demo draws a somewhat familiar geometric object composed of three
 * triangles.
 *
 * The main differences between this demo and the previous demo are:
 * - Each vertex now defines two subcomponents: position and color.
 * - Instead of defining the same vertices multiple times for different
 *   triangles, we now define the triangles using vertex indices instead,
 *   thereby making more efficient use of GPU memory. The indices are
 *   uploaded to the GPU using an Element Buffer Object (EBO).
 * - A uniform variable is introduced to the shader program, which is used to
 *   accomplish a glowing effect.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <gdev.h>

// change this to your desired window attributes
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 360
#define WINDOW_TITLE  "CRYO VISION"
#define I_CENTER 0
#define I_HEXAGON 6
GLFWwindow *pWindow;

// define a vertex array to hold our vertices


float vertices[] =
    {
//DODECAGON
 0.0000,  0.0000,    0.0f, 1.0f, 1.0f, 1.0f, //0
 0.2812,  0.0000,    0.0f, 0.5f, 0.8f, 1.0f, //1
 0.2436,  0.2500,    0.0f, 0.5f, 0.8f, 1.0f, //2
 0.1406,  0.4330,    0.0f, 0.5f, 0.8f, 1.0f, //3
 0.0000,  0.5000,    0.0f, 0.5f, 0.8f, 1.0f, //4
-0.1406,  0.4330,    0.0f, 0.5f, 0.8f, 1.0f, //5
-0.2436,  0.2500,    0.0f, 0.5f, 0.8f, 1.0f, //6
-0.2812,  0.0000,    0.0f, 0.5f, 0.8f, 1.0f, //7
-0.2436, -0.2500,    0.0f, 0.5f, 0.8f, 1.0f, //8
-0.1406, -0.4330,    0.0f, 0.5f, 0.8f, 1.0f, //9
-0.0000, -0.5000,    0.0f, 0.5f, 0.8f, 1.0f, //10
 0.1406, -0.4330,    0.0f, 0.5f, 0.8f, 1.0f, //11
 0.2436, -0.2500,    0.0f, 0.5f, 0.8f, 1.0f, //12

//SNOWFLAKE INNER 1
 0.0000,  0.0000,    0.0f, 1.0f, 1.0f, 1.0f, //13
 0.0703,  0.0000,    0.0f, 1.0f, 1.0f, 1.0f, //14
 0.0543,  0.0259,    0.0f, 1.0f, 1.0f, 1.0f, //15
 0.0352,  0.1083,    0.0f, 1.0f, 1.0f, 1.0f, //16
 0.0146,  0.0966,    0.0f, 1.0f, 1.0f, 1.0f, //17
-0.0352,  0.1083,    0.0f, 1.0f, 1.0f, 1.0f, //18
-0.0398,  0.0707,    0.0f, 1.0f, 1.0f, 1.0f, //19
-0.0703,  0.0000,    0.0f, 1.0f, 1.0f, 1.0f, //20
-0.0543, -0.0259,    0.0f, 1.0f, 1.0f, 1.0f, //21
-0.0352, -0.1083,    0.0f, 1.0f, 1.0f, 1.0f, //22
-0.0146, -0.0966,    0.0f, 1.0f, 1.0f, 1.0f, //23
 0.0352, -0.1083,    0.0f, 1.0f, 1.0f, 1.0f, //24
 0.0398, -0.0707,    0.0f, 1.0f, 1.0f, 1.0f, //25

//SNOWFLAKE INNER 2
 0.0000,  0.0000,    0.0f, 1.0f, 1.0f, 1.0f, //26
 0.0703,  0.0000,    0.0f, 1.0f, 1.0f, 1.0f, //27
 0.0543, -0.0259,    0.0f, 1.0f, 1.0f, 1.0f, //28
 0.0352,  0.1083,    0.0f, 1.0f, 1.0f, 1.0f, //29
 0.0398,  0.0707,    0.0f, 1.0f, 1.0f, 1.0f, //30
-0.0352,  0.1083,    0.0f, 1.0f, 1.0f, 1.0f, //31
-0.0146,  0.0966,    0.0f, 1.0f, 1.0f, 1.0f, //32
-0.0703,  0.0000,    0.0f, 1.0f, 1.0f, 1.0f, //33
-0.0543,  0.0259,    0.0f, 1.0f, 1.0f, 1.0f, //34
-0.0352, -0.1083,    0.0f, 1.0f, 1.0f, 1.0f, //35
-0.0398, -0.0707,    0.0f, 1.0f, 1.0f, 1.0f, //36
 0.0352, -0.1083,    0.0f, 1.0f, 1.0f, 1.0f, //37
 0.0146, -0.0966,    0.0f, 1.0f, 1.0f, 1.0f, //38

//SNOWFLAKE OUTER 1

 0.0000,  0.0000,    0.0f, 1.0f, 1.0f, 1.0f, //39
 0.1453,  0.0877,    0.0f, 1.0f, 1.0f, 1.0f, //40
 0.1682,  0.1474,    0.0f, 1.0f, 1.0f, 1.0f, //41
 0.0299,  0.2675,    0.0f, 1.0f, 1.0f, 1.0f, //42
 0.0123,  0.3326,    0.0f, 1.0f, 1.0f, 1.0f, //43
-0.1153,  0.1798,    0.0f, 1.0f, 1.0f, 1.0f, //44
-0.1559,  0.1852,    0.0f, 1.0f, 1.0f, 1.0f, //45
-0.1453, -0.0877,    0.0f, 1.0f, 1.0f, 1.0f, //46
-0.1682, -0.1474,    0.0f, 1.0f, 1.0f, 1.0f, //47
-0.0299, -0.2675,    0.0f, 1.0f, 1.0f, 1.0f, //48
-0.0123, -0.3326,    0.0f, 1.0f, 1.0f, 1.0f, //49
 0.1153, -0.1798,    0.0f, 1.0f, 1.0f, 1.0f, //50
 0.1559, -0.1852,    0.0f, 1.0f, 1.0f, 1.0f, //51

//SNOWFLAKE OUTER 2
 0.0000,  0.0000,    0.0f, 1.0f, 1.0f, 1.0f, //52
 0.1559,  0.1852,    0.0f, 1.0f, 1.0f, 1.0f, //53
 0.1269,  0.1978,    0.0f, 1.0f, 1.0f, 1.0f, //54
-0.0123,  0.3326,    0.0f, 1.0f, 1.0f, 1.0f, //55
-0.0329,  0.2942,    0.0f, 1.0f, 1.0f, 1.0f, //56
-0.1682,  0.1474,    0.0f, 1.0f, 1.0f, 1.0f, //57
-0.1598,  0.0964,    0.0f, 1.0f, 1.0f, 1.0f, //58
-0.1559, -0.1852,    0.0f, 1.0f, 1.0f, 1.0f, //59
-0.1269, -0.1978,    0.0f, 1.0f, 1.0f, 1.0f, //60
 0.0123, -0.3326,    0.0f, 1.0f, 1.0f, 1.0f, //61
 0.0329, -0.2942,    0.0f, 1.0f, 1.0f, 1.0f, //62
 0.1682, -0.1474,    0.0f, 1.0f, 1.0f, 1.0f, //63
 0.1598, -0.0964,    0.0f, 1.0f, 1.0f, 1.0f, //64

//SNOWFLAKE OUTER 3
 0.1406,  0.0000,    0.0f, 1.0f, 1.0f, 1.0f, //65
 0.1461,  0.1500,    0.0f, 1.0f, 1.0f, 1.0f, //66
 0.1949,  0.2000,    0.0f, 1.0f, 1.0f, 1.0f, //67
 0.0703,  0.2165,    0.0f, 1.0f, 1.0f, 1.0f, //68
 0.0000,  0.3000,    0.0f, 1.0f, 1.0f, 1.0f, //69
 0.0000,  0.4000,    0.0f, 1.0f, 1.0f, 1.0f, //70
-0.0703,  0.2165,    0.0f, 1.0f, 1.0f, 1.0f, //71
-0.1461,  0.1500,    0.0f, 1.0f, 1.0f, 1.0f, //72
-0.1949,  0.2000,    0.0f, 1.0f, 1.0f, 1.0f, //73
-0.1406,  0.0000,    0.0f, 1.0f, 1.0f, 1.0f, //74
-0.1461, -0.1500,    0.0f, 1.0f, 1.0f, 1.0f, //75
-0.1949, -0.2000,    0.0f, 1.0f, 1.0f, 1.0f, //76
-0.0703, -0.2165,    0.0f, 1.0f, 1.0f, 1.0f, //77
-0.0000, -0.3000,    0.0f, 1.0f, 1.0f, 1.0f, //78
-0.0000, -0.4000,    0.0f, 1.0f, 1.0f, 1.0f, //79
 0.0703, -0.2165,    0.0f, 1.0f, 1.0f, 1.0f, //80
 0.1461, -0.1500,    0.0f, 1.0f, 1.0f, 1.0f, //81
 0.1949, -0.2000,    0.0f, 1.0f, 1.0f, 1.0f, //82

//SNOWFLAKE OUTER 4
 0.0703,  0.2165,    0.0f, 1.0f, 1.0f, 1.0f, //83
 0.1461,  0.1500,    0.0f, 1.0f, 1.0f, 1.0f, //84
 0.1949,  0.2000,    0.0f, 1.0f, 1.0f, 1.0f, //85
-0.0703,  0.2165,    0.0f, 1.0f, 1.0f, 1.0f, //86
 0.0000,  0.3000,    0.0f, 1.0f, 1.0f, 1.0f, //87
 0.0000,  0.4000,    0.0f, 1.0f, 1.0f, 1.0f, //88
-0.1406,  0.0000,    0.0f, 1.0f, 1.0f, 1.0f, //89
-0.1461,  0.1500,    0.0f, 1.0f, 1.0f, 1.0f, //90
-0.1949,  0.2000,    0.0f, 1.0f, 1.0f, 1.0f, //91
-0.0703, -0.2165,    0.0f, 1.0f, 1.0f, 1.0f, //92
-0.1461, -0.1500,    0.0f, 1.0f, 1.0f, 1.0f, //93
-0.1949, -0.2000,    0.0f, 1.0f, 1.0f, 1.0f, //94
 0.0703, -0.2165,    0.0f, 1.0f, 1.0f, 1.0f, //95
-0.0000, -0.3000,    0.0f, 1.0f, 1.0f, 1.0f, //96
-0.0000, -0.4000,    0.0f, 1.0f, 1.0f, 1.0f, //97
 0.1406, -0.0000,    0.0f, 1.0f, 1.0f, 1.0f, //98
 0.1461, -0.1500,    0.0f, 1.0f, 1.0f, 1.0f, //99
 0.1949, -0.2000,    0.0f, 1.0f, 1.0f, 1.0f, //100

//DODECAGON 2.1
 0.2961,  0.0000,    0.0f, 0.5f, 0.5f, 0.5f, //101
 0.2564,  0.2632,    0.0f, 0.5f, 0.5f, 0.5f, //102
 0.1480,  0.4558,    0.0f, 0.5f, 0.5f, 0.5f, //103
 0.0000,  0.6250,    0.0f, 0.5f, 0.5f, 0.5f, //104
-0.1480,  0.4558,    0.0f, 0.5f, 0.5f, 0.5f, //105
-0.2564,  0.2632,    0.0f, 0.5f, 0.5f, 0.5f, //106
-0.2961,  0.0000,    0.0f, 0.5f, 0.5f, 0.5f, //107
-0.2564, -0.2632,    0.0f, 0.5f, 0.5f, 0.5f, //108
-0.1480, -0.4558,    0.0f, 0.5f, 0.5f, 0.5f, //109
-0.0000, -0.5263,    0.0f, 0.5f, 0.5f, 0.5f, //110
 0.1480, -0.4558,    0.0f, 0.5f, 0.5f, 0.5f, //111
 0.2564, -0.2632,    0.0f, 0.5f, 0.5f, 0.5f, //112

//DODECAGON 2.2
 0.3750,  0.0000,    0.0f, 0.5f, 0.5f, 0.5f, //113
 0.3248,  0.3333,    0.0f, 0.5f, 0.5f, 0.5f, //114
 0.1875,  0.5774,    0.0f, 0.5f, 0.5f, 0.5f, //115
 0.0000,  0.6667,    0.0f, 0.5f, 0.5f, 0.5f, //116
-0.1875,  0.5774,    0.0f, 0.5f, 0.5f, 0.5f, //117
-0.3248,  0.3333,    0.0f, 0.5f, 0.5f, 0.5f, //118
-0.3750,  0.0000,    0.0f, 0.5f, 0.5f, 0.5f, //119
-0.3248, -0.3333,    0.0f, 0.5f, 0.5f, 0.5f, //120
-0.1875, -0.5774,    0.0f, 0.5f, 0.5f, 0.5f, //121
-0.0000, -0.6667,    0.0f, 0.5f, 0.5f, 0.5f, //122
 0.1875, -0.5774,    0.0f, 0.5f, 0.5f, 0.5f, //123
 0.3248, -0.3333,    0.0f, 0.5f, 0.5f, 0.5f, //124

//TOP
 0.0420,  0.7500,    0.0f, 0.5f, 0.5f, 0.5f, //125
-0.0420,  0.7500,    0.0f, 0.5f, 0.5f, 0.5f, //126
 0.0000,  0.9000,    0.0f, 0.5f, 0.5f, 0.5f, //127
 0.0000,  1.0000,    0.0f, 0.5f, 0.5f, 0.5f, //128

//BOTTOM
 0.0000, -1.0000,    0.0f, 0.5f, 0.5f, 0.5f, //129

//SPIKES
-0.2340,  0.4159,    0.0f, 0.5f, 0.5f, 0.5f, //130
-0.3196,  0.1522,    0.0f, 0.5f, 0.5f, 0.5f, //131
-0.4871,  0.5000,    0.0f, 0.5f, 0.5f, 0.5f, //132
 0.3196,  0.1522,    0.0f, 0.5f, 0.5f, 0.5f, //133
 0.2340,  0.4159,    0.0f, 0.5f, 0.5f, 0.5f, //134
 0.4871,  0.5000,    0.0f, 0.5f, 0.5f, 0.5f, //135

-0.2564, -0.2632,    0.0f, 0.5f, 0.5f, 0.5f, //136
-0.2961,  0.0000,    0.0f, 0.5f, 0.5f, 0.5f, //137
-0.3747, -0.3846,    0.0f, 0.5f, 0.5f, 0.5f, //138
-0.5625,  0.0000,    0.0f, 0.5f, 0.5f, 0.5f, //139
 0.2564, -0.2632,    0.0f, 0.5f, 0.5f, 0.5f, //140
 0.2961,  0.0000,    0.0f, 0.5f, 0.5f, 0.5f, //141
 0.3747, -0.3846,    0.0f, 0.5f, 0.5f, 0.5f, //142
 0.5625,  0.0000,    0.0f, 0.5f, 0.5f, 0.5f, //143

//SPIKES DESIGN
-0.2975,  0.4058,    0.0f, 0.8f, 0.8f, 0.8f, //144
-0.3465,  0.2551,    0.0f, 0.8f, 0.8f, 0.8f, //145
-0.4429,  0.4545,    0.0f, 0.6f, 0.6f, 0.6f, //146
 0.3465,  0.2551,    0.0f, 0.8f, 0.8f, 0.8f, //147
 0.2975,  0.4058,    0.0f, 0.8f, 0.8f, 0.8f, //148
 0.4429,  0.4545,    0.0f, 0.6f, 0.6f, 0.6f, //149

-0.2784, -0.2522,    0.0f, 0.6f, 0.6f, 0.6f, //150
-0.3121, -0.0291,    0.0f, 0.8f, 0.8f, 0.8f, //151
-0.3713, -0.3363,    0.0f, 0.8f, 0.8f, 0.8f, //152
-0.5250, -0.0291,    0.0f, 0.6f, 0.6f, 0.6f, //153
 0.2784, -0.2522,    0.0f, 0.6f, 0.6f, 0.6f, //154
 0.3121, -0.0291,    0.0f, 0.8f, 0.8f, 0.8f, //155
 0.3713, -0.3363,    0.0f, 0.8f, 0.8f, 0.8f, //156
 0.5250, -0.0291,    0.0f, 0.6f, 0.6f, 0.6f, //157

-0.1654, -0.5094,    0.0f, 0.6f, 0.6f, 0.6f, //158
 0.1654, -0.5094,    0.0f, 0.6f, 0.6f, 0.6f, //159
-0.0000, -0.5882,    0.0f, 0.9f, 0.9f, 0.9f, //160
 0.0000, -0.9000,    0.0f, 0.9f, 0.9f, 0.9f, //161
};

// define the triangles as triplets of indices into the vertex array
GLuint indices[] =
{
//DODECAGON
0,2,1,
0,3,2,
0,4,3,
0,5,4,
0,6,5,
0,7,6,
0,8,7,
0,9,8,
0,10,9,
0,11,10,
0,12,11,
0,1,12,

//SNOWFLAKE INNER 1
13,15,14,
13,17,16,
13,19,18,
13,21,20,
13,23,22,
13,25,24,

//SNOWFLAKE INNER 2
26,28,27,
26,30,29,
26,32,31,
26,34,33,
26,36,35,
26,38,37,

//SNOWFLAKE OUTER 1
39,41,40,
39,43,42,
39,45,44,
39,47,46,
39,49,48,
39,51,50,

//SNOWFLAKE OUTER 2
52,54,53,
52,56,55,
52,58,57,
52,60,59,
52,62,61,
52,64,63,

//SNOWFLAKE OUTER 3
65,67,66,
68,70,69,
71,73,72,
74,76,75,
77,79,78,
80,82,81,

//SNOWFLAKE OUTER 4
83,85,84,
86,88,87,
89,91,90,
92,94,93,
95,97,96,
98,100,99,

//DODECAGON 2.1
101,102,113,
102,103,114,
103,104,115,
104,105,116,
105,106,117,
106,107,118,
107,108,119,
108,109,120,
109,110,121,
110,111,122,
111,112,123,
112,101,124,

//DODECAGON 2.2
102,114,113,
103,115,114,
104,116,115,
105,117,116,
106,118,117,
107,119,118,
108,120,119,
109,121,120,
110,122,121,
111,123,122,
112,124,123,
101,113,124,

//TOP
115,116,125,
116,117,126,
126,117,127,
127,115,125,
128,117,127,
127,115,128,

//BOTTOM
129,121,122,
129,122,123,

//SPIKES
130,131,132,
133,134,135,
136,137,138,
137,138,139,
140,141,142,
141,142,143,

//SPIKES DESIGN
144,145,146,
147,148,149,
150,151,152,
151,152,153,
154,155,156,
155,156,157,

161,160,158,
160,161,159,
};

// define OpenGL object IDs to represent the vertex array and the shader program in the GPU
GLuint vao;         // vertex array object (stores the render state for our vertex array)
GLuint vbo;         // vertex buffer object (reserves GPU memory for our vertex array)
GLuint ebo;         // element buffer object (stores the indices of the vertices to form triangles)
GLuint shader;      // combined vertex and fragment shader

// called by the main function to do initial setup, such as uploading vertex
// arrays, shader programs, etc.; returns true if successful, false otherwise
bool setup()
{
    // generate the VAO, VBO, and EBO objects and store their IDs in vao, vbo, and ebo, respectively
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // bind the newly-created VAO to make it the current one that OpenGL will apply state changes to
    glBindVertexArray(vao);

    // upload our vertex array data to the newly-created VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // upload our index array data to the newly-created EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // on the VAO, register the current VBO with the following vertex attribute layout:
    // - the stride length of the vertex array is 6 floats (6 * sizeof(float))
    // - layout location 0 (position) is 3 floats and starts at the first float of the vertex array (offset 0)
    // - layout location 1 (color) is also 3 floats but starts at the fourth float (offset 3 * sizeof(float))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));

    // enable the layout locations so they can be used by the vertex shader
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // important: if you have more vertex arrays to draw, make sure you separately define them
    // with unique VAO, VBO, and EBO IDs, and follow the same process above to upload them to the GPU

    // load our shader program
    shader = gdevLoadShader("exer1.vs", "exer1.fs");
    if (! shader)
        return false;

    return true;
}

// called by the main function to do rendering per frame
void render()
{
    // clear the whole frame
    glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // compute a value for the glow amount for this frame
    float glow = (sin(glfwGetTime()/3.0f) + 1.0f)/2.0f;
    float scale = (sin(glfwGetTime()/3.0f)/4.0f) + 0.75f;
    float scale2 = (sin(glfwGetTime()/3.0f)/20.0f) + 0.95f;  
    float colorChange1 = (sin(glfwGetTime()/3.0) + 1.0f)/2.0f;
    float colorChange2 = (sin(0.5*glfwGetTime()/3.0+2.0/3.0*3.14) + 1.0f)/2.0f;
    float colorChange3 = (sin(glfwGetTime()/3.0+4.0/3.0*3.14) + 1.0f)/2.0f;
    // using our shader program...
    glUseProgram(shader);

    // ... set the uniform variables of the shader...
    // (in this case, simply set the value of the glow)
    glUniform1f(glGetUniformLocation(shader, "glow"), glow);
    glUniform1f(glGetUniformLocation(shader, "scale"), scale);
    glUniform1f(glGetUniformLocation(shader, "scale2"), scale2);
    glUniform3f(glGetUniformLocation(shader, "colorChange"), colorChange1, colorChange2, colorChange3);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
}

/*****************************************************************************/

// handler called by GLFW when there is a keyboard event
void handleKeys(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    // pressing Esc closes the window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
}

// handler called by GLFW when the window is resized
void handleResize(GLFWwindow* pWindow, int width, int height)
{
    // tell OpenGL to do its drawing within the entire "client area" (area within the borders) of the window
    glViewport(0, 0, width, height);
}

// main function
int main(int argc, char** argv)
{
    // initialize GLFW and ask for OpenGL 3.3 core
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // create a GLFW window with the specified width, height, and title
    pWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (! pWindow)
    {
        // gracefully terminate if we cannot create the window
        std::cout << "Cannot create the GLFW window.\n";
        glfwTerminate();
        return -1;
    }

    // make the window the current context of subsequent OpenGL commands,
    // and enable vertical sync and aspect-ratio correction on the GLFW window
    glfwMakeContextCurrent(pWindow);
    glfwSwapInterval(1);
    glfwSetWindowAspectRatio(pWindow, WINDOW_WIDTH, WINDOW_HEIGHT);

    // set up callback functions to handle window system events
    glfwSetKeyCallback(pWindow, handleKeys);
    glfwSetFramebufferSizeCallback(pWindow, handleResize);

    // don't miss any momentary keypresses
    glfwSetInputMode(pWindow, GLFW_STICKY_KEYS, GLFW_TRUE);

    // initialize GLAD, which acts as a library loader for the current OS's native OpenGL library
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // if our initial setup is successful...
    if (setup())
    {
        // do rendering in a loop until the user closes the window
        while (! glfwWindowShouldClose(pWindow))
        {
            // render our next frame
            // (by default, GLFW uses double-buffering with a front and back buffer;
            // all drawing goes to the back buffer, so the frame does not get shown yet)
            render();

            // swap the GLFW front and back buffers to show the next frame
            glfwSwapBuffers(pWindow);

            // process any window events (such as moving, resizing, keyboard presses, etc.)
            glfwPollEvents();
        }
    }

    // gracefully terminate the program
    glfwTerminate();
    return 0;
}
