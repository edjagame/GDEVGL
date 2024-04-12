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
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800
#define WINDOW_TITLE  "AMOGUS"
#define I_CENTER 0
#define I_HEXAGON 6
GLFWwindow *pWindow;

// define a vertex array to hold our vertices


float vertices[] = {
 0.0000, -0.5000,  0.0000,  1.0f,  0.3f,  0.3f, 0.50f, 0.50f, 0,//0
 0.0000, -0.5000, -0.3333,  1.0f,  0.3f,  0.3f, 0.00f, 0.00f, 0,//1
-0.2887, -0.5000, -0.1667,  1.0f,  0.3f,  0.3f, 0.00f, 1.00f, 0,//2
-0.2887, -0.5000,  0.1667,  1.0f,  0.3f,  0.3f, 0.00f, 0.00f, 0,//3
 0.0000, -0.5000,  0.3333,  1.0f,  0.3f,  0.3f, 0.00f, 1.00f, 0,//4
 0.2887, -0.5000,  0.1667,  1.0f,  0.3f,  0.3f, 0.00f, 0.00f, 0,//5
 0.2887, -0.5000, -0.1667,  1.0f,  0.3f,  0.3f, 0.00f, 1.00f, 0,//6
 0.0000,  0.3333, -0.3333,  1.0f,  0.3f,  0.3f, 1.00f, 0.00f, 0,//7
 0.1443,  0.5000, -0.2500,  1.0f,  0.3f,  0.3f, 0.00f, 0.00f, 0,//8
-0.1443,  0.5000, -0.2500,  1.0f,  0.3f,  0.3f, 0.00f, 0.16f, 0,//9
-0.1443,  0.5000, -0.2500,  1.0f,  0.3f,  0.3f, 0.00f, 0.00f, 0,//10
-0.2887,  0.5000,  0.0000,  1.0f,  0.3f,  0.3f, 0.00f, 0.33f, 0,//11
-0.2887,  0.3333, -0.1667,  1.0f,  0.3f,  0.3f, 1.00f, 1.00f, 0,//12
-0.2887,  0.5000,  0.0000,  1.0f,  0.3f,  0.3f, 0.00f, 0.00f, 0,//13
-0.1443,  0.5000,  0.2500,  1.0f,  0.3f,  0.3f, 0.00f, 0.50f, 0,//14
-0.2887,  0.3333,  0.1667,  1.0f,  0.3f,  0.3f, 1.00f, 0.00f, 0,//15
-0.1443,  0.5000,  0.2500,  1.0f,  0.3f,  0.3f, 0.00f, 0.00f, 0,//16
 0.1443,  0.5000,  0.2500,  1.0f,  0.3f,  0.3f, 0.00f, 0.66f, 0,//17
 0.0000,  0.3333,  0.3333,  1.0f,  0.3f,  0.3f, 1.00f, 1.00f, 0,//18
 0.1443,  0.5000,  0.2500,  1.0f,  0.3f,  0.3f, 0.00f, 0.00f, 0,//19
 0.2887,  0.5000,  0.0000,  1.0f,  0.3f,  0.3f, 0.00f, 0.84f, 0,//20
 0.2887,  0.3333,  0.1667,  1.0f,  0.3f,  0.3f, 1.00f, 0.00f, 0,//21
 0.2887,  0.5000,  0.0000,  1.0f,  0.3f,  0.3f, 0.00f, 0.00f, 0,//22
 0.1443,  0.5000, -0.2500,  1.0f,  0.3f,  0.3f, 0.00f, 1.00f, 0,//23
 0.2887,  0.3333, -0.1667,  1.0f,  0.3f,  0.3f, 1.00f, 1.00f, 0,//24
-0.1421,  0.5019,  0.2484,  1.0f,  0.3f,  0.3f, 0.00f, 0.00f, 0,//25
 0.0000,  0.6000,  0.0000,  1.0f,  0.3f,  0.3f, 1.00f, 0.50f, 0,//26
 0.0000,  0.0556,  0.3333,  0.0f,  1.0f,  1.0f, 0.33f, 0.00f, 1,//27
 0.0000, -0.0833, -0.3333,  0.7f,  0.2f,  0.2f, 0.33f, 0.00f, 0,//28
 0.2887,  0.0556,  0.1667,  0.0f,  1.0f,  1.0f, 0.66f, 0.00f, 1,//29
-0.2887, -0.0833, -0.1667,  0.7f,  0.2f,  0.2f, 0.33f, 0.00f, 0,//30
 0.1443,  0.5000,  0.2500,  1.0f,  1.0f,  1.0f, 0.00f, 0.00f, 0,//31
 0.0000,  0.3333,  0.3333,  1.0f,  1.0f,  1.0f, 0.00f, 0.66f, 1,//32 
 0.1443,  0.5000,  0.2500,  1.0f,  1.0f,  1.0f, 0.00f, 0.00f, 0,//33
 0.2887,  0.3333,  0.1667,  1.0f,  1.0f,  1.0f, 1.00f, 0.66f, 1,//34 
 0.0000,  0.0556,  0.3333,  1.0f,  1.0f,  1.0f, 0.00f, 0.33f, 1,//35
 0.2887,  0.0556,  0.1667,  1.0f,  1.0f,  1.0f, 0.00f, 0.00f, 1,//36 
 0.0000,  0.3333,  0.3333,  1.0f,  1.0f,  1.0f, 0.00f, 0.00f, 0,//37
 0.2887,  0.3333,  0.1667,  1.0f,  1.0f,  1.0f, 0.00f, 0.00f, 0,//38
 0.0000,  0.0556,  0.3333,  1.0f,  1.0f,  1.0f, 0.00f, 0.00f, 0,//39
 0.2887,  0.0556,  0.1667,  1.0f,  1.0f,  1.0f, 0.00f, 0.00f, 0,//40
 0.0000,  0.3333,  0.3333,  1.0f,  1.0f,  1.0f, 0.00f, 0.00f, 0,//41
 0.2887,  0.3333,  0.1667,  1.0f,  1.0f,  1.0f, 0.00f, 0.00f, 0,//42
 0.0000,  0.0556,  0.3333,  1.0f,  1.0f,  1.0f, 0.00f, 0.00f, 0,//43
 0.2887,  0.0556,  0.1667,  1.0f,  1.0f,  1.0f, 0.00f, 0.00f, 0,//44
 0.0833,  0.3333,  0.4777,  1.0f,  1.0f,  1.0f, 0.33f, 0.66f, 1,//45
 0.3720,  0.3333,  0.3110,  1.0f,  1.0f,  1.0f, 0.66f, 0.66f, 1,//46
 0.0833,  0.0556,  0.4777,  0.0f,  1.0f,  1.0f, 0.33f, 0.33f, 1,//47
 0.3720,  0.0556,  0.3110,  0.0f,  1.0f,  1.0f, 0.66f, 0.33f, 1,//48
 0.0833,  0.3333,  0.4777,  1.0f,  1.0f,  1.0f, 0.00f, 0.00f, 0,//49
 0.3720,  0.3333,  0.3110,  1.0f,  1.0f,  1.0f, 0.00f, 0.00f, 0,//50
 0.0833,  0.0556,  0.4777,  1.0f,  1.0f,  1.0f, 0.00f, 0.00f, 0,//51
 0.3720,  0.0556,  0.3110,  1.0f,  1.0f,  1.0f, 0.00f, 0.00f, 0,//52
 0.0833,  0.3333,  0.4777,  1.0f,  1.0f,  1.0f, 0.00f, 0.00f, 0,//53
 0.3720,  0.3333,  0.3110,  1.0f,  1.0f,  1.0f, 0.00f, 0.00f, 0,//54
 0.0833,  0.0556,  0.4777,  1.0f,  1.0f,  1.0f, 0.00f, 0.00f, 0,//55
 0.3720,  0.0556,  0.3110,  1.0f,  1.0f,  1.0f, 0.00f, 0.00f, 0,//56
-0.0833,  0.3333, -0.4777,  0.7f,  0.2f,  0.2f, 0.33f, 0.66f, 0,//57
-0.3720,  0.3333, -0.3110,  0.7f,  0.2f,  0.2f, 0.66f, 0.66f, 0,//58
-0.0833, -0.0833, -0.4777,  0.7f,  0.2f,  0.2f, 0.33f, 0.33f, 0,//59
-0.3720, -0.0833, -0.3110,  0.7f,  0.2f,  0.2f, 0.66f, 0.33f, 0,//60
-0.2887,  0.3333, -0.1667,  0.7f,  0.2f,  0.2f, 0.66f, 1.00f, 0,//61 
 0.0000,  0.3333, -0.3333,  0.7f,  0.2f,  0.2f, 0.33f, 1.00f, 0,//62
 0.0000,  0.3333,  0.3333,  0.0f,  1.0f,  1.0f, 0.33f, 1.00f, 1,//63 
 0.2887,  0.3333,  0.1667,  0.0f,  1.0f,  1.0f, 0.66f, 1.00f, 1,//64
};

