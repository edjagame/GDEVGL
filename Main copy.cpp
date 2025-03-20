/******************************************************************************
* CONTROLS
* 
* N -> Toggle normal map
* 
* CAMERA MANIPULATION (RELATIVE TO CAMERA SPACE)
* W -> Move forward
* S -> Move backward
* A -> Move left
* D -> Move right
* Q -> Move up
* E -> Move down
* Mouse - rotation
* 
*
* M -> Toggle between the light sources
* SPACE -> Make the current light emanate from camera position
* 
* LIGHT MOVEMENT (RELATIVE TO WORLD SPACE)
* J -> Move in the negative x direction
* L -> Move in the positive x direction
* I -> Move in the positive z direction
* K -> Move in the negative z direction
* U -> Move in the positive y direction
* O -> Move in the negative y direction
* 
* LIGHT ROTATION
* LEFT and RIGHT arrow keys -> controls the YAW
* UP and DOWN arrow keys -> controls the PITCH (Pitch is relative to the yaw angle)
* 
* LIGHT ATTENUATION
* MINUS -> increase attenuation (goes dark faster)
* PLUS -> decrease attenuation (goes dark slower)
* 
* SPOTLIGHT RADIUS
* [ and ] -> decrease or increase the spotlight outer radius
* Shift + [ and Shift + ] -> decrease or increase the spotlight inner radius
* 
*****************************************************************************/

#include <iostream>
#include <random>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <gdev.h>

// change this to your desired window attributes
#define WINDOW_WIDTH  1600
#define WINDOW_HEIGHT 900
#define WINDOW_TITLE  "Hello Transform (use WASDQE and arrow keys; 1 and 2 to select object)"
#define PI 3.14159265358979323846
GLFWwindow *pWindow;
float fov   =  75.0f;

////////////////////////////////////////////////////////////////////////////////////////
// VERTEX LOADING
// Make sure to add the necessary text files for vertex loading

//Vertex Struct
struct Vertex {
   float x, y, z; 
   float r, g, b, a;
   float nx, ny, nz;
   float u, v;
   float tx, ty, tz;
};

//Function to load vertices from a text file
std::vector<Vertex> loadVerts(const std::string& filePath) {
   std::vector<Vertex> data;
   std::ifstream file(filePath);
   std::string line;

   Vertex currentVertex;
   int stride = 0;

   while (std::getline(file, line)) {
    float value = std::stof(line);
    switch (stride) {
        case 0: currentVertex.x = value; break;
        case 1: currentVertex.y = value; break;
        case 2: currentVertex.z = value; break;
        case 3: currentVertex.r = value; break;
        case 4: currentVertex.g = value; break;
        case 5: currentVertex.b = value; break;
        case 6: currentVertex.a = value; break;
        case 7: currentVertex.nx = value; break;
        case 8: currentVertex.ny = value; break;
        case 9: currentVertex.nz = value; break;
        case 10: currentVertex.u = value; break;
        case 11: currentVertex.v = value; break;
        case 12: currentVertex.tx = value; break;
        case 13: currentVertex.ty = value; break;
        case 14: currentVertex.tz = value; break;
    }
       stride++;
       if (stride == 15) {
           data.push_back(currentVertex);
           stride = 0;
       }
   }

   return data;
}

//The actual vertices
std::vector<Vertex> tailsVertices = loadVerts("./TailsVerts.txt");
std::vector<Vertex> sonicVertices = loadVerts("./SonicVerts.txt");
std::vector<Vertex> knucklesVertices = loadVerts("./KnucklesVerts.txt");
std::vector<Vertex> chessVertices = loadVerts("./ChessVerts.txt");

// define OpenGL object IDs to represent the vertex array, shader program, and texture in the GPU
GLuint tailsVAO, tailsVBO, chessVAO, chessVBO, sonicVAO, sonicVBO, knucklesVAO, knucklesVBO;
GLuint tailsTex[3], sonicTex[3], chessTex[3], knucklesTex[3];
GLuint shader;

// variables controlling the object's position, rotation, and scaling
struct modelInstance
{
    float x         = 0.0f;
    float y         = 0.0f;
    float z         = 0.0f;  // ADDED FOR DEMO4B (to change the sprite's depth)
    float rotation  = 0.0f;
    float scaling   = 1.0f;

    //for the chess movement
    int   state     = 0   ;
    int   row       = 0   ;
    int   column    = 0   ;
    int   nextMove  = 0   ;
    float t = 0;
};

const float tileWidth = 38.0/7.0f;
const float smallAngle = 26.565f;
const float bigAngle = 63.435f;
const float originX = -16.5; 
const float originY = 0;
const float originZ = -16.5;
const float offsetX = -2.5;
const float offsetZ = -2.5;

std::vector<std::pair<int, int>> knightMoves = {
    {1, 2},
    {2, 1},
    {2, -1},
    {1, -2},
    {-1, -2},
    {-2, -1},
    {-2, 1},
    {-1, 2}
};
std::vector<float> knightAngles = {
    bigAngle,
    smallAngle,
    -smallAngle,
    -bigAngle,
    180+bigAngle,
    180+smallAngle,
    180-smallAngle,
    180-bigAngle
};

