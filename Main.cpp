/******************************************************************************
* CONTROLS
* 
* NEW TO EXERCISE 3
* P -> Toggle shadows
* ; -> Make shadows softer
* SHIFT + ; -> Make shadows sharper
* 
* N -> Toggle normal map
* ENTER -> Toggle model movement

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
* SPACE -> Make the current light emanate from camera position (flashlight)
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
* SHADOWS
* P -> Toggle shadows
* ; -> Make shadows softer
* ; -> Make shadows sharper
*
*****************************************************************************/
/**********
 * Files to Include:
 * 
 * Main.cpp
 * main.fs
 * main.vs
 * mainS.vs
 * mainS.fs
 * (Chess | Knuckles | Sonic | Tails)Texture.png
 * (Chess | Knuckles | Sonic | Tails)NormalMap.png
 * (Chess | Knuckles | Sonic | Tails)SpecularMap.png
 * (Chess | Knuckles | Sonic | Tails)Verts.txt
 * 
 * Auxilliary:
 * test
 * test.cmd
 * triangleMaker.py
 * (Chess | Knuckles | Sonic | Tails)Object.obj
 */
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
std::vector<Vertex> buildingsVertices = loadVerts("./BuildingsVerts.txt");
std::vector<Vertex> mirrorVertices = loadVerts("./MirrorVerts.txt");
std::vector<Vertex> vertices;

// define OpenGL object IDs to represent the vertex array, shader program, and texture in the GPU
GLuint tailsTex[4], sonicTex[4], chessTex[4], knucklesTex[4], buildingsTex[5], mirrorTex[4];
GLuint celShadingMap;
GLuint shader;
GLuint vao, vbo;

// variables controlling the object's position, rotation, and scaling

const float tileWidth = 95.5/7.0;

const float originX = -47.0; 
const float originY = 3.0;
const float originZ = -47.0;
const float offsetX = 0;
const float offsetZ = 0;

enum object {TAILS, SONIC, KNUCKLES, CHESS, BUILDINGS, MIRROR};
enum state {IDLE, ROTATING, MOVING, STATIC};
struct modelInstance
{
    object obj;
    glm::vec3 position;
    float rotation  = 0.0f;
    float scaling   = 1.0f;

    //for movement
    state state = IDLE;
    glm::vec2 coords;
    glm::vec2 nextCoords;
    float t = 0;

    glm::mat4 prevModelTransform = glm::mat4(1.0f);
    glm::mat4 prevModelTransformReflection = glm::mat4(1.0f);

    std::vector<glm::mat4> afterimageTransforms;
    float afterimageTimer = 0.0f;
};
std::vector<modelInstance> models = {
    {   
        TAILS,
        {originX + 0 * tileWidth, originY, originZ + 0 * tileWidth}, 
        0, 
        1.0,
        IDLE,
        {0, 0}, 
    },
    {   
        TAILS,
        {originX + 7 * tileWidth, originY, originZ + 6 * tileWidth}, 
        0, 
        1.0,
        IDLE,
        {6, 7}
    },
    {
        SONIC,
        {originX + 2 * tileWidth, originY, originZ + 3 * tileWidth},
        0,
        1.0,
        IDLE,
        {3, 2}
    },
    {
        SONIC,
        {originX + 4 * tileWidth, originY, originZ + 5 * tileWidth},
        0,
        1.0,
        IDLE,
        {5, 4}
    },
    {
        KNUCKLES,
        {originX + 4 * tileWidth, originY, originZ + 3 * tileWidth},
        0,
        0.7,
        IDLE,
        {3, 4}
    },
    {
        KNUCKLES,
        {originX + 5 * tileWidth, originY, originZ + 1 * tileWidth},
        0,
        0.7,
        IDLE,
        {1, 5}
    },
    {
        CHESS, 
        {0.0f, 0.0f, 0.0f}, 
        0.0f, 
        1.0f, 
        STATIC, 
    },
    {
        BUILDINGS,
        {0.0f, 0.0f, 0.0f},
        0.0f,
        1.0f,
        STATIC,
    },
    {
        MIRROR,
        {0.0f, 0.0f, 0.0f},
        0.0f,
        1.0f,
        STATIC,
    }
};

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
    glm::vec3(0.0f, 250.0f, 00.0f), // Position
    glm::vec3(1.0),                 // Color
    glm::vec3(1.0, 0.00014, 0.00000014), // Attenuation
    glm::vec3(1.0),                 // Direction
    0,                              // Inner Cutoff
    0                               // Outer Cutoff
};
lightInstance spotLight = {
    1,                              // Type
    glm::vec3(0.0f, 30.0f, 0.0f),   // Position
    glm::vec3(1.0f),                // Color
    glm::vec3(1.0, 0.014, 0.00014), // Attenuation
    glm::vec3(0.0f, -0.99f, -0.01f),  // Direction
    glm::cos(glm::radians(12.5f)),  // Inner Cutoff
    glm::cos(glm::radians(17.5f)),  // Outer Cutoff
};
std::vector<lightInstance> lights = {pointLight, spotLight};

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

///////////////////////////////////////////////////////////////////////////////
// SKYBOX CODE
GLuint skyboxVAO, skyboxVBO;
GLuint skyboxTexture;
GLuint skyboxShader;