// define the triangles as triplets of indices into the vertex array
GLuint indices[] = {
//BOTTOM HEXAGON
0,2,1,
0,3,2,
0,4,3,
0,5,4,
0,6,5,
0,1,6,

//TOP CURVE 1
7,12,9,
12,15,11,
15,18,14,
18,21,17,
21,24,20,
24,7,23,

//TOP CURVE 2
11,9,12,
14,11,15,
17,14,18,
20,17,21,
23,20,24,
9,23,7,

//TOP CURVE 3
9,11,26,
11,14,26,
14,17,26,
17,20,26,
20,23,26,
23,9,26,

//VISOR
47,45,32,
47,32,35,

48,46,45,
48,45,47,

36,34,46,
36,46,48,

46,64,63,
46,63,45,

29,48,47,
29,47,27,

//BACKPACK
//28, 30, 57, 58, 59, 60, 61, 62
61,57,58,
61,62,57,

61,58,60,
61,60,30,

58,57,59,
58,59,60,

57,62,28,
57,28,59,

28,30,60,
28,60,59,

//BODY
1,2,7,
2,3,12,
3,4,15,
4,5,18,
5,6,21,
6,1,24,
7,2,12,
12,3,15,
15,4,18,
18,5,21,
21,6,24,
24,1,7,
};