modelInstance tailsInstances[2] = {
    {   
        originX + offsetX + 0 * tileWidth,
        originY, 
        originZ + offsetZ + 0 * tileWidth, 
        0, 
        1.0,
        0,
        0, //row
        0 //col
    },
    {   
        originX + offsetX + 7 * tileWidth,
        originY, 
        originZ + offsetZ + 6 * tileWidth, 
        0, 
        1.0,
        0,
        7,
        6
    }
};
modelInstance sonicInstances[2] = {
    {
        originX + offsetX + 2 * tileWidth, 
        2.2, 
        originZ + offsetZ + 3 * tileWidth, 
        0, 
        1.0,
        0,
        3,
        2
    },
    {
        originX + offsetX + 4 * tileWidth, 
        2.2, 
        originZ + offsetZ + 5 * tileWidth, 
        0, 
        1.0,
        0,
        5,
        4
    }
};
modelInstance knucklesInstances[2] = {
    {
        originX + offsetX + 4 * tileWidth, 
        3.14, 
        originZ + offsetZ + 3 * tileWidth, 
        0, 
        0.7, 
        0, 
        3, 
        4
    },
    {
        originX + offsetX + 5 * tileWidth, 
        3.14, 
        originZ + offsetZ + 1 * tileWidth, 
        0,
        0.7, 
        0, 
        1,     
        5
    }
};
modelInstance chessInstances[1];

void setVAO(GLuint &VAO, GLuint &VBO, const std::vector<Vertex> vertices) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // on the VAO, register the current VBO with the following vertex attribute layout:
    // - the stride length of the vertex array is 15 floats (15 * sizeof(float))
    // - layout location 0 (position) is 3 floats and starts at the first float of the vertex array (offset 0)
    // - layout location 1 (color + opacity) is 4 floats and starts at the fourth float (offset 3 * sizeof(float))
    // - layout location 2 (texcoord) is 2 floats and starts at the eighth float (offset 7 * sizeof(float))
    // - layout location 3 (normal) is 3 floats and starts at the tenth float (offset 9 * sizeof(float))
    // - layout location 4 (tangent) is 3 floats and starts at the thirteenth float (offset 12 * sizeof(float))
    // Position attribute
       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, x)));
       glEnableVertexAttribArray(0);

       // Color attribute
       glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, r)));
       glEnableVertexAttribArray(1);

       // Normal attribute
       glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, nx)));
       glEnableVertexAttribArray(2);

       // Texture coordinates attribute
       glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, u)));
       glEnableVertexAttribArray(3);

       // Tangent coordinates attribute
       glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tx)));
       glEnableVertexAttribArray(3);


    // enable the layout locations so they can be used by the vertex shader
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    // Unbind VAO and VBO
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);
}

bool loadTexture(GLuint* texture, std::string name) {
   texture[0] = gdevLoadTexture((name + "Texture.png").c_str(), GL_REPEAT, true, true);
   texture[1] = gdevLoadTexture((name + "NormalMap.png").c_str(), GL_REPEAT, true, true);
   texture[2] = gdevLoadTexture((name + "SpecularMap.png").c_str(), GL_REPEAT, true, true);
   if (! texture[0] || ! texture[1] || ! texture[2])
       return false;
   return true;
}
// called by the main function to do initial setup, such as uploading vertex
// arrays, shader programs, etc.; returns true if successful, false otherwise

bool setup()
{
    // upload the TAILS model to the GPU (explanations omitted for brevity)
    setVAO(tailsVAO, tailsVBO, tailsVertices);
    setVAO(sonicVAO, sonicVBO, sonicVertices);
    setVAO(knucklesVAO, knucklesVBO, knucklesVertices);
    setVAO(chessVAO, chessVBO, chessVertices);

    // load our shader program
    shader = gdevLoadShader("main.vs", "main.fs");
    if (! shader)
        return false;
    
    glUseProgram(shader);

    // load our texture
   if (!loadTexture(sonicTex, "Sonic")) return false;
   if (!loadTexture(knucklesTex, "Knuckles")) return false;
   if (!loadTexture(tailsTex, "Tails")) return false;
   if (!loadTexture(chessTex, "Chess")) return false;

    glUniform1i(glGetUniformLocation(shader, "diffuseMap"), 0);
    glUniform1i(glGetUniformLocation(shader, "normalMap"),  1);
    glUniform1i(glGetUniformLocation(shader, "specularMap"), 2);

    // enable OpenGL blending so that texels with alpha values less than one are drawn transparent
    // (you can omit these lines if you don't use alpha)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // the next line will enable z-buffer depth testing to properly draw objects on top of each other
    // (but wait, will this actually work properly with this demo?...)
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    return true;
}

// Set up the initial camera vectors
glm::vec3 cameraPos   = glm::vec3(0.0f, 10.0f, 40.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, -1.0f, -3.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);


//LIGHTING VARIABLES
struct lightInstance {
    int type;
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 attenuation;
    glm::vec3 direction;
    float innerCutoff;
    float outerCutoff;
    glm::vec3 up = glm::vec3(1.0f);
};
lightInstance pointLight = {
    0,                              // Type
    glm::vec3(30.0f, 70.0f, 30.0f), // Position
    glm::vec3(1.0),                 // Color
    glm::vec3(1.0, 0.014, 0.00014), // Attenuation
    glm::vec3(1.0),                 // Direction
    0,                              // Inner Cutoff
    0                               // Outer Cutoff
};
lightInstance spotLight = {
    1,                              // Type
    glm::vec3(0.0f, 30.0f, 0.0f),   // Position
    glm::vec3(1.0f),                // Color
    glm::vec3(1.0, 0.014, 0.00014), // Attenuation
    glm::vec3(0.0f, -0.99f, 0.0f),  // Direction
    glm::cos(glm::radians(12.5f)),  // Inner Cutoff
    glm::cos(glm::radians(17.5f)),  // Outer Cutoff
};
std::vector<lightInstance> lights = {pointLight, spotLight};