float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

bool setupSkybox() {
    // Create VAO and VBO for skybox
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Load skybox shader
    skyboxShader = gdevLoadShader("skyboxShader.vs", "skyboxShader.fs");
    if (!skyboxShader)
        return false;

    // Load skybox textures
    std::vector<std::string> faces = {
        "skyboxRight.jpg",
        "skyboxLeft.jpg",
        "skyboxBottom.jpg",
        "skyboxTop.jpg",
        "skyboxFront.jpg", 
        "skyboxBack.jpg"   
    };

    glGenTextures(1, &skyboxTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

    int width, height, nrChannels;
    unsigned char *data;  
    for(unsigned int i = 0; i < faces.size(); i++)
    {
        data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
            0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
        );
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  
    return true;
}
///////////////////////////////////////////////////////////////////////////////
// CUSTOM FRAMEBUFFER CODE

float screenQuadVertices[] =   // vertices for a fullscreen quadrilateral
{
    // position   texcoords
    -1.0f, -1.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 1.0f, 0.0f,
     1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, 0.0f, 1.0f
};

GLuint fbo;         // framebuffer object
GLuint fboTexture;  // texture for the framebuffer
GLuint fboDepthTexture; // depth texture for the framebuffer
GLuint fboVelocityTexture; // velocity texture for the framebuffer
GLuint fboVao;      // vertex array object for the fullscreen quadrilateral
GLuint fboVbo;      // vertex buffer object for the fullscreen quadrilateral
GLuint fboShader;   // shader for the fullscreen quadrilateral

bool setupFbo()
{
    // create the framebuffer object
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // attach a texture object to the framebuffer
    glGenTextures(1, &fboTexture);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);

    // attach a depth texture object to the framebuffer
    glGenTextures(1, &fboDepthTexture);
    glBindTexture(GL_TEXTURE_2D, fboDepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fboDepthTexture, 0);

    // attach a velocity texture object to the framebuffer
    glGenTextures(1, &fboVelocityTexture);
    glBindTexture(GL_TEXTURE_2D, fboVelocityTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RG, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Attach as another color attachment
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, fboVelocityTexture, 0);

    GLuint drawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, drawBuffers);

    // check if we did everything right
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Could not create custom framebuffer.\n";
        return false;
    }

    // create the VAO and VBO for the fullscreen quadrilateral
    glGenVertexArrays(1, &fboVao);
    glGenBuffers(1, &fboVbo);
    glBindVertexArray(fboVao);
    glBindBuffer(GL_ARRAY_BUFFER, fboVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuadVertices), screenQuadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // load the shader program for the fullscreen quadrilateral
    fboShader = gdevLoadShader("fbo.vs", "fbo.fs");
    if (! fboShader)
        return false;

    // set the framebuffer back to the default onscreen buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void renderFbo()
{

    // set the framebuffer back to the default onscreen buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // always draw to the whole window
    int width, height;
    glfwGetFramebufferSize(pWindow, &width, &height); // Get current size
    glViewport(0, 0, width, height); // Use current size

    // clear the onscreen buffer
    // (the clear color does not matter because we're filling the window with our framebuffer texture)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // using the framebuffer shader...
    glUseProgram(fboShader);

    // ... set the active texture to our framebuffer texture...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glUniform1i(glGetUniformLocation(fboShader, "colorTexture"), 0);
    
    // ... set the active texture to our framebuffer velocity texture...
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, fboVelocityTexture);
    glUniform1i(glGetUniformLocation(fboShader, "velocityTexture"), 1);

    glUniform2fv(glGetUniformLocation(fboShader, "screenSize"), 1, glm::value_ptr(glm::vec2(width, height)));
    // ... then draw our fullscreen quadrilateral
    glBindVertexArray(fboVao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(screenQuadVertices) / (4 * sizeof(float)));
}

// CUSTOM FRAMEBUFFER CODE
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// SHADOW MAPPING CODE
void drawModel (modelInstance& model, float deltaTime, GLuint shader, bool mainPass);

#define SHADOW_SIZE 2048
float shadowSharpness = 500.0f;
GLuint shadowMapFbo;      // shadow map framebuffer object
GLuint shadowMapTexture;  // shadow map texture
GLuint shadowMapShader;   // shadow map shader


bool setupShadowMap()
{

    // create the FBO for rendering shadows
    glGenFramebuffers(1, &shadowMapFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFbo);

    // attach a texture object to the framebuffer
    glGenTextures(1, &shadowMapTexture);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_SIZE, SHADOW_SIZE,
                 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTexture, 0);

    // check if we did everything right
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Could not create custom framebuffer.\n";
        return false;
    }

    if (!setupSkybox()) {
        return false;
    }
    
    // load the shader program for drawing the shadow map
    shadowMapShader = gdevLoadShader("mainShadow.vs", "mainShadow.fs");
    if (! shadowMapShader)
        return false;

    
    // set the framebuffer back to the default onscreen buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

