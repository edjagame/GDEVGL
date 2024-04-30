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
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE  "AMOGUS"
#define I_CENTER 0
#define I_HEXAGON 6
GLFWwindow *pWindow;

// define a vertex array to hold our vertices


float vertices[] = {
//Triangle #0
0.288675f,	0.000000f,	-0.166667f,		1.00f,	0.50f,	0.50f,		0.500000f,	-0.000000f,	-0.866000f,		0.202608f,	0.901937f,		0,	
0.000000f,	0.000000f,	-0.333333f,		1.00f,	0.50f,	0.50f,		0.500000f,	-0.000000f,	-0.866000f,		0.403803f,	0.972939f,		0,	
0.000000f,	0.833333f,	-0.333333f,		1.00f,	0.50f,	0.50f,		0.500000f,	-0.000000f,	-0.866000f,		0.308986f,	0.752300f,		0,	

//Triangle #1
0.288675f,	0.833333f,	-0.166667f,		1.00f,	0.50f,	0.50f,		1.000000f,	-0.000000f,	-0.000000f,		0.189629f,	0.665478f,		0,	
0.288675f,	0.000000f,	0.166667f,		1.00f,	0.50f,	0.50f,		1.000000f,	-0.000000f,	-0.000000f,		0.230454f,	0.124688f,		0,	
0.288675f,	0.000000f,	-0.166667f,		1.00f,	0.50f,	0.50f,		1.000000f,	-0.000000f,	-0.000000f,		0.320228f,	0.286795f,		0,	

//Triangle #2
-0.288675f,	0.000000f,	-0.166667f,		1.00f,	0.50f,	0.50f,		-0.500000f,	-0.000000f,	-0.866000f,		0.683489f,	0.622506f,		0,	
-0.288675f,	0.833333f,	-0.166667f,		1.00f,	0.50f,	0.50f,		-0.500000f,	-0.000000f,	-0.866000f,		0.410992f,	0.666038f,		0,	
0.000000f,	0.000000f,	-0.333333f,		1.00f,	0.50f,	0.50f,		-0.500000f,	-0.000000f,	-0.866000f,		0.403803f,	0.972939f,		0,	

//Triangle #3
-0.288675f,	0.833333f,	0.166667f,		1.00f,	0.50f,	0.50f,		-1.000000f,	-0.000000f,	-0.000000f,		0.458307f,	0.466728f,		0,	
-0.288675f,	0.833333f,	-0.166667f,		1.00f,	0.50f,	0.50f,		-1.000000f,	-0.000000f,	-0.000000f,		0.410992f,	0.666038f,		0,	
-0.288675f,	0.000000f,	-0.166667f,		1.00f,	0.50f,	0.50f,		-1.000000f,	-0.000000f,	-0.000000f,		0.683489f,	0.622506f,		0,	

//Triangle #4
-0.288675f,	0.833333f,	-0.166667f,		1.00f,	0.50f,	0.50f,		-0.500000f,	-0.000000f,	-0.866000f,		0.410992f,	0.666038f,		0,	
0.000000f,	0.833333f,	-0.333333f,		1.00f,	0.50f,	0.50f,		-0.500000f,	-0.000000f,	-0.866000f,		0.308986f,	0.752300f,		0,	
0.000000f,	0.000000f,	-0.333333f,		1.00f,	0.50f,	0.50f,		-0.500000f,	-0.000000f,	-0.866000f,		0.403803f,	0.972939f,		0,	

//Triangle #5
0.288675f,	0.000000f,	0.166667f,		1.00f,	0.50f,	0.50f,		0.500000f,	-0.000000f,	0.866000f,		0.230454f,	0.124688f,		0,	
0.288675f,	0.833333f,	0.166667f,		1.00f,	0.50f,	0.50f,		0.500000f,	-0.000000f,	0.866000f,		0.142353f,	0.570768f,		0,	
0.000000f,	0.000000f,	0.333333f,		1.00f,	0.50f,	0.50f,		0.500000f,	-0.000000f,	0.866000f,		0.120780f,	0.026197f,		0,	

//Triangle #6
-0.288675f,	0.000000f,	0.166667f,		1.00f,	0.50f,	0.50f,		-0.500000f,	-0.000000f,	0.866000f,		0.000057f,	0.000057f,		0,	
0.000000f,	0.000000f,	0.333333f,		1.00f,	0.50f,	0.50f,		-0.500000f,	-0.000000f,	0.866000f,		0.120780f,	0.026197f,		0,	
0.000000f,	0.833333f,	0.333333f,		1.00f,	0.50f,	0.50f,		-0.500000f,	-0.000000f,	0.866000f,		0.080522f,	0.565709f,		0,	

//Triangle #7
-0.288675f,	0.000000f,	0.166667f,		1.00f,	0.50f,	0.50f,		-1.000000f,	-0.000000f,	-0.000000f,		0.460191f,	0.148810f,		0,	
-0.288675f,	0.833333f,	0.166667f,		1.00f,	0.50f,	0.50f,		-1.000000f,	-0.000000f,	-0.000000f,		0.458307f,	0.466728f,		0,	
-0.288675f,	0.000000f,	-0.166667f,		1.00f,	0.50f,	0.50f,		-1.000000f,	-0.000000f,	-0.000000f,		0.683489f,	0.622506f,		0,	

//Triangle #8
0.288675f,	0.833333f,	-0.166667f,		1.00f,	0.50f,	0.50f,		1.000000f,	-0.000000f,	-0.000000f,		0.189629f,	0.665478f,		0,	
0.288675f,	0.833333f,	0.166667f,		1.00f,	0.50f,	0.50f,		1.000000f,	-0.000000f,	-0.000000f,		0.142353f,	0.570768f,		0,	
0.288675f,	0.000000f,	0.166667f,		1.00f,	0.50f,	0.50f,		1.000000f,	-0.000000f,	-0.000000f,		0.230454f,	0.124688f,		0,	

//Triangle #9
0.000000f,	0.833333f,	0.333333f,		1.00f,	0.50f,	0.50f,		-0.500000f,	-0.000000f,	0.866000f,		0.080522f,	0.565709f,		0,	
-0.288675f,	0.833333f,	0.166667f,		1.00f,	0.50f,	0.50f,		-0.500000f,	-0.000000f,	0.866000f,		0.000057f,	0.725951f,		0,	
-0.288675f,	0.000000f,	0.166667f,		1.00f,	0.50f,	0.50f,		-0.500000f,	-0.000000f,	0.866000f,		0.000057f,	0.000057f,		0,	

//Triangle #10
0.000000f,	0.000000f,	-0.333333f,		1.00f,	0.50f,	0.50f,		-0.000000f,	-1.000000f,	-0.000000f,		0.403803f,	0.972939f,		0,	
0.288675f,	0.000000f,	0.166667f,		1.00f,	0.50f,	0.50f,		-0.000000f,	-1.000000f,	-0.000000f,		0.319642f,	0.289249f,		0,	
-0.288675f,	0.000000f,	0.166667f,		1.00f,	0.50f,	0.50f,		-0.000000f,	-1.000000f,	-0.000000f,		0.460191f,	0.148810f,		0,	

//Triangle #11
0.288675f,	0.833333f,	0.166667f,		1.00f,	0.50f,	0.50f,		0.500000f,	-0.000000f,	0.866000f,		0.142353f,	0.570768f,		0,	
0.000000f,	0.833333f,	0.333333f,		1.00f,	0.50f,	0.50f,		0.500000f,	-0.000000f,	0.866000f,		0.080522f,	0.565709f,		0,	
0.000000f,	0.000000f,	0.333333f,		1.00f,	0.50f,	0.50f,		0.500000f,	-0.000000f,	0.866000f,		0.120780f,	0.026197f,		0,	

//Triangle #12
0.000000f,	0.833333f,	-0.333333f,		1.00f,	0.50f,	0.50f,		0.500000f,	-0.000000f,	-0.866000f,		0.205296f,	0.909015f,		0,	
0.288675f,	0.833333f,	-0.166667f,		1.00f,	0.50f,	0.50f,		0.500000f,	-0.000000f,	-0.866000f,		0.189629f,	0.665478f,		0,	
0.288675f,	0.000000f,	-0.166667f,		1.00f,	0.50f,	0.50f,		0.500000f,	-0.000000f,	-0.866000f,		0.320228f,	0.286795f,		0,	

//Triangle #13
0.000000f,	1.000000f,	-0.140883f,		1.00f,	0.50f,	0.50f,		-0.191300f,	0.923900f,	-0.331400f,		0.136540f,	0.780790f,		0,	
-0.239860f,	0.951184f,	-0.138483f,		1.00f,	0.50f,	0.50f,		-0.191300f,	0.923900f,	-0.331400f,		0.090671f,	0.878617f,		0,	
-0.122008f,	1.000000f,	-0.070442f,		1.00f,	0.50f,	0.50f,		-0.191300f,	0.923900f,	-0.331400f,		0.105784f,	0.794041f,		0,	

//Triangle #14
0.000000f,	0.951184f,	-0.276966f,		1.00f,	0.50f,	0.50f,		-0.461900f,	0.382700f,	-0.800100f,		0.161077f,	0.840068f,		0,	
-0.288675f,	0.833333f,	-0.166667f,		1.00f,	0.50f,	0.50f,		-0.461900f,	0.382700f,	-0.800100f,		0.075709f,	0.999943f,		0,	
-0.239860f,	0.951184f,	-0.138483f,		1.00f,	0.50f,	0.50f,		-0.461900f,	0.382700f,	-0.800100f,		0.090671f,	0.878617f,		0,	

//Triangle #15
-0.122008f,	1.000000f,	-0.070442f,		1.00f,	0.50f,	0.50f,		-0.382700f,	0.923900f,	-0.000000f,		0.105784f,	0.794041f,		0,	
-0.239860f,	0.951184f,	0.138483f,		1.00f,	0.50f,	0.50f,		-0.382700f,	0.923900f,	-0.000000f,		0.053723f,	0.729995f,		0,	
-0.122008f,	1.000000f,	0.070442f,		1.00f,	0.50f,	0.50f,		-0.382700f,	0.923900f,	-0.000000f,		0.088451f,	0.730218f,		0,	

//Triangle #16
-0.239860f,	0.951184f,	-0.138483f,		1.00f,	0.50f,	0.50f,		-0.923900f,	0.382700f,	-0.000000f,		0.090671f,	0.878617f,		0,	
-0.288675f,	0.833333f,	0.166667f,		1.00f,	0.50f,	0.50f,		-0.923900f,	0.382700f,	-0.000000f,		0.000057f,	0.725951f,		0,	
-0.239860f,	0.951184f,	0.138483f,		1.00f,	0.50f,	0.50f,		-0.923900f,	0.382700f,	-0.000000f,		0.053723f,	0.729995f,		0,	

//Triangle #17
-0.122008f,	1.000000f,	0.070442f,		1.00f,	0.50f,	0.50f,		-0.191300f,	0.923900f,	0.331400f,		0.088451f,	0.730218f,		0,	
0.000000f,	0.951184f,	0.276966f,		1.00f,	0.50f,	0.50f,		-0.191300f,	0.923900f,	0.331400f,		0.092066f,	0.623760f,		0,	
0.000000f,	1.000000f,	0.140883f,		1.00f,	0.50f,	0.50f,		-0.191300f,	0.923900f,	0.331400f,		0.102936f,	0.674919f,		0,	

//Triangle #18
-0.239860f,	0.951184f,	0.138483f,		1.00f,	0.50f,	0.50f,		-0.461900f,	0.382700f,	0.800100f,		0.053723f,	0.729995f,		0,	
0.000000f,	0.833333f,	0.333333f,		1.00f,	0.50f,	0.50f,		-0.461900f,	0.382700f,	0.800100f,		0.080522f,	0.565709f,		0,	
0.000000f,	0.951184f,	0.276966f,		1.00f,	0.50f,	0.50f,		-0.461900f,	0.382700f,	0.800100f,		0.092066f,	0.623760f,		0,	

//Triangle #19
0.000000f,	1.000000f,	0.140883f,		1.00f,	0.50f,	0.50f,		0.191300f,	0.923900f,	0.331400f,		0.102936f,	0.674919f,		0,	
0.239860f,	0.951184f,	0.138483f,		1.00f,	0.50f,	0.50f,		0.191300f,	0.923900f,	0.331400f,		0.135509f,	0.622771f,		0,	
0.122008f,	1.000000f,	0.070442f,		1.00f,	0.50f,	0.50f,		0.191300f,	0.923900f,	0.331400f,		0.126905f,	0.674382f,		0,	

//Triangle #20
0.000000f,	0.951184f,	0.276966f,		1.00f,	0.50f,	0.50f,		0.461900f,	0.382700f,	0.800100f,		0.092066f,	0.623760f,		0,	
0.288675f,	0.833333f,	0.166667f,		1.00f,	0.50f,	0.50f,		0.461900f,	0.382700f,	0.800100f,		0.142353f,	0.570768f,		0,	
0.239860f,	0.951184f,	0.138483f,		1.00f,	0.50f,	0.50f,		0.461900f,	0.382700f,	0.800100f,		0.135509f,	0.622771f,		0,	

//Triangle #21
0.122008f,	1.000000f,	0.070442f,		1.00f,	0.50f,	0.50f,		0.382700f,	0.923900f,	-0.000000f,		0.126905f,	0.674382f,		0,	
0.239860f,	0.951184f,	-0.138483f,		1.00f,	0.50f,	0.50f,		0.382700f,	0.923900f,	-0.000000f,		0.166947f,	0.696840f,		0,	
0.122008f,	1.000000f,	-0.070442f,		1.00f,	0.50f,	0.50f,		0.382700f,	0.923900f,	-0.000000f,		0.143774f,	0.716596f,		0,	

//Triangle #22
0.239860f,	0.951184f,	0.138483f,		1.00f,	0.50f,	0.50f,		0.923900f,	0.382700f,	-0.000000f,		0.135509f,	0.622771f,		0,	
0.288675f,	0.833333f,	-0.166667f,		1.00f,	0.50f,	0.50f,		0.923900f,	0.382700f,	-0.000000f,		0.189629f,	0.665478f,		0,	
0.239860f,	0.951184f,	-0.138483f,		1.00f,	0.50f,	0.50f,		0.923900f,	0.382700f,	-0.000000f,		0.166947f,	0.696840f,		0,	

//Triangle #23
0.122008f,	1.000000f,	-0.070442f,		1.00f,	0.50f,	0.50f,		0.191300f,	0.923900f,	-0.331400f,		0.143774f,	0.716596f,		0,	
0.000000f,	0.951184f,	-0.276966f,		1.00f,	0.50f,	0.50f,		0.191300f,	0.923900f,	-0.331400f,		0.161077f,	0.840068f,		0,	
0.000000f,	1.000000f,	-0.140883f,		1.00f,	0.50f,	0.50f,		0.191300f,	0.923900f,	-0.331400f,		0.136540f,	0.780790f,		0,	

//Triangle #24
0.239860f,	0.951184f,	-0.138483f,		1.00f,	0.50f,	0.50f,		0.461900f,	0.382700f,	-0.800100f,		0.166947f,	0.696840f,		0,	
0.000000f,	0.833333f,	-0.333333f,		1.00f,	0.50f,	0.50f,		0.461900f,	0.382700f,	-0.800100f,		0.205296f,	0.909015f,		0,	
0.000000f,	0.951184f,	-0.276966f,		1.00f,	0.50f,	0.50f,		0.461900f,	0.382700f,	-0.800100f,		0.161077f,	0.840068f,		0,	

//Triangle #25
-0.122008f,	1.000000f,	0.070442f,		1.00f,	0.50f,	0.50f,		-0.000000f,	1.000000f,	-0.000000f,		0.088451f,	0.730218f,		0,	
0.122008f,	1.000000f,	0.070442f,		1.00f,	0.50f,	0.50f,		-0.000000f,	1.000000f,	-0.000000f,		0.126905f,	0.674382f,		0,	
0.000000f,	1.000000f,	-0.140883f,		1.00f,	0.50f,	0.50f,		-0.000000f,	1.000000f,	-0.000000f,		0.136540f,	0.780790f,		0,	

//Triangle #26
-0.288675f,	0.000000f,	0.166667f,		1.00f,	0.50f,	0.50f,		-0.000000f,	-1.000000f,	-0.000000f,		0.460191f,	0.148810f,		0,	
-0.288675f,	0.000000f,	-0.166667f,		1.00f,	0.50f,	0.50f,		-0.000000f,	-1.000000f,	-0.000000f,		0.683489f,	0.622506f,		0,	
0.000000f,	0.000000f,	-0.333333f,		1.00f,	0.50f,	0.50f,		-0.000000f,	-1.000000f,	-0.000000f,		0.403803f,	0.972939f,		0,	

//Triangle #27
0.000000f,	0.000000f,	-0.333333f,		1.00f,	0.50f,	0.50f,		-0.000000f,	-1.000000f,	-0.000000f,		0.403803f,	0.972939f,		0,	
0.288675f,	0.000000f,	-0.166667f,		1.00f,	0.50f,	0.50f,		-0.000000f,	-1.000000f,	-0.000000f,		0.202608f,	0.901937f,		0,	
0.288675f,	0.000000f,	0.166667f,		1.00f,	0.50f,	0.50f,		-0.000000f,	-1.000000f,	-0.000000f,		0.319642f,	0.289249f,		0,	

//Triangle #28
0.288675f,	0.000000f,	0.166667f,		1.00f,	0.50f,	0.50f,		-0.000000f,	-1.000000f,	-0.000000f,		0.319642f,	0.289249f,		0,	
0.000000f,	0.000000f,	0.333333f,		1.00f,	0.50f,	0.50f,		-0.000000f,	-1.000000f,	-0.000000f,		0.369205f,	0.040247f,		0,	
-0.288675f,	0.000000f,	0.166667f,		1.00f,	0.50f,	0.50f,		-0.000000f,	-1.000000f,	-0.000000f,		0.460191f,	0.148810f,		0,	

//Triangle #29
0.000000f,	1.000000f,	-0.140883f,		1.00f,	0.50f,	0.50f,		-0.191300f,	0.923900f,	-0.331400f,		0.136540f,	0.780790f,		0,	
0.000000f,	0.951184f,	-0.276966f,		1.00f,	0.50f,	0.50f,		-0.191300f,	0.923900f,	-0.331400f,		0.161077f,	0.840068f,		0,	
-0.239860f,	0.951184f,	-0.138483f,		1.00f,	0.50f,	0.50f,		-0.191300f,	0.923900f,	-0.331400f,		0.090671f,	0.878617f,		0,	

//Triangle #30
0.000000f,	0.951184f,	-0.276966f,		1.00f,	0.50f,	0.50f,		-0.461900f,	0.382700f,	-0.800100f,		0.161077f,	0.840068f,		0,	
0.000000f,	0.833333f,	-0.333333f,		1.00f,	0.50f,	0.50f,		-0.461900f,	0.382700f,	-0.800100f,		0.205296f,	0.909015f,		0,	
-0.288675f,	0.833333f,	-0.166667f,		1.00f,	0.50f,	0.50f,		-0.461900f,	0.382700f,	-0.800100f,		0.075709f,	0.999943f,		0,	

//Triangle #31
-0.122008f,	1.000000f,	-0.070442f,		1.00f,	0.50f,	0.50f,		-0.382700f,	0.923900f,	-0.000000f,		0.105784f,	0.794041f,		0,	
-0.239860f,	0.951184f,	-0.138483f,		1.00f,	0.50f,	0.50f,		-0.382700f,	0.923900f,	-0.000000f,		0.090671f,	0.878617f,		0,	
-0.239860f,	0.951184f,	0.138483f,		1.00f,	0.50f,	0.50f,		-0.382700f,	0.923900f,	-0.000000f,		0.053723f,	0.729995f,		0,	

//Triangle #32
-0.239860f,	0.951184f,	-0.138483f,		1.00f,	0.50f,	0.50f,		-0.923900f,	0.382700f,	-0.000000f,		0.090671f,	0.878617f,		0,	
-0.288675f,	0.833333f,	-0.166667f,		1.00f,	0.50f,	0.50f,		-0.923900f,	0.382700f,	-0.000000f,		0.075709f,	0.999943f,		0,	
-0.288675f,	0.833333f,	0.166667f,		1.00f,	0.50f,	0.50f,		-0.923900f,	0.382700f,	-0.000000f,		0.000057f,	0.725951f,		0,	

//Triangle #33
-0.122008f,	1.000000f,	0.070442f,		1.00f,	0.50f,	0.50f,		-0.191300f,	0.923900f,	0.331400f,		0.088451f,	0.730218f,		0,	
-0.239860f,	0.951184f,	0.138483f,		1.00f,	0.50f,	0.50f,		-0.191300f,	0.923900f,	0.331400f,		0.053723f,	0.729995f,		0,	
0.000000f,	0.951184f,	0.276966f,		1.00f,	0.50f,	0.50f,		-0.191300f,	0.923900f,	0.331400f,		0.092066f,	0.623760f,		0,	

//Triangle #34
-0.239860f,	0.951184f,	0.138483f,		1.00f,	0.50f,	0.50f,		-0.461900f,	0.382700f,	0.800100f,		0.053723f,	0.729995f,		0,	
-0.288675f,	0.833333f,	0.166667f,		1.00f,	0.50f,	0.50f,		-0.461900f,	0.382700f,	0.800100f,		0.000057f,	0.725951f,		0,	
0.000000f,	0.833333f,	0.333333f,		1.00f,	0.50f,	0.50f,		-0.461900f,	0.382700f,	0.800100f,		0.080522f,	0.565709f,		0,	

//Triangle #35
0.000000f,	1.000000f,	0.140883f,		1.00f,	0.50f,	0.50f,		0.191300f,	0.923900f,	0.331400f,		0.102936f,	0.674919f,		0,	
0.000000f,	0.951184f,	0.276966f,		1.00f,	0.50f,	0.50f,		0.191300f,	0.923900f,	0.331400f,		0.092066f,	0.623760f,		0,	
0.239860f,	0.951184f,	0.138483f,		1.00f,	0.50f,	0.50f,		0.191300f,	0.923900f,	0.331400f,		0.135509f,	0.622771f,		0,	

//Triangle #36
0.000000f,	0.951184f,	0.276966f,		1.00f,	0.50f,	0.50f,		0.461900f,	0.382700f,	0.800100f,		0.092066f,	0.623760f,		0,	
0.000000f,	0.833333f,	0.333333f,		1.00f,	0.50f,	0.50f,		0.461900f,	0.382700f,	0.800100f,		0.080522f,	0.565709f,		0,	
0.288675f,	0.833333f,	0.166667f,		1.00f,	0.50f,	0.50f,		0.461900f,	0.382700f,	0.800100f,		0.142353f,	0.570768f,		0,	

//Triangle #37
0.122008f,	1.000000f,	0.070442f,		1.00f,	0.50f,	0.50f,		0.382700f,	0.923900f,	-0.000000f,		0.126905f,	0.674382f,		0,	
0.239860f,	0.951184f,	0.138483f,		1.00f,	0.50f,	0.50f,		0.382700f,	0.923900f,	-0.000000f,		0.135509f,	0.622771f,		0,	
0.239860f,	0.951184f,	-0.138483f,		1.00f,	0.50f,	0.50f,		0.382700f,	0.923900f,	-0.000000f,		0.166947f,	0.696840f,		0,	

//Triangle #38
0.239860f,	0.951184f,	0.138483f,		1.00f,	0.50f,	0.50f,		0.923900f,	0.382700f,	-0.000000f,		0.135509f,	0.622771f,		0,	
0.288675f,	0.833333f,	0.166667f,		1.00f,	0.50f,	0.50f,		0.923900f,	0.382700f,	-0.000000f,		0.142353f,	0.570768f,		0,	
0.288675f,	0.833333f,	-0.166667f,		1.00f,	0.50f,	0.50f,		0.923900f,	0.382700f,	-0.000000f,		0.189629f,	0.665478f,		0,	

//Triangle #39
0.122008f,	1.000000f,	-0.070442f,		1.00f,	0.50f,	0.50f,		0.191300f,	0.923900f,	-0.331400f,		0.143774f,	0.716596f,		0,	
0.239860f,	0.951184f,	-0.138483f,		1.00f,	0.50f,	0.50f,		0.191300f,	0.923900f,	-0.331400f,		0.166947f,	0.696840f,		0,	
0.000000f,	0.951184f,	-0.276966f,		1.00f,	0.50f,	0.50f,		0.191300f,	0.923900f,	-0.331400f,		0.161077f,	0.840068f,		0,	

//Triangle #40
0.239860f,	0.951184f,	-0.138483f,		1.00f,	0.50f,	0.50f,		0.461900f,	0.382700f,	-0.800100f,		0.166947f,	0.696840f,		0,	
0.288675f,	0.833333f,	-0.166667f,		1.00f,	0.50f,	0.50f,		0.461900f,	0.382700f,	-0.800100f,		0.189629f,	0.665478f,		0,	
0.000000f,	0.833333f,	-0.333333f,		1.00f,	0.50f,	0.50f,		0.461900f,	0.382700f,	-0.800100f,		0.205296f,	0.909015f,		0,	

//Triangle #41
0.000000f,	1.000000f,	-0.140883f,		1.00f,	0.50f,	0.50f,		-0.000000f,	1.000000f,	-0.000000f,		0.136540f,	0.780790f,		0,	
-0.122008f,	1.000000f,	-0.070442f,		1.00f,	0.50f,	0.50f,		-0.000000f,	1.000000f,	-0.000000f,		0.105784f,	0.794041f,		0,	
-0.122008f,	1.000000f,	0.070442f,		1.00f,	0.50f,	0.50f,		-0.000000f,	1.000000f,	-0.000000f,		0.088451f,	0.730218f,		0,	

//Triangle #42
-0.122008f,	1.000000f,	0.070442f,		1.00f,	0.50f,	0.50f,		-0.000000f,	1.000000f,	-0.000000f,		0.088451f,	0.730218f,		0,	
0.000000f,	1.000000f,	0.140883f,		1.00f,	0.50f,	0.50f,		-0.000000f,	1.000000f,	-0.000000f,		0.102936f,	0.674919f,		0,	
0.122008f,	1.000000f,	0.070442f,		1.00f,	0.50f,	0.50f,		-0.000000f,	1.000000f,	-0.000000f,		0.126905f,	0.674382f,		0,	

//Triangle #43
0.122008f,	1.000000f,	0.070442f,		1.00f,	0.50f,	0.50f,		-0.000000f,	1.000000f,	-0.000000f,		0.126905f,	0.674382f,		0,	
0.122008f,	1.000000f,	-0.070442f,		1.00f,	0.50f,	0.50f,		-0.000000f,	1.000000f,	-0.000000f,		0.143774f,	0.716596f,		0,	
0.000000f,	1.000000f,	-0.140883f,		1.00f,	0.50f,	0.50f,		-0.000000f,	1.000000f,	-0.000000f,		0.136540f,	0.780790f,		0,	

//Triangle #44
-0.455342f,	0.833333f,	0.166667f,		0.70f,	0.30f,	0.30f,		-1.000000f,	-0.000000f,	-0.000000f,		0.625000f,	0.000000f,		0,	
-0.455342f,	0.166667f,	-0.166667f,		0.70f,	0.30f,	0.30f,		-1.000000f,	-0.000000f,	-0.000000f,		0.375000f,	0.250000f,		0,	
-0.455342f,	0.166667f,	0.166667f,		0.70f,	0.30f,	0.30f,		-1.000000f,	-0.000000f,	-0.000000f,		0.375000f,	0.000000f,		0,	

//Triangle #45
-0.455342f,	0.833333f,	-0.166667f,		0.70f,	0.30f,	0.30f,		-0.000000f,	-0.000000f,	-1.000000f,		0.625000f,	0.250000f,		0,	
-0.288675f,	0.166667f,	-0.166667f,		0.70f,	0.30f,	0.30f,		-0.000000f,	-0.000000f,	-1.000000f,		0.375000f,	0.500000f,		0,	
-0.455342f,	0.166667f,	-0.166667f,		0.70f,	0.30f,	0.30f,		-0.000000f,	-0.000000f,	-1.000000f,		0.375000f,	0.250000f,		0,	

//Triangle #46
-0.288675f,	0.833333f,	-0.166667f,		0.70f,	0.30f,	0.30f,		1.000000f,	-0.000000f,	-0.000000f,		0.625000f,	0.500000f,		0,	
-0.288675f,	0.166667f,	0.166667f,		0.70f,	0.30f,	0.30f,		1.000000f,	-0.000000f,	-0.000000f,		0.375000f,	0.750000f,		0,	
-0.288675f,	0.166667f,	-0.166667f,		0.70f,	0.30f,	0.30f,		1.000000f,	-0.000000f,	-0.000000f,		0.375000f,	0.500000f,		0,	

//Triangle #47
-0.288675f,	0.833333f,	0.166667f,		0.70f,	0.30f,	0.30f,		-0.000000f,	-0.000000f,	1.000000f,		0.625000f,	0.750000f,		0,	
-0.455342f,	0.166667f,	0.166667f,		0.70f,	0.30f,	0.30f,		-0.000000f,	-0.000000f,	1.000000f,		0.375000f,	1.000000f,		0,	
-0.288675f,	0.166667f,	0.166667f,		0.70f,	0.30f,	0.30f,		-0.000000f,	-0.000000f,	1.000000f,		0.375000f,	0.750000f,		0,	

//Triangle #48
-0.288675f,	0.166667f,	-0.166667f,		0.70f,	0.30f,	0.30f,		-0.000000f,	-1.000000f,	-0.000000f,		0.375000f,	0.500000f,		0,	
-0.455342f,	0.166667f,	0.166667f,		0.70f,	0.30f,	0.30f,		-0.000000f,	-1.000000f,	-0.000000f,		0.125000f,	0.750000f,		0,	
-0.455342f,	0.166667f,	-0.166667f,		0.70f,	0.30f,	0.30f,		-0.000000f,	-1.000000f,	-0.000000f,		0.125000f,	0.500000f,		0,	

//Triangle #49
-0.455342f,	0.833333f,	-0.166667f,		0.70f,	0.30f,	0.30f,		-0.000000f,	1.000000f,	-0.000000f,		0.875000f,	0.500000f,		0,	
-0.288675f,	0.833333f,	0.166667f,		0.70f,	0.30f,	0.30f,		-0.000000f,	1.000000f,	-0.000000f,		0.625000f,	0.750000f,		0,	
-0.288675f,	0.833333f,	-0.166667f,		0.70f,	0.30f,	0.30f,		-0.000000f,	1.000000f,	-0.000000f,		0.625000f,	0.500000f,		0,	

//Triangle #50
-0.455342f,	0.833333f,	0.166667f,		0.70f,	0.30f,	0.30f,		-1.000000f,	-0.000000f,	-0.000000f,		0.625000f,	0.000000f,		0,	
-0.455342f,	0.833333f,	-0.166667f,		0.70f,	0.30f,	0.30f,		-1.000000f,	-0.000000f,	-0.000000f,		0.625000f,	0.250000f,		0,	
-0.455342f,	0.166667f,	-0.166667f,		0.70f,	0.30f,	0.30f,		-1.000000f,	-0.000000f,	-0.000000f,		0.375000f,	0.250000f,		0,	

//Triangle #51
-0.455342f,	0.833333f,	-0.166667f,		0.70f,	0.30f,	0.30f,		-0.000000f,	-0.000000f,	-1.000000f,		0.625000f,	0.250000f,		0,	
-0.288675f,	0.833333f,	-0.166667f,		0.70f,	0.30f,	0.30f,		-0.000000f,	-0.000000f,	-1.000000f,		0.625000f,	0.500000f,		0,	
-0.288675f,	0.166667f,	-0.166667f,		0.70f,	0.30f,	0.30f,		-0.000000f,	-0.000000f,	-1.000000f,		0.375000f,	0.500000f,		0,	

//Triangle #52
-0.288675f,	0.833333f,	-0.166667f,		0.70f,	0.30f,	0.30f,		1.000000f,	-0.000000f,	-0.000000f,		0.625000f,	0.500000f,		0,	
-0.288675f,	0.833333f,	0.166667f,		0.70f,	0.30f,	0.30f,		1.000000f,	-0.000000f,	-0.000000f,		0.625000f,	0.750000f,		0,	
-0.288675f,	0.166667f,	0.166667f,		0.70f,	0.30f,	0.30f,		1.000000f,	-0.000000f,	-0.000000f,		0.375000f,	0.750000f,		0,	

//Triangle #53
-0.288675f,	0.833333f,	0.166667f,		0.70f,	0.30f,	0.30f,		-0.000000f,	-0.000000f,	1.000000f,		0.625000f,	0.750000f,		0,	
-0.455342f,	0.833333f,	0.166667f,		0.70f,	0.30f,	0.30f,		-0.000000f,	-0.000000f,	1.000000f,		0.625000f,	1.000000f,		0,	
-0.455342f,	0.166667f,	0.166667f,		0.70f,	0.30f,	0.30f,		-0.000000f,	-0.000000f,	1.000000f,		0.375000f,	1.000000f,		0,	

//Triangle #54
-0.288675f,	0.166667f,	-0.166667f,		0.70f,	0.30f,	0.30f,		-0.000000f,	-1.000000f,	-0.000000f,		0.375000f,	0.500000f,		0,	
-0.288675f,	0.166667f,	0.166667f,		0.70f,	0.30f,	0.30f,		-0.000000f,	-1.000000f,	-0.000000f,		0.375000f,	0.750000f,		0,	
-0.455342f,	0.166667f,	0.166667f,		0.70f,	0.30f,	0.30f,		-0.000000f,	-1.000000f,	-0.000000f,		0.125000f,	0.750000f,		0,	

//Triangle #55
-0.455342f,	0.833333f,	-0.166667f,		0.70f,	0.30f,	0.30f,		-0.000000f,	1.000000f,	-0.000000f,		0.875000f,	0.500000f,		0,	
-0.455342f,	0.833333f,	0.166667f,		0.70f,	0.30f,	0.30f,		-0.000000f,	1.000000f,	-0.000000f,		0.875000f,	0.750000f,		0,	
-0.288675f,	0.833333f,	0.166667f,		0.70f,	0.30f,	0.30f,		-0.000000f,	1.000000f,	-0.000000f,		0.625000f,	0.750000f,		0,	

//Triangle #56
0.288675f,	0.708333f,	0.166667f,		0.50f,	1.00f,	1.00f,		-1.000000f,	-0.000000f,	-0.000000f,		0.625000f,	0.000000f,		1,	
0.288675f,	0.458333f,	-0.166667f,		0.50f,	1.00f,	1.00f,		-1.000000f,	-0.000000f,	-0.000000f,		0.375000f,	0.250000f,		1,	
0.288675f,	0.458333f,	0.166667f,		0.50f,	1.00f,	1.00f,		-1.000000f,	-0.000000f,	-0.000000f,		0.375000f,	0.000000f,		1,	

//Triangle #57
0.288675f,	0.708333f,	-0.166667f,		0.50f,	1.00f,	1.00f,		-0.000000f,	-0.000000f,	-1.000000f,		0.625000f,	0.250000f,		1,	
0.455342f,	0.458333f,	-0.166667f,		0.50f,	1.00f,	1.00f,		-0.000000f,	-0.000000f,	-1.000000f,		0.375000f,	0.500000f,		1,	
0.288675f,	0.458333f,	-0.166667f,		0.50f,	1.00f,	1.00f,		-0.000000f,	-0.000000f,	-1.000000f,		0.375000f,	0.250000f,		1,	

//Triangle #58
0.455342f,	0.708333f,	-0.166667f,		0.50f,	1.00f,	1.00f,		1.000000f,	-0.000000f,	-0.000000f,		0.625000f,	0.500000f,		1,	
0.455342f,	0.458333f,	0.166667f,		0.50f,	1.00f,	1.00f,		1.000000f,	-0.000000f,	-0.000000f,		0.375000f,	0.750000f,		1,	
0.455342f,	0.458333f,	-0.166667f,		0.50f,	1.00f,	1.00f,		1.000000f,	-0.000000f,	-0.000000f,		0.375000f,	0.500000f,		1,	

//Triangle #59
0.455342f,	0.708333f,	0.166667f,		0.50f,	1.00f,	1.00f,		-0.000000f,	-0.000000f,	1.000000f,		0.625000f,	0.750000f,		1,	
0.288675f,	0.458333f,	0.166667f,		0.50f,	1.00f,	1.00f,		-0.000000f,	-0.000000f,	1.000000f,		0.375000f,	1.000000f,		1,	
0.455342f,	0.458333f,	0.166667f,		0.50f,	1.00f,	1.00f,		-0.000000f,	-0.000000f,	1.000000f,		0.375000f,	0.750000f,		1,	

//Triangle #60
0.455342f,	0.458333f,	-0.166667f,		0.50f,	1.00f,	1.00f,		-0.000000f,	-1.000000f,	-0.000000f,		0.375000f,	0.500000f,		1,	
0.288675f,	0.458333f,	0.166667f,		0.50f,	1.00f,	1.00f,		-0.000000f,	-1.000000f,	-0.000000f,		0.125000f,	0.750000f,		1,	
0.288675f,	0.458333f,	-0.166667f,		0.50f,	1.00f,	1.00f,		-0.000000f,	-1.000000f,	-0.000000f,		0.125000f,	0.500000f,		1,	

//Triangle #61
0.288675f,	0.708333f,	-0.166667f,		0.50f,	1.00f,	1.00f,		-0.000000f,	1.000000f,	-0.000000f,		0.875000f,	0.500000f,		1,	
0.455342f,	0.708333f,	0.166667f,		0.50f,	1.00f,	1.00f,		-0.000000f,	1.000000f,	-0.000000f,		0.625000f,	0.750000f,		1,	
0.455342f,	0.708333f,	-0.166667f,		0.50f,	1.00f,	1.00f,		-0.000000f,	1.000000f,	-0.000000f,		0.625000f,	0.500000f,		1,	

//Triangle #62
0.288675f,	0.708333f,	0.166667f,		0.50f,	1.00f,	1.00f,		-1.000000f,	-0.000000f,	-0.000000f,		0.625000f,	0.000000f,		1,	
0.288675f,	0.708333f,	-0.166667f,		0.50f,	1.00f,	1.00f,		-1.000000f,	-0.000000f,	-0.000000f,		0.625000f,	0.250000f,		1,	
0.288675f,	0.458333f,	-0.166667f,		0.50f,	1.00f,	1.00f,		-1.000000f,	-0.000000f,	-0.000000f,		0.375000f,	0.250000f,		1,	

//Triangle #63
0.288675f,	0.708333f,	-0.166667f,		0.50f,	1.00f,	1.00f,		-0.000000f,	-0.000000f,	-1.000000f,		0.625000f,	0.250000f,		1,	
0.455342f,	0.708333f,	-0.166667f,		0.50f,	1.00f,	1.00f,		-0.000000f,	-0.000000f,	-1.000000f,		0.625000f,	0.500000f,		1,	
0.455342f,	0.458333f,	-0.166667f,		0.50f,	1.00f,	1.00f,		-0.000000f,	-0.000000f,	-1.000000f,		0.375000f,	0.500000f,		1,	

//Triangle #64
0.455342f,	0.708333f,	-0.166667f,		0.50f,	1.00f,	1.00f,		1.000000f,	-0.000000f,	-0.000000f,		0.625000f,	0.500000f,		1,	
0.455342f,	0.708333f,	0.166667f,		0.50f,	1.00f,	1.00f,		1.000000f,	-0.000000f,	-0.000000f,		0.625000f,	0.750000f,		1,	
0.455342f,	0.458333f,	0.166667f,		0.50f,	1.00f,	1.00f,		1.000000f,	-0.000000f,	-0.000000f,		0.375000f,	0.750000f,		1,	

//Triangle #65
0.455342f,	0.708333f,	0.166667f,		0.50f,	1.00f,	1.00f,		-0.000000f,	-0.000000f,	1.000000f,		0.625000f,	0.750000f,		1,	
0.288675f,	0.708333f,	0.166667f,		0.50f,	1.00f,	1.00f,		-0.000000f,	-0.000000f,	1.000000f,		0.625000f,	1.000000f,		1,	
0.288675f,	0.458333f,	0.166667f,		0.50f,	1.00f,	1.00f,		-0.000000f,	-0.000000f,	1.000000f,		0.375000f,	1.000000f,		1,	

//Triangle #66
0.455342f,	0.458333f,	-0.166667f,		0.50f,	1.00f,	1.00f,		-0.000000f,	-1.000000f,	-0.000000f,		0.375000f,	0.500000f,		1,	
0.455342f,	0.458333f,	0.166667f,		0.50f,	1.00f,	1.00f,		-0.000000f,	-1.000000f,	-0.000000f,		0.375000f,	0.750000f,		1,	
0.288675f,	0.458333f,	0.166667f,		0.50f,	1.00f,	1.00f,		-0.000000f,	-1.000000f,	-0.000000f,		0.125000f,	0.750000f,		1,	

//Triangle #67
0.288675f,	0.708333f,	-0.166667f,		0.50f,	1.00f,	1.00f,		-0.000000f,	1.000000f,	-0.000000f,		0.875000f,	0.500000f,		1,	
0.288675f,	0.708333f,	0.166667f,		0.50f,	1.00f,	1.00f,		-0.000000f,	1.000000f,	-0.000000f,		0.875000f,	0.750000f,		1,	
0.455342f,	0.708333f,	0.166667f,		0.50f,	1.00f,	1.00f,		-0.000000f,	1.000000f,	-0.000000f,		0.625000f,	0.750000f,		1,	
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

    // on the VAO, register the current VBO with the following vertex attribute layout:
    // - the stride length of the vertex array is 6 floats (6 * sizeof(float))
    // - layout location 0 (position) is 3 floats and starts at the first float of the vertex array (offset 0)
    // - layout location 1 (color) is also 3 floats but starts at the fourth float (offset 3 * sizeof(float))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*) (6 * sizeof(float)));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*) (9 * sizeof(float)));
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*) (11 * sizeof(float)));
    // enable the layout locations so they can be used by the vertex shader
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    // important: if you have more vertex arrays to draw, make sure you separately define them
    // with unique VAO, VBO, and EBO IDs, and follow the same process above to upload them to the GPU

    marbleTexture = gdevLoadTexture("fur.jpg", GL_REPEAT, true, true);
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
    glClearColor(0.0f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    // using our shader program...
    glUseProgram(shader);
    // ... set the active texture...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, marbleTexture);

    float time = glfwGetTime();
    glUniform1f(glGetUniformLocation(shader, "time"), time);

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

    float angle = 60;//glfwGetTime()*60;
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

        projView *= viewTransform;
    
        model = glm::translate(glm::mat4(1.0), amogusPositions[i]);
        model = glm::rotate(model, glm::radians(angle), axes[i]);
        model = glm::scale(model, scales[i]);
        
        normal = glm::transpose(glm::inverse(model));
        
        glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader, "projView"), 1, GL_FALSE, glm::value_ptr(projView));
        glUniformMatrix4fv(glGetUniformLocation(shader, "normal"), 1, GL_FALSE, glm::value_ptr(normal));

        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (3 * sizeof(float)));
    }
    
    glm::vec3 lightPos = glm::vec3(2.0 * cos(time),2.0f,2.0 * sin(time));
    glm::vec3 lightColor = glm::vec3((sin(time)+1.0)/2.0*0.8,(sin(time+6.28/3.0)+1.0)/2.0*0.8,(sin(time+12.57/3.0)+1.0)/2.0*0.8);
    
    glUniform3fv(glGetUniformLocation(shader, "lightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(shader, "lightColor"), 1, glm::value_ptr(lightColor));
    glUniform3fv(glGetUniformLocation(shader, "eyePosition"), 1, glm::value_ptr(eyePosition));
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