bool useNormals = true, useNormalsPressed = false;
int currentLight = 1;
bool currentLightPressed = false;
bool makeFlashlight = false, makeFlashlightPressed = false;
bool keyJPressed = false, keyLPressed = false, keyIPressed = false, keyKPressed = false;
bool keyUPressed = false, keyOPressed = false;
bool keyLeftPressed = false, keyRightPressed = false, keyUpPressed = false, keyDownPressed = false;
bool keyMinusPressed = false, keyPlusPressed = false;
bool keyLeftBracketPressed = false, keyRightBracketPressed = false;
bool keyShiftLeftBracketPressed = false, keyShiftRightBracketPressed = false;

void movementControls(float deltaTime, std::vector<lightInstance> &lights) {

    float cameraSpeed = deltaTime * 20;
    
    // Scene Bounds
    const float MIN_Y = 3.0f;
    const float MAX_Y = 40.0f;
    const float MIN_XZ = -40.0f;
    const float MAX_XZ = 40.0f;

    // Move camera
    if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS) cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS) cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS) cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS) cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS) cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS) cameraPos -= cameraSpeed * cameraUp;

    cameraPos.y = glm::clamp(cameraPos.y, MIN_Y, MAX_Y);
    cameraPos.x = glm::clamp(cameraPos.x, MIN_XZ, MAX_XZ);
    cameraPos.z = glm::clamp(cameraPos.z, MIN_XZ, MAX_XZ);


    //Toggle Normal Map Usage
    if (glfwGetKey(pWindow, GLFW_KEY_N) == GLFW_PRESS && !useNormalsPressed){
        useNormals = !useNormals;
        useNormalsPressed = true;
        if(useNormals){
           std::cout << "Toggle Normal Map On\n";
        }
       else {
           std::cout << "Toggle Normal Map Off\n";
       } 
    }
    if (glfwGetKey(pWindow, GLFW_KEY_N) == GLFW_RELEASE){
        useNormalsPressed = false;
    }

    
    //Toggle Light
    if (glfwGetKey(pWindow, GLFW_KEY_M) == GLFW_PRESS && !currentLightPressed){
        currentLight = (currentLight + 1) % lights.size();
        currentLightPressed = true;
        if(currentLight == 0){
           std::cout << "Toggle Point Light\n";
        }
        else {
           std::cout << "Toggle Spot Light\n";
        }
    }
    if (glfwGetKey(pWindow, GLFW_KEY_M) == GLFW_RELEASE){
        currentLightPressed = false;
    }

    //Manipulate light parameters
    float lightSpeed = 30.0f; //Not to be confused with the speed of light

    //Light Position

    if (glfwGetKey(pWindow, GLFW_KEY_J) == GLFW_PRESS) {
       lights[currentLight].position.x -= lightSpeed * deltaTime;
       if (!keyJPressed) {
           std::cout << "Move Selected Light Source in the Negative X Direction\n"; 
           keyJPressed = true;
       }
    }
    if (glfwGetKey(pWindow, GLFW_KEY_J) == GLFW_RELEASE) {
       keyJPressed = false;
    }
    if (glfwGetKey(pWindow, GLFW_KEY_L) == GLFW_PRESS) {
       lights[currentLight].position.x += lightSpeed * deltaTime;
       if (!keyLPressed) {
           std::cout << "Move Selected Light Source in the Positive X Direction\n"; 
           keyLPressed = true;
       }
    }
    if (glfwGetKey(pWindow, GLFW_KEY_L) == GLFW_RELEASE) {
       keyLPressed = false;
    }
    if (glfwGetKey(pWindow, GLFW_KEY_I) == GLFW_PRESS) {
       lights[currentLight].position.z -= lightSpeed * deltaTime;
       if (!keyIPressed) {
           std::cout << "Move Selected Light Source in the Positive Z Direction\n"; 
           keyIPressed = true;
       }
    }
    if (glfwGetKey(pWindow, GLFW_KEY_I) == GLFW_RELEASE) {
       keyIPressed = false;
    }
    if (glfwGetKey(pWindow, GLFW_KEY_K) == GLFW_PRESS) {
       lights[currentLight].position.z += lightSpeed * deltaTime;
       if (!keyKPressed) {
           std::cout << "Move Selected Light Source in the Negative Z Direction\n"; 
           keyKPressed = true;
       }
    }
    if (glfwGetKey(pWindow, GLFW_KEY_K) == GLFW_RELEASE) {
       keyKPressed = false;
    }
    if (glfwGetKey(pWindow, GLFW_KEY_U) == GLFW_PRESS) {
       lights[currentLight].position.y += lightSpeed * deltaTime;
       if (!keyUPressed) {
           std::cout << "Move Selected Light Source in the Positive Y Direction\n"; 
           keyUPressed = true;
       }
    }
    if (glfwGetKey(pWindow, GLFW_KEY_U) == GLFW_RELEASE) {
       keyUPressed = false;
    }
    if (glfwGetKey(pWindow, GLFW_KEY_O) == GLFW_PRESS) {
       lights[currentLight].position.y -= lightSpeed * deltaTime;
       if (!keyOPressed) {
           std::cout << "Move Selected Light Source in the Negative Y Direction\n"; 
           keyOPressed = true;
       }
    }
    if (glfwGetKey(pWindow, GLFW_KEY_O) == GLFW_RELEASE) {
       keyOPressed = false;
    }

    const float LIGHT_MIN_Y = 3.0f;
    const float LIGHT_MAX_Y = 100.0f;
    const float LIGHT_MIN_XZ = -30.0f;
    const float LIGHT_MAX_XZ = 30.0f;

    lights[currentLight].position.y = glm::clamp(lights[currentLight].position.y, LIGHT_MIN_Y, LIGHT_MAX_Y);
    lights[currentLight].position.x = glm::clamp(lights[currentLight].position.x, LIGHT_MIN_XZ, LIGHT_MAX_XZ);
    lights[currentLight].position.z = glm::clamp(lights[currentLight].position.z, LIGHT_MIN_XZ, LIGHT_MAX_XZ);

    //Light rotation (for spotlights)
    float rotationSpeed = 100.0f;
    if (lights[currentLight].type == 1){

        if (glfwGetKey(pWindow, GLFW_KEY_LEFT) == GLFW_PRESS) {
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationSpeed * deltaTime), glm::vec3(0.0f, 1.0f, 0.0f));
            lights[currentLight].direction = glm::vec3(rotationMatrix * glm::vec4(lights[currentLight].direction, 0.0f));
            lights[currentLight].up = glm::vec3(rotationMatrix * glm::vec4(lights[currentLight].up, 0.0f));
            if (!keyLeftPressed) {
               std::cout << "Rotate Selected Light Source along the Yaw (Counterclockwise from above)\n";
               keyLeftPressed = true;
            }
        }
        if (glfwGetKey(pWindow, GLFW_KEY_LEFT) == GLFW_RELEASE) {
           keyLeftPressed = false;
        }
        if (glfwGetKey(pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-rotationSpeed * deltaTime), glm::vec3(0.0f, 1.0f, 0.0f));
            lights[currentLight].direction = glm::vec3(rotationMatrix * glm::vec4(lights[currentLight].direction, 0.0f));
            lights[currentLight].up = glm::vec3(rotationMatrix * glm::vec4(lights[currentLight].up, 0.0f));
            if (!keyRightPressed) {
               std::cout << "Rotate Selected Light Source along the Yaw (Clockwise from above)\n";
               keyRightPressed = true;
            }
        }
        if (glfwGetKey(pWindow, GLFW_KEY_RIGHT) == GLFW_RELEASE) {
           keyRightPressed = false;
        }
        
        glm::vec3 pitchAxis = glm::normalize(glm::cross(lights[currentLight].direction, lights[currentLight].up));
        if (glfwGetKey(pWindow, GLFW_KEY_UP) == GLFW_PRESS && lights[currentLight].direction.y <= -0.0001f) {
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationSpeed * deltaTime), pitchAxis);
            lights[currentLight].direction = glm::vec3(rotationMatrix * glm::vec4(lights[currentLight].direction, 0.0f));
            if (!keyUpPressed) {
               std::cout << "Rotate Selected Light Source along the Pitch in Positive direction\n";
               keyUpPressed = true;
            }
        }
        if (glfwGetKey(pWindow, GLFW_KEY_UP) == GLFW_RELEASE) {
           keyUpPressed = false;
        }
        if (glfwGetKey(pWindow, GLFW_KEY_DOWN) == GLFW_PRESS && lights[currentLight].direction.y >= -0.9999f) {
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-rotationSpeed * deltaTime), pitchAxis);
            lights[currentLight].direction = glm::vec3(rotationMatrix * glm::vec4(lights[currentLight].direction, 0.0f));
            if (!keyDownPressed) {
               std::cout << "Rotate Selected Light Source along the Pitch in Negative direction\n";
               keyDownPressed = true;
            }
        }
        if (glfwGetKey(pWindow, GLFW_KEY_DOWN) == GLFW_RELEASE) {
           keyDownPressed = false;
        }

        lights[currentLight].direction = glm::normalize(lights[currentLight].direction);  

        if (!(glfwGetKey(pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) && glfwGetKey(pWindow, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS) {
           lights[currentLight].outerCutoff += 0.001f;
           if (!keyLeftBracketPressed) {
               std::cout << "Decrease the spotlight outer radius\n"; //check 
               keyLeftBracketPressed = true;
           }
        }
        if (glfwGetKey(pWindow, GLFW_KEY_LEFT_BRACKET) == GLFW_RELEASE) {
           keyLeftBracketPressed = false;
        }
        if (!(glfwGetKey(pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) && glfwGetKey(pWindow, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS) {
           lights[currentLight].outerCutoff -= 0.001f;
           if (!keyRightBracketPressed) {
               std::cout << "Increase the spotlight outer radius\n"; //check 
               keyRightBracketPressed = true;
           }
        }
        if (glfwGetKey(pWindow, GLFW_KEY_RIGHT_BRACKET) == GLFW_RELEASE) {
           keyRightBracketPressed = false;
        }
        if (glfwGetKey(pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(pWindow, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS) {
           lights[currentLight].innerCutoff += 0.01f;
           if (!keyShiftLeftBracketPressed) {
               std::cout << "Decrease the spotlight inner radius\n"; //check 
               keyShiftLeftBracketPressed = true;
           }
        }
        if (glfwGetKey(pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(pWindow, GLFW_KEY_LEFT_BRACKET) == GLFW_RELEASE) {
           keyShiftLeftBracketPressed = false;
        }
        if (glfwGetKey(pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(pWindow, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS) {
           lights[currentLight].innerCutoff -= 0.01f;
           if (!keyShiftRightBracketPressed) {
               std::cout << "Increase the spotlight inner radius\n"; //check 
               keyShiftRightBracketPressed = true;
           }
        }
        if (glfwGetKey(pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(pWindow, GLFW_KEY_RIGHT_BRACKET) == GLFW_RELEASE) {
           keyShiftRightBracketPressed = false;
        }

        lights[currentLight].innerCutoff = glm::clamp(lights[currentLight].innerCutoff, lights[currentLight].outerCutoff, 0.99f);
        lights[currentLight].outerCutoff = glm::clamp(lights[currentLight].outerCutoff, 0.01f, lights[currentLight].innerCutoff);

        if (glfwGetKey(pWindow, GLFW_KEY_EQUAL) == GLFW_PRESS) {
            lights[currentLight].attenuation.y += 0.01f * deltaTime;
            if (!keyPlusPressed) {
               std::cout << "Increase attenuation \n"; //check 
               keyPlusPressed = true;
            }
        }
        if (glfwGetKey(pWindow, GLFW_KEY_EQUAL) == GLFW_RELEASE) {
           keyPlusPressed = false;
        }
        if (glfwGetKey(pWindow, GLFW_KEY_MINUS) == GLFW_PRESS) {
            lights[currentLight].attenuation.y -= 0.01f * deltaTime;
            if (!keyMinusPressed) {
               std::cout << "Decrease attenuation \n"; // check
               keyMinusPressed = true;
            }
        }
        if (glfwGetKey(pWindow, GLFW_KEY_MINUS) == GLFW_RELEASE) {
           keyMinusPressed = false;
        }
        
       lights[currentLight].attenuation.y = glm::clamp(lights[currentLight].attenuation.y, lights[currentLight].attenuation.z, lights[currentLight].attenuation.x);
       
   }

   if (glfwGetKey(pWindow, GLFW_KEY_SPACE) == GLFW_PRESS && !makeFlashlightPressed){
       makeFlashlight = !makeFlashlight;
       makeFlashlightPressed = true;
       if (makeFlashlight) {
           std::cout << "Make the current light emanate from camera position\n"; 
       } 
       else {
           std::cout << "Make the current light stop emanating from camera position\n"; 
       }
   }
   if (glfwGetKey(pWindow, GLFW_KEY_SPACE) == GLFW_RELEASE){
       makeFlashlightPressed = false;
   }
}

int current = 0;
double previousTime = 0.0;


// called by the main function to do rendering per frame
void render()
{
    lightInstance spotlight = lights[1];
    // randomizes the random seed
    srand(glfwGetTime()*1000);

    // find the elapsed time since the last frame
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - previousTime;
    previousTime = currentTime;
    
    movementControls(deltaTime, lights);
    glUniform1i(glGetUniformLocation(shader, "useNormals"), useNormals);

    if (makeFlashlight) {
           spotlight.position = cameraPos;
           spotlight.direction = cameraFront;
    }

    // clear the whole frame
    glClearColor(0.0f, 0.0f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // using our shader program...
    glUseProgram(shader);
    
    // ... set up the projection matrix...
    glm::mat4 projectionTransform;
    projectionTransform = glm::perspective(glm::radians(fov),                   // fov
                                           (float) WINDOW_WIDTH / WINDOW_HEIGHT,  // aspect ratio
                                           0.1f,                                  // near plane
                                           200.0f);                               // far plane
    glUniformMatrix4fv(glGetUniformLocation(shader, "projectionTransform"),
                       1, GL_FALSE, glm::value_ptr(projectionTransform));

    // ... set up the view matrix...
    glm::mat4 viewTransform;
    viewTransform = glm::lookAt(cameraPos,   // eye position
                                cameraPos + cameraFront,   // center position
                                cameraUp);  // up vector

    glUniformMatrix4fv(glGetUniformLocation(shader, "viewTransform"),
                       1, GL_FALSE, glm::value_ptr(viewTransform));



    // Tails Matrix Transformations
    // Time 0: idle time
    // Time 1: rotating time
    // Time 2: Movement time
    std::vector<std::pair<int, int>> tailsValidMoves;
    std::vector<float> tailsValidAngles;
    int newRow, newColumn;
    float tailsTime0 = 0.3f, tailsTime1 = 1.0f, tailsTime2 = 2.0f;
    
    // Main loop that calculates the transformations and passes them to the vertex array
    for (int i = 0; i < 2; i++) {
        tailsInstances[i].t += deltaTime;
        tailsValidMoves.clear();
        tailsValidAngles.clear();

        for(unsigned int j=0; j<knightMoves.size(); j++) {
            newRow = tailsInstances[i].row + knightMoves[j].first;
            newColumn = tailsInstances[i].column + knightMoves[j].second;
            if(newRow >= 0 && newRow < 8 && newColumn >= 0 && newColumn < 8) {
                tailsValidMoves.push_back({knightMoves[j].first, knightMoves[j].second});
                tailsValidAngles.push_back(knightAngles[j]);
            }
        }

        // Calculate the next move in the idle state
        if (tailsInstances[i].state == 0){
            if (tailsInstances[i].t >= tailsTime0) {
                tailsInstances[i].nextMove = rand() % tailsValidMoves.size();
                tailsInstances[i].state = 1;
                tailsInstances[i].t = 0;
            }
        }

        // ... set up the model matrix...
        glm::mat4 modelTransform = glm::mat4(1.0f);  // set to identity first
        
        // Set the initial position of the model
        modelTransform = glm::translate(modelTransform,
                                    glm::vec3(tailsInstances[i].x, tailsInstances[i].y, tailsInstances[i].z)); // translate xyz
        
        // Translate it to a different square in the movement phase, then bring it back to idle state
        if (tailsInstances[i].state == 2) {
            float timeScaling = tailsInstances[i].t / tailsTime2;
            float translateX = tailsValidMoves[tailsInstances[i].nextMove].second * tileWidth * timeScaling;
            float translateY = 5 * sin(PI * timeScaling);
            float translateZ = tailsValidMoves[tailsInstances[i].nextMove].first * tileWidth * timeScaling;
            modelTransform = glm::translate(modelTransform, glm::vec3(translateX, translateY, translateZ));
            if (tailsInstances[i].t >= tailsTime2) {
                tailsInstances[i].state = 0;
                tailsInstances[i].t = 0;
                tailsInstances[i].row += tailsValidMoves[tailsInstances[i].nextMove].first;
                tailsInstances[i].column += tailsValidMoves[tailsInstances[i].nextMove].second;
                tailsInstances[i].x = originX + offsetX + tailsInstances[i].column * tileWidth;
                tailsInstances[i].z = originZ + offsetZ + tailsInstances[i].row * tileWidth;
            } 
        }

        // Set the initial rotation
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(tailsInstances[i].rotation),
                                    glm::vec3(0.0f, 1.0f, 0.0f));

        // Rotate it according to the next move in the rotating phase
        if (tailsInstances[i].state == 1) {
            float turnAngle = tailsValidAngles[tailsInstances[i].nextMove] - tailsInstances[i].rotation - 90.0f;
            if(fabs(turnAngle) > 180) {
                turnAngle = 360 - fabs(turnAngle);
            }
            float timeScaling = (tailsInstances[i].t / tailsTime1);
            modelTransform = glm::rotate(modelTransform, 
                                         glm::radians(turnAngle * timeScaling),
                                         glm::vec3(0.0f, 1.0f, 0.0f));

            if (tailsInstances[i].t >= tailsTime1) {
                tailsInstances[i].state = 2;
                tailsInstances[i].rotation = tailsValidAngles[tailsInstances[i].nextMove] - 90;
                tailsInstances[i].t = 0;    
            }
        }

        // Rotation during the movement phase
        if(tailsInstances[i].state == 2) {
            modelTransform = glm::rotate(modelTransform,
                                        glm::radians(0.0f * tailsInstances[i].t),
                                        glm::vec3(0.0f, 0.0f, 1.0f));
        }

        

        //Scale (in case its needed)
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(tailsInstances[i].scaling, tailsInstances[i].scaling, tailsInstances[i].scaling));   // scale x and y
        

        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active texture...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tailsTex[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tailsTex[1]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, tailsTex[2]);
        
        glUniform1i(glGetUniformLocation(shader, "diffuseMap"), 0);
        glUniform1i(glGetUniformLocation(shader, "normalMap"), 1);
        glUniform1i(glGetUniformLocation(shader, "specularMap"), 2);


        // ... then draw our triangles
        glBindVertexArray(tailsVAO);
        glDrawArrays(GL_TRIANGLES, 0, tailsVertices.size());
    }


    // Sonic Matrix Transformations
    std::vector<std::pair<int, int>> sonicValidMoves;
    std::vector<float> sonicValidAngles;
    int newRow0, newColumn0;
    float sonicTime0 = 0.3f; //sonic idle time;
    float sonicTime1 = 1.0f; //sonic rotat time;
    float sonicTime2 = 2.0f; //sonic move time;
    for (int i = 0; i < 2; i++) {
        
        sonicInstances[i].t += deltaTime;
        sonicValidMoves.clear();
        sonicValidAngles.clear();

        // Calculate the next move
        for(unsigned int j=1; j<8; j++){
            newRow0 = sonicInstances[i].row + j;
            newColumn0 = sonicInstances[i].column + j;
            if (newRow0 >= 0 && newRow0 < 8 && newColumn0 >= 0 && newColumn0 < 8) {
                sonicValidMoves.push_back({j, j});
                sonicValidAngles.push_back(45.0f);
            }
            
            newRow0 = sonicInstances[i].row - j;
            newColumn0 = sonicInstances[i].column + j;
            if (newRow0 >= 0 && newRow0 < 8 && newColumn0 >= 0 && newColumn0 < 8) {
                sonicValidMoves.push_back({-j, j});
                sonicValidAngles.push_back(135.0f);
            }
            
            newRow0 = sonicInstances[i].row - j;
            newColumn0 = sonicInstances[i].column - j;
            if (newRow0 >= 0 && newRow0 < 8 && newColumn0 >= 0 && newColumn0 < 8) {
                sonicValidMoves.push_back({-j, -j});
                sonicValidAngles.push_back(225.0f);
            }
            
            newRow0 = sonicInstances[i].row + j;
            newColumn0 = sonicInstances[i].column - j;
            if (newRow0 >= 0 && newRow0 < 8 && newColumn0 >= 0 && newColumn0 < 8) {
                sonicValidMoves.push_back({j, -j});
                sonicValidAngles.push_back(315.0f);
            }
        }


        // Calculate the next move in the idle state
        if (sonicInstances[i].state == 0){
            if (sonicInstances[i].t >= sonicTime0) {
                sonicInstances[i].nextMove = rand() % sonicValidMoves.size();
                sonicInstances[i].state = 1;
                sonicInstances[i].t = 0;
            }
        }

        // ... set up the model matrix...
        glm::mat4 modelTransform = glm::mat4(1.0f);  // set to identity first

        // Set the initial position of the model
        modelTransform = glm::translate(modelTransform,
                                    glm::vec3(sonicInstances[i].x, sonicInstances[i].y, sonicInstances[i].z)); // translate xyz
            
        // Translate it to a different square in the movement phase, then bring it back to idle state
        if (sonicInstances[i].state == 2) {

            if (sonicInstances[i].t >= sonicTime2 * 0.5) {
                modelTransform = glm::translate(modelTransform, 
                                                glm::vec3((sonicValidMoves[sonicInstances[i].nextMove].second * tileWidth * 0.5) * pow(sonicTime2 * 0.4, -3) * pow(sonicInstances[i].t - sonicTime2 * 0.5, 3), 
                                                        0, 
                                                        (sonicValidMoves[sonicInstances[i].nextMove].first * tileWidth * 0.5) * pow(sonicTime2 * 0.4, -3) * pow(sonicInstances[i].t - sonicTime2 * 0.5, 3)));
            }
            if (sonicInstances[i].t >= sonicTime2) {
                sonicInstances[i].state = 0;
                sonicInstances[i].t = 0;
                sonicInstances[i].row += sonicValidMoves[sonicInstances[i].nextMove].first;
                sonicInstances[i].column += sonicValidMoves[sonicInstances[i].nextMove].second;
                sonicInstances[i].x = originX + offsetX + sonicInstances[i].column * tileWidth;
                sonicInstances[i].z = originZ + offsetZ + sonicInstances[i].row * tileWidth;
            } 
        }
        
        // Set the initial rotation
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(sonicInstances[i].rotation),
                                    glm::vec3(0.0f, 1.0f, 0.0f));
        
        // Rotate it according to the next move in the rotating phase
        if (sonicInstances[i].state == 1) {

            float turnAngle = sonicValidAngles[sonicInstances[i].nextMove] - sonicInstances[i].rotation - 90.0f;
            if(fabs(turnAngle) > 180) {
                turnAngle = 360 - fabs(turnAngle);
            }
            modelTransform = glm::rotate(modelTransform, 
                                         glm::radians(turnAngle * (sonicInstances[i].t / sonicTime1)),
                                         glm::vec3(0.0f, 1.0f, 0.0f));

            if (sonicInstances[i].t >= sonicTime1) {
                sonicInstances[i].state = 2;
                sonicInstances[i].rotation = sonicValidAngles[sonicInstances[i].nextMove] - 90;
                sonicInstances[i].t = 0;    
            }
        }

        // Rotation during the movement phase
        if(sonicInstances[i].state == 2) {
            modelTransform = glm::rotate(modelTransform,
                                        glm::radians(-3600.0f * float(pow(sonicTime2,-3)) * float(pow(sonicInstances[i].t,3))),
                                        glm::vec3(0.0f, 0.0f, 1.0f));
        }

        // Scale (in case its needed)
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(sonicInstances[i].scaling, sonicInstances[i].scaling, sonicInstances[i].scaling));   // scale x and y



        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"), 1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active texture...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sonicTex[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, sonicTex[1]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, sonicTex[2]);
 
        glUniform1i(glGetUniformLocation(shader, "diffuseMap"), 0);
        glUniform1i(glGetUniformLocation(shader, "normalMap"), 1);
        glUniform1i(glGetUniformLocation(shader, "specularMap"), 2);


        // ... then draw our triangles
        glBindVertexArray(sonicVAO);
        glDrawArrays(GL_TRIANGLES, 0, sonicVertices.size());
    }



    // Knuckles Matrix Transformations
    std::vector<std::pair<int, int>> knucklesValidMoves;
    std::vector<float> knucklesValidAngles;
    int newRow1, newColumn1;
    for (int i = 0; i < 2; i++) {
        
        knucklesInstances[i].t += deltaTime;
        knucklesValidMoves.clear();
        knucklesValidAngles.clear();

        // Calculate the next move
        for(unsigned int j=1; j<8; j++){
            newRow1 = knucklesInstances[i].row + j;
            newColumn1 = knucklesInstances[i].column;
            if (newRow1 >= 0 && newRow1 < 8 && newColumn1 >= 0 && newColumn1 < 8) {
                knucklesValidMoves.push_back({j, 0});
                knucklesValidAngles.push_back(0.0f);
            }
            
            newRow1 = knucklesInstances[i].row - j;
            newColumn1 = knucklesInstances[i].column;
            if (newRow1 >= 0 && newRow1 < 8 && newColumn1 >= 0 && newColumn1 < 8) {
                knucklesValidMoves.push_back({-j, 0});
                knucklesValidAngles.push_back(180.0f);
            }

            
            newRow1 = knucklesInstances[i].row;
            newColumn1 = knucklesInstances[i].column + j;
            if (newRow1 >= 0 && newRow1 < 8 && newColumn1 >= 0 && newColumn1 < 8) {
                knucklesValidMoves.push_back({0, j});
                knucklesValidAngles.push_back(90.0f);
            }
        
            newRow1 = knucklesInstances[i].row;
            newColumn1 = knucklesInstances[i].column - j;
            if (newRow1 >= 0 && newRow1 < 8 && newColumn1 >= 0 && newColumn1 < 8) {
                knucklesValidMoves.push_back({0, -j});
                knucklesValidAngles.push_back(270.0f);
            }
        }

        // Calculate the next move in the idle state
        if (knucklesInstances[i].state == 0){
            if (knucklesInstances[i].t >= 0.3f) {
                knucklesInstances[i].nextMove = rand() % knucklesValidMoves.size();
                knucklesInstances[i].state = 1;
                knucklesInstances[i].t = 0;
            }
        }

        // ... set up the model matrix...
        glm::mat4 modelTransform = glm::mat4(1.0f);  // set to identity first

        // Set the initial position of the model
        modelTransform = glm::translate(modelTransform,
                                    glm::vec3(knucklesInstances[i].x, knucklesInstances[i].y, knucklesInstances[i].z)); // translate xyz

        // Translate it to a different square in the movement phase, then bring it back to idle state
        if (knucklesInstances[i].state == 2) {
            float time1 = 1.0f;
            
            modelTransform = glm::translate(modelTransform, 
                                            glm::vec3(knucklesValidMoves[knucklesInstances[i].nextMove].second * tileWidth * (knucklesInstances[i].t / time1), 
                                                      fabs(5*sin((abs(knucklesValidMoves[knucklesInstances[i].nextMove].second)+abs(knucklesValidMoves[knucklesInstances[i].nextMove].first)) * PI * knucklesInstances[i].t * (1 / time1))), 
                                                      knucklesValidMoves[knucklesInstances[i].nextMove].first * tileWidth * (knucklesInstances[i].t / time1)));
            if (knucklesInstances[i].t >= time1) {
                knucklesInstances[i].state = 0;
                knucklesInstances[i].t = 0;
                knucklesInstances[i].row += knucklesValidMoves[knucklesInstances[i].nextMove].first;
                knucklesInstances[i].column += knucklesValidMoves[knucklesInstances[i].nextMove].second;
                knucklesInstances[i].x = originX + offsetX + knucklesInstances[i].column * tileWidth;
                knucklesInstances[i].z = originZ + offsetZ + knucklesInstances[i].row * tileWidth;
            } 
        }

        // Set the initial rotation
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(knucklesInstances[i].rotation),
                                    glm::vec3(0.0f, 1.0f, 0.0f));
        
        // Rotate it according to the next move in the rotating phase
        if (knucklesInstances[i].state == 1) {
            
            float turnAngle = knucklesValidAngles[knucklesInstances[i].nextMove] - knucklesInstances[i].rotation - 90.0f;
            if(fabs(turnAngle) > 180) {
                turnAngle = 360 - fabs(turnAngle);
            }
            float time2 = 1.0f * fabs(turnAngle) / 180.0;
            modelTransform = glm::rotate(modelTransform, 
                                         glm::radians(turnAngle * (knucklesInstances[i].t / time2)),
                                         glm::vec3(0.0f, 1.0f, 0.0f));

            if (knucklesInstances[i].t >= time2) {
                knucklesInstances[i].state = 2;
                knucklesInstances[i].rotation = knucklesValidAngles[knucklesInstances[i].nextMove] - 90;
                knucklesInstances[i].t = 0;    
            }
        }

        // Rotation during the movement phase
        if(knucklesInstances[i].state == 2) {
            modelTransform = glm::rotate(modelTransform,
                                        glm::radians((abs(knucklesValidMoves[knucklesInstances[i].nextMove].second)+abs(knucklesValidMoves[knucklesInstances[i].nextMove].first)) * -360.0f * knucklesInstances[i].t),
                                        glm::vec3(0.0f, 0.0f, 1.0f));
        }

        // Scale (in case its needed)
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(knucklesInstances[i].scaling, knucklesInstances[i].scaling, knucklesInstances[i].scaling));   // scale x and y

        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"), 1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active texture...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, knucklesTex[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, knucklesTex[1]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, knucklesTex[2]);
 
        glUniform1i(glGetUniformLocation(shader, "diffuseMap"), 0);
        glUniform1i(glGetUniformLocation(shader, "normalMap"), 1);
        glUniform1i(glGetUniformLocation(shader, "specularMap"), 2);
        // ... then draw our triangles
        glBindVertexArray(knucklesVAO);
        glDrawArrays(GL_TRIANGLES, 0, knucklesVertices.size());
    }


    // Chess Matrix Transformations
    for (int i = 0; i < 1; i++)
    {
        // ... set up the model matrix...
        glm::mat4 modelTransform = glm::mat4(1.0f);  // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(chessInstances[i].x, chessInstances[i].y, chessInstances[i].z)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                     chessInstances[i].rotation,
                                     glm::vec3(0.0f, 0.0f, 1.0f));                                    // rotate around z
        modelTransform = glm::rotate(modelTransform,
                                     0.0f,//float(10*glfwGetTime()),
                                     glm::vec3(0.0f, 1.0f, 0.0f));
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(chessInstances[i].scaling * 0.4f, chessInstances[i].scaling * 0.4f, 1.0f * 0.4f));   // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active texture...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, chessTex[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, chessTex[1]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, chessTex[2]);
        
        glUniform1i(glGetUniformLocation(shader, "diffuseMap"), 0);
        glUniform1i(glGetUniformLocation(shader, "normalMap"), 1);
        glUniform1i(glGetUniformLocation(shader, "specularMap"), 2);

        // ... then draw our triangles
        glBindVertexArray(chessVAO);
        glDrawArrays(GL_TRIANGLES, 0, chessVertices.size());
    }
    
    
    //LIGHTING UNIFORMS
    glUniform3fv(glGetUniformLocation(shader, "pointLightPos"), 1, glm::value_ptr(lights[0].position));
    glUniform3fv(glGetUniformLocation(shader, "pointLightColor"), 1, glm::value_ptr(lights[0].color));
    glUniform3fv(glGetUniformLocation(shader, "pointLightAttenuation"), 1, glm::value_ptr(lights[0].attenuation));
    glUniform3fv(glGetUniformLocation(shader, "spotLightPos"), 1, glm::value_ptr(lights[1].position));
    glUniform3fv(glGetUniformLocation(shader, "spotLightColor"), 1, glm::value_ptr(lights[1].color));
    glUniform3fv(glGetUniformLocation(shader, "spotLightDir"), 1, glm::value_ptr(lights[1].direction));
    glUniform3fv(glGetUniformLocation(shader, "spotLightAttenuation"), 1, glm::value_ptr(lights[1].attenuation));
    glUniform1f(glGetUniformLocation(shader, "spotLightInnerCutoff"), lights[1].innerCutoff);
    glUniform1f(glGetUniformLocation(shader, "spotLightOuterCutoff"), lights[1].outerCutoff);
    glUniform3fv(glGetUniformLocation(shader, "eyePosition"), 1, glm::value_ptr(cameraPos));

    
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

bool firstMouse = true;
float yaw   = -90.0f;	
float pitch =  -atan(cameraFront.y/cameraFront.z)*180/PI;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;

void mouse_callback(GLFWwindow* pWindow, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
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
    glfwSetCursorPosCallback(pWindow, mouse_callback);
    glfwSetFramebufferSizeCallback(pWindow, handleResize);
    glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