glm::mat4 renderShadowMap(lightInstance& light, float deltaTime)
{
    // use the shadow framebuffer for drawing the shadow map
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFbo);

    // the viewport should be the size of the shadow map
    glViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE);

    // clear the shadow map
    // (we don't have a color buffer attachment, so no need to clear that)
    glClear(GL_DEPTH_BUFFER_BIT);

    // using the shadow map shader...
    glUseProgram(shadowMapShader);

    // ... set up the light space matrix...
    // (note that if you use a spot light, the FOV and the center position
    // vector should be set to the spot light's outer cone angle times 2
    // and the spot light's focus point, respectively)
    glm::mat4 lightTransform;
    lightTransform = glm::perspective(2 * acos(light.outerCutoff),       // fov
                                      1.0f,                      // aspect ratio
                                      0.1f,                      // near plane
                                      200.0f);                   // far plane
    lightTransform *= glm::lookAt(light.position,                 // eye position
                                  light.position + light.direction,   // center position
                                  glm::vec3(0.0f, 1.0f, 0.0f));  // up vector
    
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "lightTransform"),
                       1, GL_FALSE, glm::value_ptr(lightTransform));

    // draw the scene
    for (modelInstance& model : models) {
        if (model.state == STATIC) continue; 
        drawModel(model, deltaTime, shadowMapShader, false);
    }
    
    // set the framebuffer back to the default onscreen buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // before drawing the final scene, we need to set drawing to the whole window
    int width, height;
    glfwGetFramebufferSize(pWindow, &width, &height);
    glViewport(0, 0, width, height);

    // we will need the light transformation matrix again in the main rendering code
    return lightTransform;
}

// SHADOW MAPPING CODE
///////////////////////////////////////////////////////////////////////////////

