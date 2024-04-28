/******************************************************************************
 * CONTROL MECHANICS:
 * W: Move camera UP
 * A: Move camera LEFT
 * S: Move camera DOWN
 * D: Move camera RIGHT
 * E: Move camera FORWARD
 * Q: Move camera DOWN
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
-0.1443,  0.5000, -0.2500,  1.0f,  0.3f,  0.3f, 0.00f, 0.16f, 0,//8
-0.2887,  0.5000,  0.0000,  1.0f,  0.3f,  0.3f, 0.00f, 0.33f, 0,//9
-0.2887,  0.3333, -0.1667,  1.0f,  0.3f,  0.3f, 1.00f, 1.00f, 0,//10
-0.1443,  0.5000,  0.2500,  1.0f,  0.3f,  0.3f, 0.00f, 0.50f, 0,//11
-0.2887,  0.3333,  0.1667,  1.0f,  0.3f,  0.3f, 1.00f, 0.00f, 0,//12
 0.1443,  0.5000,  0.2500,  1.0f,  0.3f,  0.3f, 0.00f, 0.66f, 0,//13
 0.0000,  0.3333,  0.3333,  1.0f,  0.3f,  0.3f, 1.00f, 1.00f, 0,//14
 0.2887,  0.5000,  0.0000,  1.0f,  0.3f,  0.3f, 0.00f, 0.84f, 0,//15
 0.2887,  0.3333,  0.1667,  1.0f,  0.3f,  0.3f, 1.00f, 0.00f, 0,//16
 0.1443,  0.5000, -0.2500,  1.0f,  0.3f,  0.3f, 0.00f, 1.00f, 0,//17
 0.2887,  0.3333, -0.1667,  1.0f,  0.3f,  0.3f, 1.00f, 1.00f, 0,//18
 0.0000,  0.6000,  0.0000,  1.0f,  0.3f,  0.3f, 1.00f, 0.50f, 0,//19
 0.0000,  0.0556,  0.3333,  0.0f,  1.0f,  1.0f, 0.33f, 0.00f, 1,//20
 0.0000, -0.0833, -0.3333,  0.7f,  0.2f,  0.2f, 0.33f, 0.00f, 0,//21
 0.2887,  0.0556,  0.1667,  0.0f,  1.0f,  1.0f, 0.66f, 0.00f, 1,//22
-0.2887, -0.0833, -0.1667,  0.7f,  0.2f,  0.2f, 0.33f, 0.00f, 0,//23
 0.0000,  0.3333,  0.3333,  1.0f,  1.0f,  1.0f, 0.00f, 0.66f, 1,//24 
 0.2887,  0.3333,  0.1667,  1.0f,  1.0f,  1.0f, 1.00f, 0.66f, 1,//25 
 0.0000,  0.0556,  0.3333,  1.0f,  1.0f,  1.0f, 0.00f, 0.33f, 1,//26
 0.2887,  0.0556,  0.1667,  1.0f,  1.0f,  1.0f, 0.00f, 0.00f, 1,//27
 0.0833,  0.3333,  0.4777,  1.0f,  1.0f,  1.0f, 0.33f, 0.66f, 1,//28
 0.3720,  0.3333,  0.3110,  1.0f,  1.0f,  1.0f, 0.66f, 0.66f, 1,//29
 0.0833,  0.0556,  0.4777,  0.0f,  1.0f,  1.0f, 0.33f, 0.33f, 1,//30
 0.3720,  0.0556,  0.3110,  0.0f,  1.0f,  1.0f, 0.66f, 0.33f, 1,//31
-0.0833,  0.3333, -0.4777,  0.7f,  0.2f,  0.2f, 0.33f, 0.66f, 0,//32
-0.3720,  0.3333, -0.3110,  0.7f,  0.2f,  0.2f, 0.66f, 0.66f, 0,//33
-0.0833, -0.0833, -0.4777,  0.7f,  0.2f,  0.2f, 0.33f, 0.33f, 0,//34
-0.3720, -0.0833, -0.3110,  0.7f,  0.2f,  0.2f, 0.66f, 0.33f, 0,//35
-0.2887,  0.3333, -0.1667,  0.7f,  0.2f,  0.2f, 0.66f, 1.00f, 0,//36 
 0.0000,  0.3333, -0.3333,  0.7f,  0.2f,  0.2f, 0.33f, 1.00f, 0,//37
 0.0000,  0.3333,  0.3333,  0.0f,  1.0f,  1.0f, 0.33f, 1.00f, 1,//38 
 0.2887,  0.3333,  0.1667,  0.0f,  1.0f,  1.0f, 0.66f, 1.00f, 1,//39
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
7,10,8,
10,12,9,
12,14,11,
14,16,13,
16,18,15,
18,7,17,

//TOP CURVE 2
9,8,10,
11,9,12,
13,11,14,
15,13,16,
17,15,18,
8,17,7,

//TOP CURVE 3
8,9,19,
9,11,19,
11,13,19,
13,15,19,
15,17,19,
17,8,19,

//VISOR
30,28,24,
30,24,26,

31,29,28,
31,28,30,

27,25,29,
27,29,31,

29,39,38,
29,38,28,

22,31,30,
22,30,20,

//BACKPACK
//21, 23, 32, 33, 34, 35, 36, 37
36,32,33,
36,37,32,

36,33,35,
36,35,23,

33,32,34,
33,34,35,

32,37,21,
32,21,34,

21,23,35,
21,35,34,

//BODY
1,2,7,
2,3,10,
3,4,12,
4,5,14,
5,6,16,
6,1,18,
7,2,10,
10,3,12,
12,4,14,
14,5,16,
16,6,18,
18,1,7,
};

float PI = glm::pi<float>();
glm::vec3 amogusPositions[] = {
    glm::vec3( 0, 0, 0), 
    glm::vec3( -1.0, -1.0, -1.0), 
    glm::vec3( 1.0, 1.0, 1.0),  
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
    shader = gdevLoadShader("exer3.vs", "exer3.fs");
    if (! shader)
        return false;

    return true;
}

glm::vec3 eyePosition = glm::vec3(0.0f, 0.0f, -5.0f);
glm::vec3 targetPosition = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

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

    glm::mat4 model;
    glm::mat4 projView;
    glm::mat4 normal;

    float angle = glfwGetTime()*60;
    glm::vec3 axes[] = {
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
    };
    
    glm::vec3 scales[] = {
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.7f, 0.7f, 0.7f),
        glm::vec3(0.9f, 0.9f, 0.9f),
    };

    glm::mat4 viewTransform = glm::lookAt(eyePosition, targetPosition + eyePosition, upVector);

    for(unsigned int i = 0; i < 3; i++) {

        projView = glm::perspective(glm::radians(60.0f),
                                  (float) WINDOW_WIDTH / WINDOW_HEIGHT,
                                  0.1f,
                                  100.0f);

        projView = projView * viewTransform;
    
        model = glm::translate(glm::mat4(1.0), amogusPositions[i]);
        model = glm::rotate(model, glm::radians(angle), axes[i]);
        model = glm::scale(model, scales[i]);
        

        
        glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader, "projView"), 1, GL_FALSE, glm::value_ptr(projView));
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

    }
    
    
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
    const float cameraSpeed = 0.05f; // adjust accordingly
    if (key == GLFW_KEY_W && action == GLFW_REPEAT) eyePosition.y += cameraSpeed;
    if (key == GLFW_KEY_S && action == GLFW_REPEAT) eyePosition.y -= cameraSpeed;
    if (key == GLFW_KEY_A && action == GLFW_REPEAT) eyePosition.x += cameraSpeed;
    if (key == GLFW_KEY_D && action == GLFW_REPEAT) eyePosition.x -= cameraSpeed;
    if (key == GLFW_KEY_E && action == GLFW_REPEAT) eyePosition.z += cameraSpeed;
    if (key == GLFW_KEY_Q && action == GLFW_REPEAT) eyePosition.z -= cameraSpeed;
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