// define OpenGL object IDs to represent the vertex array and the shader program in the GPU
GLuint vao;         // vertex array object (stores the render state for our vertex array)
GLuint vbo;         // vertex buffer object (reserves GPU memory for our vertex array)
GLuint ebo;         // element buffer object (stores the indices of the vertices to form triangles)
GLuint shader;      // combined vertex and fragment shader
GLuint marbleTexture, glassTexture, noiseTexture;

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (6 * sizeof(float)));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (8 * sizeof(float)));
    // enable the layout locations so they can be used by the vertex shader
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    // important: if you have more vertex arrays to draw, make sure you separately define them
    // with unique VAO, VBO, and EBO IDs, and follow the same process above to upload them to the GPU

    marbleTexture = gdevLoadTexture("marble.jpg", GL_REPEAT, true, true);
    if (! marbleTexture) return false;

    glassTexture = gdevLoadTexture("glass.jpg", GL_REPEAT, true, true);
    if (! glassTexture) return false;

    noiseTexture = gdevLoadTexture("glass.png", GL_REPEAT, true, true);
    if (! noiseTexture) return false;

    // load our shader program
    shader = gdevLoadShader("exer2.vs", "exer2.fs");
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
    glEnable(GL_CULL_FACE); 

    // using our shader program...
    glUseProgram(shader);
    // ... set the active texture...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, marbleTexture);

    glUniform1f(glGetUniformLocation(shader, "time"), glfwGetTime());

    glEnable(GL_DEPTH_TEST); // enable OpenGL's hidden surface removal

    glm::vec3 eyePosition = glm::vec3(4.0f, 2.0f, 0.0f);
    glm::vec3 targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 viewTransform;
    viewTransform = glm::lookAt(eyePosition, targetPosition, upVector);

    glm::mat4 matrix;
    glm::mat4 amog1;
    glm::mat4 amog2;
    glm::mat4 amog3;

    matrix = glm::perspective(glm::radians(60.0f),
                              (float) WINDOW_WIDTH / WINDOW_HEIGHT,
                              0.1f,
                              100.0f);
    
    float angle = glfwGetTime()*60;
    amog1 = glm::translate(matrix, glm::vec3(0.0f, 0.0f, -3.0f));
    amog1 = glm::rotate(amog1, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
    amog1 = glm::scale(amog1, glm::vec3(0.2f, 0.2f, 0.2f));
    amog1 = amog1 * viewTransform;
    
    glUniformMatrix4fv(glGetUniformLocation(shader, "matrix"), 1, GL_FALSE, glm::value_ptr(amog1));

    glBindVertexArray(vao);

    // set which texture object goes into which texture unit
    glActiveTexture(GL_TEXTURE0); // texture unit 0
    glBindTexture(GL_TEXTURE_2D, marbleTexture);
    glActiveTexture(GL_TEXTURE1); // texture unit 1
    glBindTexture(GL_TEXTURE_2D, glassTexture);
    glActiveTexture(GL_TEXTURE2); // texture unit 1
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    glActiveTexture(GL_TEXTURE0);

    // then connect each texture unit to a sampler2D in the fragment shader
    glUniform1i(glGetUniformLocation(shader, "marbleTexture"), 0);
    glUniform1i(glGetUniformLocation(shader, "glassTexture"), 1);
    glUniform1i(glGetUniformLocation(shader, "noiseTexture"), 2);

    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

    float angle2 = glfwGetTime()*120;
    amog2 = glm::translate(matrix, glm::vec3(-1.0f, 0.0f, -2.0f));
    amog2 = glm::rotate(amog2, glm::radians(angle2), glm::vec3(0.0f, 0.0f, 1.0f));
    amog2 = glm::scale(amog2, glm::vec3(1.0f, 1.0f, 1.0f));
    amog2 = amog2 * viewTransform;

    glUniformMatrix4fv(glGetUniformLocation(shader, "matrix"), 1, GL_FALSE, glm::value_ptr(amog2));
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

    float angle3 = glfwGetTime()*45;
    amog3 = glm::translate(matrix, glm::vec3(1.0f, 1.0f,-6.0f));
    amog3 = glm::rotate(amog3, glm::radians(-angle3), glm::vec3(0.1f, 1.0f, 0.0f));
    amog3 = glm::scale(amog3, glm::vec3(3.0f, 3.0f, 3.0f));
    amog3 = amog3 * viewTransform;

    glUniformMatrix4fv(glGetUniformLocation(shader, "matrix"), 1, GL_FALSE, glm::value_ptr(amog3));
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
}

/*****************************************************************************/

// handler called by GLFW when there is a keyboard event
void handleKeys(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    // pressing Esc closes the window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindow, GL_TRUE);

    // if (key == GLFW_KEY_W && action == GLFW_PRESS)
    //     ;
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