bool setup()
{
    vertices.insert(vertices.end(), tailsVertices.begin(), tailsVertices.end());
    vertices.insert(vertices.end(), sonicVertices.begin(), sonicVertices.end());
    vertices.insert(vertices.end(), knucklesVertices.begin(), knucklesVertices.end());
    vertices.insert(vertices.end(), chessVertices.begin(), chessVertices.end());
    vertices.insert(vertices.end(), buildingsVertices.begin(), buildingsVertices.end());
    vertices.insert(vertices.end(), mirrorVertices.begin(), mirrorVertices.end());
    // upload the TAILS model to the GPU (explanations omitted for brevity)

    setVAO(vao, vbo, vertices);
    
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
    if (!loadTexture(buildingsTex, "Buildings")) return false;
    if (!loadTexture(mirrorTex, "Mirror")) return false;
    buildingsTex[4] = gdevLoadTexture("BuildingsReflectionMap.png", GL_REPEAT, true, true); // buildings normal map
    celShadingMap = gdevLoadTexture("celShading.png", GL_CLAMP_TO_EDGE, true, true);  // usually in tex unit 4, after diffuse, normal, specular, and shadow

    // enable OpenGL blending so that texels with alpha values less than one are drawn transparent
    // (you can omit these lines if you don't use alpha)
    glDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // the next line will enable z-buffer depth testing to properly draw objects on top of each other
    // (but wait, will this actually work properly with this demo?...)
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    ///////////////////////////////////////////////////////////////////////////
    // setup the custom framebuffer
    if (! setupFbo())
        return false;
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // setup shadow rendering
    if (! setupShadowMap())
        return false;
    ///////////////////////////////////////////////////////////////////////////

    // UNIFORMS
    glUniform2i(glGetUniformLocation(shader, "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);

    return true;
}

// Set up the initial camera vectors
glm::vec3 cameraPos   = glm::vec3(0.0f, 10.0f, 40.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, -1.0f, -3.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

bool useNormals = true;
bool useNormalsPressed = false;
int currentLight = 1;
bool currentLightPressed = false;
bool flashlightMode = false;
bool flashlightModePressed = false;
bool enableShadows = true;
bool enableShadowsPressed = false;
bool moveModels = true;
bool moveModelsPressed = false;

 void movementControls(float deltaTime, std::vector<lightInstance> &lights) {
 
     float cameraSpeed = deltaTime * 60;
     
     // Scene Bounds
     const float MIN_Y = 3.0f;
     const float MAX_Y = 200.0f;
     const float MIN_XZ = -70.0f;
     const float MAX_XZ = 70.0f;
 
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
     }
     if (glfwGetKey(pWindow, GLFW_KEY_N) == GLFW_RELEASE){
         useNormalsPressed = false;
     }
 
     
     //Toggle Light
     if (glfwGetKey(pWindow, GLFW_KEY_M) == GLFW_PRESS && !currentLightPressed){
         currentLight = (currentLight + 1) % lights.size();
         currentLightPressed = true;
     }
     if (glfwGetKey(pWindow, GLFW_KEY_M) == GLFW_RELEASE){
         currentLightPressed = false;
     }
 
     //Manipulate light parameters
     float lightSpeed = 30.0f; //Not to be confused with the speed of light
 
     //Light Position
     if (glfwGetKey(pWindow, GLFW_KEY_J) == GLFW_PRESS) lights[currentLight].position.x -= lightSpeed * deltaTime;
     if (glfwGetKey(pWindow, GLFW_KEY_L) == GLFW_PRESS) lights[currentLight].position.x += lightSpeed * deltaTime;
     if (glfwGetKey(pWindow, GLFW_KEY_I) == GLFW_PRESS) lights[currentLight].position.z -= lightSpeed * deltaTime;
     if (glfwGetKey(pWindow, GLFW_KEY_K) == GLFW_PRESS) lights[currentLight].position.z += lightSpeed * deltaTime;  
     if (glfwGetKey(pWindow, GLFW_KEY_U) == GLFW_PRESS) lights[currentLight].position.y += lightSpeed * deltaTime;
     if (glfwGetKey(pWindow, GLFW_KEY_O) == GLFW_PRESS) lights[currentLight].position.y -= lightSpeed * deltaTime;
 
     const float LIGHT_MIN_Y = 3.0f;
     const float LIGHT_MAX_Y = 100.0f;
     const float LIGHT_MIN_XZ = -70.0f;
     const float LIGHT_MAX_XZ = 70.0f;
 
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
         }
         if (glfwGetKey(pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) {
             glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-rotationSpeed * deltaTime), glm::vec3(0.0f, 1.0f, 0.0f));
             lights[currentLight].direction = glm::vec3(rotationMatrix * glm::vec4(lights[currentLight].direction, 0.0f));
             lights[currentLight].up = glm::vec3(rotationMatrix * glm::vec4(lights[currentLight].up, 0.0f));
         }
         
         glm::vec3 pitchAxis = glm::normalize(glm::cross(lights[currentLight].direction, lights[currentLight].up));
         if (glfwGetKey(pWindow, GLFW_KEY_UP) == GLFW_PRESS && lights[currentLight].direction.y <= -0.0001f) {
             glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationSpeed * deltaTime), pitchAxis);
             lights[currentLight].direction = glm::vec3(rotationMatrix * glm::vec4(lights[currentLight].direction, 0.0f));
         }
         if (glfwGetKey(pWindow, GLFW_KEY_DOWN) == GLFW_PRESS && lights[currentLight].direction.y >= -0.9999f) {
             glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-rotationSpeed * deltaTime), pitchAxis);
             lights[currentLight].direction = glm::vec3(rotationMatrix * glm::vec4(lights[currentLight].direction, 0.0f));
         }
 
         lights[currentLight].direction = glm::normalize(lights[currentLight].direction);
 
         
 
         if (!(glfwGetKey(pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) && glfwGetKey(pWindow, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS) lights[currentLight].outerCutoff += 0.001f;
         if (!(glfwGetKey(pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) && glfwGetKey(pWindow, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS) lights[currentLight].outerCutoff -= 0.001f;
         if (glfwGetKey(pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(pWindow, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS) lights[currentLight].innerCutoff += 0.01f;
         if (glfwGetKey(pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(pWindow, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS) lights[currentLight].innerCutoff -= 0.01f;
         lights[currentLight].innerCutoff = glm::clamp(lights[currentLight].innerCutoff, lights[currentLight].outerCutoff, 0.99f);
         lights[currentLight].outerCutoff = glm::clamp(lights[currentLight].outerCutoff, 0.01f, lights[currentLight].innerCutoff);
 
         if (glfwGetKey(pWindow, GLFW_KEY_EQUAL) == GLFW_PRESS) {
             lights[currentLight].attenuation.y += 0.01f * deltaTime;
         }
         if (glfwGetKey(pWindow, GLFW_KEY_MINUS) == GLFW_PRESS) {
             lights[currentLight].attenuation.y -= 0.01f * deltaTime;
         }
         
        lights[currentLight].attenuation.y = glm::clamp(lights[currentLight].attenuation.y, lights[currentLight].attenuation.z, lights[currentLight].attenuation.x);
        
    }

    if (glfwGetKey(pWindow, GLFW_KEY_SPACE) == GLFW_PRESS && !flashlightModePressed){
        flashlightMode = !flashlightMode;
        flashlightModePressed = true;
    }
    if (glfwGetKey(pWindow, GLFW_KEY_SPACE) == GLFW_RELEASE){
        flashlightModePressed = false;
    }

    if (glfwGetKey(pWindow, GLFW_KEY_P) == GLFW_PRESS && !enableShadowsPressed){
        enableShadows = !enableShadows;
        enableShadowsPressed = true;
    }
    if (glfwGetKey(pWindow, GLFW_KEY_P) == GLFW_RELEASE){
        enableShadowsPressed = false;
    }


    if (glfwGetKey(pWindow, GLFW_KEY_SEMICOLON) == GLFW_PRESS && !(glfwGetKey(pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)) {
        shadowSharpness += 500.0f * deltaTime;
    }
    if (glfwGetKey(pWindow, GLFW_KEY_SEMICOLON) == GLFW_PRESS && (glfwGetKey(pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)) {
        shadowSharpness -= 500.0f * deltaTime;
    }
    shadowSharpness = glm::clamp(shadowSharpness, 0.00f, 10000.0f);


    if (glfwGetKey(pWindow, GLFW_KEY_ENTER) == GLFW_PRESS && !moveModelsPressed){
        moveModels = !moveModels;
        moveModelsPressed = true;
    }
    if (glfwGetKey(pWindow, GLFW_KEY_ENTER) == GLFW_RELEASE){
        moveModelsPressed = false;
    }
 }

glm::vec2 nextKnightMove(int currentRow, int currentCol) {
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
    
    std::vector<glm::vec2> validMoves;
    for (unsigned int i = 0; i < knightMoves.size(); i++) {
        int newRow = currentRow + knightMoves[i].first;
        int newCol = currentCol + knightMoves[i].second;
        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
            validMoves.push_back({newRow, newCol});
        }
    }

    return validMoves[rand() % validMoves.size()];
}

glm::vec2 nextBishopMove(int currentRow, int currentCol) {
    std::vector<glm::vec2> validMoves;
    for (int i = 1; i < 8; i++) {
        if (currentRow + i < 8 && currentCol + i < 8) validMoves.push_back({currentRow + i, currentCol + i});
        if (currentRow + i < 8 && currentCol - i >= 0) validMoves.push_back({currentRow + i, currentCol - i});
        if (currentRow - i >= 0 && currentCol + i < 8) validMoves.push_back({currentRow - i, currentCol + i});
        if (currentRow - i >= 0 && currentCol - i >= 0) validMoves.push_back({currentRow - i, currentCol - i});
    }
    return validMoves[rand() % validMoves.size()];
}

glm::vec2 nextRookMove(int currentRow, int currentCol) {
    std::vector<glm::vec2> validMoves;
    for (int i = 0; i < 8; i++) {
        if (i != currentCol) validMoves.push_back({currentRow, i});
        if (i != currentRow) validMoves.push_back({i, currentCol});
    }
    return validMoves[rand() % validMoves.size()];
}

object currentObj = TAILS;
glm::mat4 calculateModelTransform(modelInstance& instance, float deltaTime, bool enableShadows = true) {
    if (moveModels)
        instance.t += deltaTime;

    float randomOffset = 0.5f * (rand() % 10) / 10.0f;
    const float idleTime = 0.3f;
    const float rotationTime = 0.5f;
    const float moveTime = 1.0f;

    // Set to identity matrix at initialization
    glm::mat4 modelTransform = glm::mat4(1.0f);
    if (instance.state == IDLE && instance.obj != CHESS) {
        if (instance.t >= idleTime + randomOffset && instance.obj == currentObj) {
            instance.state = ROTATING;
            instance.t = 0;
            if (instance.obj == TAILS) instance.nextCoords = nextKnightMove(instance.coords.x, instance.coords.y);
            else if (instance.obj == SONIC) instance.nextCoords = nextBishopMove(instance.coords.x, instance.coords.y);
            else if (instance.obj == KNUCKLES) instance.nextCoords = nextRookMove(instance.coords.x, instance.coords.y);
        }
    }

    modelTransform = glm::translate(modelTransform,
                                    glm::vec3(instance.position.x, instance.position.y, instance.position.z)); // translate xyz
    
    //IDLE MOVEMENT
    if (instance.state == IDLE && instance.obj != CHESS) {
        if (instance.obj == TAILS) {
            modelTransform = glm::translate(modelTransform,
                                            glm::vec3(0.0f, 0.2f * sin(PI * instance.t), 0.0f));
        }
        else if (instance.obj == SONIC) {
            modelTransform = glm::translate(modelTransform,
                                            glm::vec3(0.0f, 0.2f * sin(PI * 2 * instance.t), 0.0f));
        }
        else if (instance.obj == KNUCKLES) {
            modelTransform = glm::translate(modelTransform,
                                            glm::vec3(0.0f, 0.2f * sin(PI * 0.5 * instance.t), 0.0f));
        }
    }

    if (instance.state == MOVING) {
        glm::vec3 newPos;
        float timeScaling = (instance.t / moveTime);
        if (instance.obj == TAILS) {
            newPos.x = originX + instance.nextCoords.y * tileWidth;
            newPos.y = originY + tileWidth * sin(PI * timeScaling);
            newPos.z = originZ + instance.nextCoords.x * tileWidth;
        }
        else if (instance.obj == SONIC) {
            newPos.x = originX + instance.nextCoords.y * tileWidth;
            newPos.y = originY;
            newPos.z = originZ + instance.nextCoords.x * tileWidth;
        }
        else if (instance.obj == KNUCKLES) {
            newPos.x = originX + instance.nextCoords.y * tileWidth;
            newPos.y = originY;
            newPos.z = originZ + instance.nextCoords.x * tileWidth;
        }
        modelTransform = glm::translate(modelTransform,
                                        (newPos - instance.position) * timeScaling);
        if (instance.t >= moveTime) {
            instance.state = IDLE;
            instance.position = newPos;
            instance.coords = instance.nextCoords;
            instance.t = 0;
            currentObj = static_cast<object>((currentObj + 1) % 3);
        }
    }
    
    modelTransform = glm::rotate(modelTransform, 
                                    glm::radians(instance.rotation),
                                    glm::vec3(0.0f, 1.0f, 0.0f));
    float angle;
    if (instance.state == ROTATING) {
        angle = glm::degrees(atan2(instance.nextCoords.y - instance.coords.y, instance.nextCoords.x - instance.coords.x));
        float turnAngle = angle - instance.rotation - 90.0f;
        if(fabs(turnAngle) > 180) {
            turnAngle = 360 - fabs(turnAngle);
        }
        float timeScaling = (instance.t / rotationTime);
        modelTransform = glm::rotate(modelTransform, 
                                        glm::radians(turnAngle * timeScaling),
                                        glm::vec3(0.0f, 1.0f, 0.0f));
        
        if (instance.t >= rotationTime) {
            instance.state = MOVING;
            instance.rotation = angle - 90.0f;
            instance.t = 0;    
        }
    }
    modelTransform = glm::scale(modelTransform,
                                glm::vec3(instance.scaling)); // scale uniformly
    if (instance.state == MOVING) {
        if (instance.obj == SONIC) {
            modelTransform = glm::rotate(modelTransform, 
                                        glm::radians(-360.0f * instance.t),
                                        glm::vec3(0.0f, 0.0f, 1.0f));
        }
        else if (instance.obj == KNUCKLES) {
            modelTransform = glm::rotate(modelTransform, 
                                        glm::radians(360.0f * instance.t),
                                        glm::vec3(0.0f, 1.0f, 0.0f));
        }
    }

    if (instance.obj == MIRROR) {
        modelTransform = glm::rotate(modelTransform, 
                                        glm::radians(90.0f),
                                        glm::vec3(0.0f, 1.0f, 0.0f));
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(150.0f, 0.0f, 0.0f)); // translate xyz
    }
    // Scale back to original size using the inverse of the scaling matrix

    return modelTransform;
}
void drawModel(modelInstance& model, float deltaTime, GLuint shader, bool mainPass) {

    glUniformMatrix4fv(glGetUniformLocation(shader, "prevModelTransform"),
                    1, GL_FALSE, glm::value_ptr(model.prevModelTransform));

    glm::mat4 modelTransform = calculateModelTransform(model, deltaTime);
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));
    
    // ... set the active texture...
    glActiveTexture(GL_TEXTURE0);
    switch (model.obj) {
        case TAILS:
            glBindTexture(GL_TEXTURE_2D, tailsTex[0]);
            break;
        case SONIC:
            glBindTexture(GL_TEXTURE_2D, sonicTex[0]);
            break;
        case KNUCKLES:
            glBindTexture(GL_TEXTURE_2D, knucklesTex[0]);
            break;
        case CHESS:
            glBindTexture(GL_TEXTURE_2D, chessTex[0]);
            break;
        case BUILDINGS:
            glBindTexture(GL_TEXTURE_2D, buildingsTex[0]);
            break;
        case MIRROR:
            glBindTexture(GL_TEXTURE_2D, mirrorTex[0]);
            break;
    }
    glActiveTexture(GL_TEXTURE1);
    switch (model.obj) {
        case TAILS:
            glBindTexture(GL_TEXTURE_2D, tailsTex[1]);
            break;
        case SONIC:
            glBindTexture(GL_TEXTURE_2D, sonicTex[1]);
            break;
        case KNUCKLES:
            glBindTexture(GL_TEXTURE_2D, knucklesTex[1]);
            break;
        case CHESS:
            glBindTexture(GL_TEXTURE_2D, chessTex[1]);
            break;
        case BUILDINGS:
            glBindTexture(GL_TEXTURE_2D, buildingsTex[1]);
            break;
        case MIRROR:
            glBindTexture(GL_TEXTURE_2D, mirrorTex[1]);
            break;
    }
    glActiveTexture(GL_TEXTURE2);
    switch (model.obj) {
        case TAILS:
            glBindTexture(GL_TEXTURE_2D, tailsTex[2]);
            break;
        case SONIC:
            glBindTexture(GL_TEXTURE_2D, sonicTex[2]);
            break;
        case KNUCKLES:
            glBindTexture(GL_TEXTURE_2D, knucklesTex[2]);
            break;
        case CHESS:
            glBindTexture(GL_TEXTURE_2D, chessTex[2]);
            break;
        case BUILDINGS:
            glBindTexture(GL_TEXTURE_2D, buildingsTex[2]);
            break;
        case MIRROR:
            glBindTexture(GL_TEXTURE_2D, mirrorTex[2]);
            break;
    }

    if (enableShadows) {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    }
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, celShadingMap);
    // ... then draw our triangles
    glBindVertexArray(vao);

    // object-specific uniforms
    glUniform1i(glGetUniformLocation(shader, "useAlpha"), false);
    switch (model.obj) {
        case TAILS:
            glUniform1i(glGetUniformLocation(shader, "useCelShading"), true);
            glUniform1i(glGetUniformLocation(shader, "useReflection"), false);
            break;
        case SONIC:
            glUniform1i(glGetUniformLocation(shader, "useCelShading"), true);
            glUniform1i(glGetUniformLocation(shader, "useReflection"), false);
            break;
        case KNUCKLES:
            glUniform1i(glGetUniformLocation(shader, "useCelShading"), true);
            glUniform1i(glGetUniformLocation(shader, "useReflection"), false);
            break;
        case CHESS:
            glUniform1i(glGetUniformLocation(shader, "useCelShading"), false);
            glUniform1i(glGetUniformLocation(shader, "useReflection"), false);
            break;
        case BUILDINGS:
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, buildingsTex[4]);
            glActiveTexture(GL_TEXTURE6);
            glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
            glUniform1i(glGetUniformLocation(shader, "useCelShading"), false);
            glUniform1i(glGetUniformLocation(shader, "useReflection"), true);
            break;
        case MIRROR:
            glUniform1i(glGetUniformLocation(shader, "useCelShading"), false);
            glUniform1i(glGetUniformLocation(shader, "useReflection"), false);
            glUniform1i(glGetUniformLocation(shader, "useAlpha"), true);
            break;
    }

    glm::mat4 reflectionMatrix = glm::mat4(1.0f);
    reflectionMatrix = glm::scale(reflectionMatrix, glm::vec3(1.0f, 1.0f, -1.0f));
    reflectionMatrix = glm::translate(reflectionMatrix, glm::vec3(0.0f, 0.0f, 300.0f));
    
    glm::mat4 modelTransformReflection = reflectionMatrix * modelTransform;

    if (mainPass) {
        // Draw reflected scene
        glCullFace(GL_FRONT);
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                            1, GL_FALSE, glm::value_ptr(modelTransformReflection));
        glUniformMatrix4fv(glGetUniformLocation(shader, "prevModelTransform"),
                            1, GL_FALSE, glm::value_ptr(model.prevModelTransformReflection));

        switch (model.obj) {
            case TAILS:
                    glDrawArrays(GL_TRIANGLES, 0, tailsVertices.size());
                    break;
            case SONIC:
                    glDrawArrays(GL_TRIANGLES, tailsVertices.size(), sonicVertices.size());
                    break;
            case KNUCKLES:
                    glDrawArrays(GL_TRIANGLES, tailsVertices.size() + sonicVertices.size(), knucklesVertices.size());
                    break;
            case CHESS:
                    glDrawArrays(GL_TRIANGLES, tailsVertices.size() + sonicVertices.size() + knucklesVertices.size(), chessVertices.size());
                    break;
            case BUILDINGS:
                glDrawArrays(GL_TRIANGLES, tailsVertices.size() + sonicVertices.size() + knucklesVertices.size() + chessVertices.size(), buildingsVertices.size());
                break;
        }
    }

     // Draw normal scene
    glCullFace(GL_BACK);
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));
    glUniformMatrix4fv(glGetUniformLocation(shader, "prevModelTransform"),
                        1, GL_FALSE, glm::value_ptr(model.prevModelTransform));
    switch (model.obj) {
        case TAILS:
                glDrawArrays(GL_TRIANGLES, 0, tailsVertices.size());
                break;
        case SONIC:
                glDrawArrays(GL_TRIANGLES, tailsVertices.size(), sonicVertices.size());
                break;
        case KNUCKLES:
                glDrawArrays(GL_TRIANGLES, tailsVertices.size() + sonicVertices.size(), knucklesVertices.size());
                break;
        case CHESS:
                glDrawArrays(GL_TRIANGLES, tailsVertices.size() + sonicVertices.size() + knucklesVertices.size(), chessVertices.size());
                break;
        case BUILDINGS:
                glDrawArrays(GL_TRIANGLES, tailsVertices.size() + sonicVertices.size() + knucklesVertices.size() + chessVertices.size(), buildingsVertices.size());
                break;
        case MIRROR:
                glEnable(GL_BLEND);
                glDrawArrays(GL_TRIANGLES, tailsVertices.size() + sonicVertices.size() + knucklesVertices.size() + chessVertices.size() + buildingsVertices.size(), mirrorVertices.size());
                glDisable(GL_BLEND);
                break;
    }

    
    model.prevModelTransform = modelTransform;
    model.prevModelTransformReflection = modelTransformReflection;
}
// called by the main function to do rendering per frame
int current = 0;
double previousTime = 0.0;
glm::mat4 prevViewTransform = glm::mat4(1.0f);
glm::mat4 prevProjTransform = glm::mat4(1.0f);
glm::mat4 viewTransform = glm::mat4(1.0f);
glm::mat4 projectionTransform = glm::mat4(1.0f);

float afterimageInterval = 0.1f;
int maxAfterimages = 10;

void render()
{
    // randomizes the random seed
    srand(glfwGetTime()*1000);

    // find the elapsed time since the last frame
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - previousTime;
    previousTime = currentTime;
    
    movementControls(deltaTime, lights);
    
    
    if (flashlightMode) {
           lights[1].position = cameraPos;
           lights[1].direction = cameraFront;
    }

    // // clear the whole frame
    // glClearColor(0.98f, 1.0f, 0.95f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ... set up the projection matrix...
    projectionTransform = glm::perspective(glm::radians(fov),                   // fov
                                            (float) WINDOW_WIDTH / WINDOW_HEIGHT,  // aspect ratio
                                            0.1f,                                  // near plane
                                            400.0f);                               // far plane
    // ... set up the view matrix...
    viewTransform = glm::lookAt(cameraPos,   // eye position
                                cameraPos + cameraFront,   // center position
                                cameraUp);  // up vector

    
    ///////////////////////////////////////////////////////////////////////////
    // draw the shadow map
    glm::mat4 lightTransform;
    if (enableShadows) lightTransform = renderShadowMap(lights[1], deltaTime);
    
    ///////////////////////////////////////////////////////////////////////////
    
    ///////////////////////////////////////////////////////////////////////////
    // use the custom framebuffer for subsequent rendering commands
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // make sure to render to the entire window
    // (note that we use the hardcoded window size here, since the custom
    // framebuffer was initialized using this size; if you want your program
    // to use the real resolution of the window, you would have to write code
    // to recreate the custom framebuffer texture every time the window is
    // resized)
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);
    ///////////////////////////////////////////////////////////////////////////

    // clear the whole frame
    glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // using our shader program...
    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projectionTransform"),
                        1, GL_FALSE, glm::value_ptr(projectionTransform));
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewTransform"),
                        1, GL_FALSE, glm::value_ptr(viewTransform));
    glUniformMatrix4fv(glGetUniformLocation(shader, "prevViewTransform"),
                        1, GL_FALSE, glm::value_ptr(prevViewTransform));
    glUniformMatrix4fv(glGetUniformLocation(shader, "prevProjTransform"),
                        1, GL_FALSE, glm::value_ptr(prevProjTransform));
    

    //SHADOW UNIFORMS
    glUniform1i(glGetUniformLocation(shader, "enableShadows"), enableShadows);
    if (enableShadows) {
        glUniformMatrix4fv(glGetUniformLocation(shader, "lightTransform"),
                       1, GL_FALSE, glm::value_ptr(lightTransform));
    }
    glUniform1f(glGetUniformLocation(shader, "shadowSharpness"), shadowSharpness);

    //TEXTURE UNIFORMS
    glUniform1i(glGetUniformLocation(shader, "useNormals"), useNormals);
    glUniform1i(glGetUniformLocation(shader, "diffuseMap"), 0);
    glUniform1i(glGetUniformLocation(shader, "normalMap"),  1);
    glUniform1i(glGetUniformLocation(shader, "specularMap"), 2);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
    glUniform1i(glGetUniformLocation(shader, "celShadingMap"), 4);
    glUniform1i(glGetUniformLocation(shader, "reflectionMap"), 5);
    glUniform1i(glGetUniformLocation(shader, "skyboxMap"), 6);

    //LIGHT UNIFORMS
    glUniform1i(glGetUniformLocation(shader, "flashlightMode"), flashlightMode);
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

    
    // draw the models
    for (modelInstance& model : models) {
        // Draw the main model
        glUniform1i(glGetUniformLocation(shader, "isAfterimage"), false); 
        drawModel(model, deltaTime, shader, true); 

        // Afterimage Spawning Logic
        if (model.state == MOVING) {
            model.afterimageTimer += deltaTime;
            if (model.afterimageTimer >= afterimageInterval) {
                model.afterimageTransforms.push_back(model.prevModelTransform);
                if (model.afterimageTransforms.size() > maxAfterimages) {
                    model.afterimageTransforms.erase(model.afterimageTransforms.begin());
                }
                model.afterimageTimer -= afterimageInterval; 
            }
        } else {
            if (model.afterimageTransforms.size() > 0) {
                model.afterimageTimer += deltaTime;
                if (model.afterimageTimer >= afterimageInterval) {
                    model.afterimageTransforms.erase(model.afterimageTransforms.begin());
                    model.afterimageTimer -= afterimageInterval; 
                }
            } else {
                model.afterimageTimer = 0.0f; 
            }
        } 

        // Draw Afterimages
        if (!model.afterimageTransforms.empty()) {
            glUniform1i(glGetUniformLocation(shader, "isAfterimage"), true);
            glBindVertexArray(vao); 

            for (const auto& transform : model.afterimageTransforms) {
                glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"), 1, GL_FALSE, glm::value_ptr(transform));
                glUniformMatrix4fv(glGetUniformLocation(shader, "prevModelTransform"), 1, GL_FALSE, glm::value_ptr(transform));

                glCullFace(GL_BACK); 
                switch (model.obj) {
                    case TAILS:
                        glDrawArrays(GL_TRIANGLES, 0, tailsVertices.size());
                        break;
                    case SONIC:
                        glDrawArrays(GL_TRIANGLES, tailsVertices.size(), sonicVertices.size());
                        break;
                    case KNUCKLES:
                        glDrawArrays(GL_TRIANGLES, tailsVertices.size() + sonicVertices.size(), knucklesVertices.size());
                        break;
                    default:
                        break;
                }
            } 
            
            glUniform1i(glGetUniformLocation(shader, "isAfterimage"), false);
        } 
    }
    /////////////////////////////////////////////////////////////////////////
    // draw the skybox
    glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
    glDisable(GL_CULL_FACE); // Disable culling for skybox
    glUseProgram(skyboxShader);

    // Remove translation from view matrix for skybox
    glm::mat4 skyboxView = glm::mat4(glm::mat3(viewTransform));
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "viewTransform"), 1, GL_FALSE, glm::value_ptr(skyboxView));
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "projectionTransform"), 1, GL_FALSE, glm::value_ptr(projectionTransform));

    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0); // Use texture unit 0 for skybox
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    glUniform1i(glGetUniformLocation(skyboxShader, "skybox"), 0); // Tell shader skybox sampler is on unit 0
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glEnable(GL_CULL_FACE); // Re-enable culling
    glDepthFunc(GL_LESS); // Set depth function back to default
    ///////////////////////////////////////////////////////////////////////////
    // Render the fbo
    renderFbo();

    prevProjTransform = projectionTransform;
    prevViewTransform = viewTransform;

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

