#define _USE_MATH_DEFINES
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <random>
#include <cstdlib>
#include <iostream>
#include "shader.h"
#include "shaderprogram.h"
#include <../../CImg-3.2.0/CImg.h>
#include <cstdio>
#include <stdlib.h>
#include "FastNoiseLite.h"
#include <vector>
#include <cmath>

#include<time.h>
#include<Windows.h>

static time_t now;

static unsigned int playermove;
static unsigned int playermovelr;
static unsigned int playermovefb;
static unsigned int playermovejump;

glm::vec3 pos;
glm::vec2 angles;

glm::vec3 strafe;
glm::vec3 presson;
glm::vec3 lookat;
glm::vec3 Up;

using namespace cimg_library;
/*=================================================================================================
	DOMAIN
=================================================================================================*/

// Window dimensions
const int InitWindowWidth = 800;
const int InitWindowHeight = 800;
int WindowWidth = InitWindowWidth;
int WindowHeight = InitWindowHeight;

// Last mouse cursor position
int LastMousePosX = 0;
int LastMousePosY = 0;

// Arrays that track which keys are currently pressed
bool key_states[256];
bool key_special_states[256];
bool mouse_states[8];

// Other parameters
bool draw_wireframe = false;

/*=================================================================================================
	SHADERS & TRANSFORMATIONS
=================================================================================================*/

ShaderProgram PassthroughShader;
ShaderProgram PerspectiveShader;

glm::mat4 PerspProjectionMatrix(1.0f);
glm::mat4 PerspViewMatrix(1.0f);
glm::mat4 PerspModelMatrix(1.0f);

float perspZoom = 1.0f, perspSensitivity = 0.35f;
float CamMoveX = 0.0f, CamMoveZ = 0.0f;
float perspRotationX = 0.0f, perspRotationY = 0.0f;

/*=================================================================================================
	OBJECTS
=================================================================================================*/

GLuint axis_VAO;
GLuint axis_VBO[5];
GLuint axis_VAO1;
GLuint axis_VBO1[5];
GLuint textureId;

CImg<unsigned char> texture;
unsigned char* rgb;
std::vector<float> noiseData(128 * 128);
std::vector <glm::vec4> vertex;
std::vector <glm::vec4> color;
std::vector <glm::vec2> texture_1;
std::vector <glm::vec4> vert_Normal;

std::vector <glm::vec4> lava_vertex;
std::vector <glm::vec4> lava_color;
std::vector <glm::vec2> lava_texture;
std::vector <glm::vec4> lava_normal;

uint8_t bloxs[33][33][33];

int baseline = 1;
int Food4Less = 0;
int ResetTimer = 0;

void faces(int i, int j, int k)
{
	//face 1
	vertex.push_back(glm::vec4(i, j, k, 1.0));
	vertex.push_back(glm::vec4(i + 1, j, k, 1.0));
	vertex.push_back(glm::vec4(i, j, k + 1, 1.0));
	vertex.push_back(glm::vec4(i, j, k + 1, 1.0));
	vertex.push_back(glm::vec4(i + 1, j, k, 1.0));
	vertex.push_back(glm::vec4(i + 1, j, k + 1, 1.0));
	//face 2
	vertex.push_back(glm::vec4(i, j + 1, k, 1.0));
	vertex.push_back(glm::vec4(i, j + 1, k + 1, 1.0));
	vertex.push_back(glm::vec4(i + 1, j + 1, k, 1.0));
	vertex.push_back(glm::vec4(i, j + 1, k + 1, 1.0));
	vertex.push_back(glm::vec4(i + 1, j + 1, k + 1, 1.0));
	vertex.push_back(glm::vec4(i + 1, j + 1, k, 1.0));
	//face 3
	vertex.push_back(glm::vec4(i, j, k, 1.0));
	vertex.push_back(glm::vec4(i, j, k + 1, 1.0));
	vertex.push_back(glm::vec4(i, j + 1, k, 1.0));
	vertex.push_back(glm::vec4(i, j + 1, k, 1.0));
	vertex.push_back(glm::vec4(i, j, k + 1, 1.0));
	vertex.push_back(glm::vec4(i, j + 1, k + 1, 1.0));
	//face 4
	vertex.push_back(glm::vec4(i + 1, j, k, 1.0));
	vertex.push_back(glm::vec4(i + 1, j + 1, k, 1.0));
	vertex.push_back(glm::vec4(i + 1, j, k + 1, 1.0));
	vertex.push_back(glm::vec4(i + 1, j + 1, k, 1.0));
	vertex.push_back(glm::vec4(i + 1, j + 1, k + 1, 1.0));
	vertex.push_back(glm::vec4(i + 1, j, k + 1, 1.0));
	//face 5
	vertex.push_back(glm::vec4(i, j, k, 1.0));
	vertex.push_back(glm::vec4(i + 1, j + 1, k, 1.0));
	vertex.push_back(glm::vec4(i + 1, j, k, 1.0));
	vertex.push_back(glm::vec4(i, j, k, 1.0));
	vertex.push_back(glm::vec4(i, j + 1, k, 1.0));
	vertex.push_back(glm::vec4(i + 1, j + 1, k, 1.0));
	//face 6
	vertex.push_back(glm::vec4(i, j, k + 1, 1.0));
	vertex.push_back(glm::vec4(i + 1, j, k + 1, 1.0));
	vertex.push_back(glm::vec4(i, j + 1, k + 1, 1.0));
	vertex.push_back(glm::vec4(i + 1, j, k + 1, 1.0));
	vertex.push_back(glm::vec4(i + 1, j + 1, k + 1, 1.0));
	vertex.push_back(glm::vec4(i, j + 1, k + 1, 1.0));
}

void treelog()
{
	//face 1 bot
	texture_1.push_back(glm::vec2(0.25f, 0.02941176471f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.02941176471f));
	texture_1.push_back(glm::vec2(0.25f, 0.05882352941f));
	texture_1.push_back(glm::vec2(0.25f, 0.05882352941f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.02941176471f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.05882352941f));
	//face 2 top
	texture_1.push_back(glm::vec2(0.2083333333f, 0.02941176471f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.05882352941f));
	texture_1.push_back(glm::vec2(0.25f, 0.02941176471f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.05882352941f));
	texture_1.push_back(glm::vec2(0.25f, 0.05882352941f));
	texture_1.push_back(glm::vec2(0.25f, 0.02941176471f));
	//face 3
	texture_1.push_back(glm::vec2(0.1666666667f, 0.05882352941f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.05882352941f));
	texture_1.push_back(glm::vec2(0.1666666667f, 0.02941176471f));
	texture_1.push_back(glm::vec2(0.1666666667f, 0.02941176471f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.05882352941f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.02941176471f));
	//face 4
	texture_1.push_back(glm::vec2(0.2083333333f, 0.05882352941f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.02941176471f));
	texture_1.push_back(glm::vec2(0.1666666667f, 0.05882352941f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.02941176471f));
	texture_1.push_back(glm::vec2(0.1666666667f, 0.02941176471f));
	texture_1.push_back(glm::vec2(0.1666666667f, 0.05882352941f));
	//face 5
	texture_1.push_back(glm::vec2(0.2083333333f, 0.05882352941f));
	texture_1.push_back(glm::vec2(0.1666666667f, 0.02941176471f));
	texture_1.push_back(glm::vec2(0.1666666667f, 0.05882352941f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.05882352941f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.02941176471f));
	texture_1.push_back(glm::vec2(0.1666666667f, 0.02941176471f));
	//face 6
	texture_1.push_back(glm::vec2(0.1666666667f, 0.05882352941f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.05882352941f));
	texture_1.push_back(glm::vec2(0.1666666667f, 0.02941176471f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.05882352941f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.02941176471f));
	texture_1.push_back(glm::vec2(0.1666666667f, 0.02941176471f));
}

void treeleaf()
{
	//face 1
	texture_1.push_back(glm::vec2(0.2083333333f, 0.08823529412f));
	texture_1.push_back(glm::vec2(0.1666666667f, 0.08823529412f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.1176470588f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.1176470588f));
	texture_1.push_back(glm::vec2(0.1666666667f, 0.08823529412f));
	texture_1.push_back(glm::vec2(0.1666666667f, 0.1176470588f));
	//face 2
	texture_1.push_back(glm::vec2(0.1666666667f, 0.08823529412f));
	texture_1.push_back(glm::vec2(0.1666666667f, 0.1176470588f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.08823529412f));
	texture_1.push_back(glm::vec2(0.1666666667f, 0.1176470588f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.1176470588f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.08823529412f));
	//face 3
	texture_1.push_back(glm::vec2(0.1666666667f, 0.1176470588f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.1176470588f));
	texture_1.push_back(glm::vec2(0.1666666667f, 0.08823529412f));
	texture_1.push_back(glm::vec2(0.1666666667f, 0.08823529412f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.1176470588f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.08823529412f));
	//face 4
	texture_1.push_back(glm::vec2(0.2083333333f, 0.1176470588f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.08823529412f));
	texture_1.push_back(glm::vec2(0.1666666667f, 0.1176470588f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.08823529412f));
	texture_1.push_back(glm::vec2(0.1666666667f, 0.08823529412f));
	texture_1.push_back(glm::vec2(0.1666666667f, 0.1176470588f));
	//face 5
	texture_1.push_back(glm::vec2(0.2083333333f, 0.1176470588f));
	texture_1.push_back(glm::vec2(0.1666666667f, 0.08823529412f));
	texture_1.push_back(glm::vec2(0.1666666667f, 0.1176470588f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.1176470588f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.08823529412f));
	texture_1.push_back(glm::vec2(0.1666666667f, 0.08823529412f));
	//face 6
	texture_1.push_back(glm::vec2(0.1666666667f, 0.1176470588f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.1176470588f));
	texture_1.push_back(glm::vec2(0.1666666667f, 0.08823529412f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.1176470588f));
	texture_1.push_back(glm::vec2(0.2083333333f, 0.08823529412f));
	texture_1.push_back(glm::vec2(0.1666666667f, 0.08823529412f));
}

void tree(float x, float y, float z)
{
	//tree log
	for (int i = 0; i < 5; i++)
	{
		faces(x, y + i, z);
		treelog();
	}

	//leaf on top of tree log
	faces(x, y + 5, z);
	treeleaf();

	//leaves under top leaf touching tree log
	for (int i = 4; i > 1; i--)
	{
		//front
		faces(x, y + i, z + 1);
		treeleaf();

		//back
		faces(x, y + i, z - 1);
		treeleaf();

		//left
		faces(x - 1, y + i, z);
		treeleaf();

		//right
		faces(x + 1, y + i, z);
		treeleaf();
	}
	//leaves on tree touching leaves
	for (int i = 1; i < 3; i++)
	{
		//corner front left
		faces(x - 1, y + 4 - i, z + 1);
		treeleaf();
		//corner front right
		faces(x + 1, y + 4 - i, z + 1);
		treeleaf();
		//corner back left
		faces(x - 1, y + 4 - i, z - 1);
		treeleaf();
		//corner back right
		faces(x + 1, y + 4 - i, z - 1);
		treeleaf();

		//front right
		faces(x + 1, y + 4 - i, z + 1 + 1);
		treeleaf();
		//front middle
		faces(x, y + 4 - i, z + 1 + 1);
		treeleaf();
		//front left
		faces(x - 1, y + 4 - i, z + 1 + 1);
		treeleaf();

		//back left
		faces(x + 1, y + 4 - i, z - 1 - 1);
		treeleaf();
		//back middle
		faces(x, y + 4 - i, z - 1 - 1);
		treeleaf();
		//back right
		faces(x - 1, y + 4 - i, z - 1 - 1);
		treeleaf();

		//left left
		faces(x - 1 - 1, y + 4 - i, z - 1);
		treeleaf();
		//left middle
		faces(x - 1 - 1, y + 4 - i, z);
		treeleaf();
		//left right
		faces(x - 1 - 1, y + 4 - i, z + 1);
		treeleaf();

		//right left
		faces(x + 1 + 1, y + 4 - i, z + 1);
		treeleaf();
		//right middle
		faces(x + 1 + 1, y + 4 - i, z);
		treeleaf();
		//right right
		faces(x + 1 + 1, y + 4 - i, z - 1);
		treeleaf();
	}
}


void clickturbo() {
	float MouseClickX = 5;
}

void cube()
{
	//Uses random include to generate random between a distribution of numbers 1-30
	//It is intended to help randomize elements of the terrain
	std::random_device arb; 
	std::mt19937 set(arb()); 
	std::uniform_int_distribution<> range(1, 30);

	//FastNoise is a infamous noise algorithm that generates random values with approximate range. I used it for terrain generation
	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

	// Gather noise data
	int index = 0;
	int check = 0;

	//Generates values from -1 to 1, used for 1-16 area by 16
	for (int x = 1; x < 17; x++)
	{
		for (int y = 1; y < 17; y++)
		{
			noiseData[index++] = noise.GetNoise((float)x, (float)y);
		}
	}

	//Sets the array to all 0, 0 is the equivalent to air
	memset(bloxs, 0, sizeof(bloxs));

	//These triple for loops help fill in these array with blocks and uses FastNoise to deside elevation
	for (int i = 1; i < 17; i++)
	{
		for (int k = 1; k < 17; k++)
		{
			for (int j = 1; j < 12 + (int)round(noiseData[check] * 5); j++)
			{
				if (j == 12 + (int)round(noiseData[check] * 5) - 1)
				{
					bloxs[i][j][k] = 1;  //grass block
					continue;
				}
				if ((j == 12 + (int)round(noiseData[check] * 5) - 2) || (j == 12 + (int)round(noiseData[check] * 5) - 3))
				{
					bloxs[i][j][k] = 2;  //dirt block
					continue;
				}
				//randomizes chances, makes certain outcomes happen more often
				int prob = range(set);
				if (prob < 27)
				{
					bloxs[i][j][k] = 3; //Stone block
				}
				else if (prob == 27)
				{
					bloxs[i][j][k] = 4; //Gold Ore block
				}
				else if (prob == 28)
				{
					bloxs[i][j][k] = 5; //Iron Ore block
				}
				else if (prob == 29)
				{
					bloxs[i][j][k] = 6; //Coal Ore block
				}
				else if (prob == 30)
				{
					bloxs[i][j][k] = 7; //Diamond Ore block
				}
			}
			check++;
		}
	}
	check = 0;
}

void lava() {
	//Helps construct the lava blocks and makes sure it gets its texture
	lava_vertex.push_back(glm::vec4(6, 15, 11, 1.0));
	lava_vertex.push_back(glm::vec4(7, 15, 11, 1.0));
	lava_vertex.push_back(glm::vec4(6, 15, 12, 1.0));
	lava_vertex.push_back(glm::vec4(6, 15, 12, 1.0));
	lava_vertex.push_back(glm::vec4(7, 15, 11, 1.0));
	lava_vertex.push_back(glm::vec4(7, 16, 12, 1.0));

	lava_vertex.push_back(glm::vec4(6, 16, 11, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 12, 1.0));
	lava_vertex.push_back(glm::vec4(7, 16, 11, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 12, 1.0));
	lava_vertex.push_back(glm::vec4(7, 16, 12, 1.0));
	lava_vertex.push_back(glm::vec4(7, 16, 11, 1.0));

	lava_vertex.push_back(glm::vec4(6, 15, 11, 1.0));
	lava_vertex.push_back(glm::vec4(6, 15, 12, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 11, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 11, 1.0));
	lava_vertex.push_back(glm::vec4(6, 15, 12, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 12, 1.0));

	lava_vertex.push_back(glm::vec4(7, 15, 11, 1.0));
	lava_vertex.push_back(glm::vec4(7, 16, 11, 1.0));
	lava_vertex.push_back(glm::vec4(7, 15, 12, 1.0));
	lava_vertex.push_back(glm::vec4(7, 16, 11, 1.0));
	lava_vertex.push_back(glm::vec4(7, 16, 12, 1.0));
	lava_vertex.push_back(glm::vec4(7, 15, 12, 1.0));

	lava_vertex.push_back(glm::vec4(6, 15, 11, 1.0));
	lava_vertex.push_back(glm::vec4(7, 16, 11, 1.0));
	lava_vertex.push_back(glm::vec4(7, 15, 11, 1.0));
	lava_vertex.push_back(glm::vec4(6, 15, 11, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 11, 1.0));
	lava_vertex.push_back(glm::vec4(7, 16, 11, 1.0));

	lava_vertex.push_back(glm::vec4(6, 15, 12, 1.0));
	lava_vertex.push_back(glm::vec4(7, 15, 12, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 12, 1.0));
	lava_vertex.push_back(glm::vec4(7, 15, 12, 1.0));
	lava_vertex.push_back(glm::vec4(7, 16, 12, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 12, 1.0));
	
	lava_vertex.push_back(glm::vec4(5, 15, 11, 1.0));
	lava_vertex.push_back(glm::vec4(6, 15, 11, 1.0));
	lava_vertex.push_back(glm::vec4(5, 15, 12, 1.0));
	lava_vertex.push_back(glm::vec4(5, 15, 12, 1.0));
	lava_vertex.push_back(glm::vec4(6, 15, 11, 1.0));
	lava_vertex.push_back(glm::vec4(5, 15, 12, 1.0));

	lava_vertex.push_back(glm::vec4(5, 16, 11, 1.0));
	lava_vertex.push_back(glm::vec4(5, 16, 12, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 11, 1.0));
	lava_vertex.push_back(glm::vec4(5, 16, 12, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 12, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 11, 1.0));

	lava_vertex.push_back(glm::vec4(5, 15, 11, 1.0));
	lava_vertex.push_back(glm::vec4(5, 15, 12, 1.0));
	lava_vertex.push_back(glm::vec4(5, 16, 11, 1.0));
	lava_vertex.push_back(glm::vec4(5, 16, 11, 1.0));
	lava_vertex.push_back(glm::vec4(5, 15, 12, 1.0));
	lava_vertex.push_back(glm::vec4(5, 16, 12, 1.0));

	lava_vertex.push_back(glm::vec4(6, 15, 11, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 11, 1.0));
	lava_vertex.push_back(glm::vec4(6, 15, 12, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 11, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 12, 1.0));
	lava_vertex.push_back(glm::vec4(6, 15, 12, 1.0));

	lava_vertex.push_back(glm::vec4(5, 15, 11, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 11, 1.0));
	lava_vertex.push_back(glm::vec4(6, 15, 11, 1.0));
	lava_vertex.push_back(glm::vec4(5, 15, 11, 1.0));
	lava_vertex.push_back(glm::vec4(5, 16, 11, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 11, 1.0));

	lava_vertex.push_back(glm::vec4(5, 15, 12, 1.0));
	lava_vertex.push_back(glm::vec4(6, 15, 12, 1.0));
	lava_vertex.push_back(glm::vec4(5, 16, 12, 1.0));
	lava_vertex.push_back(glm::vec4(6, 15, 12, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 12, 1.0));
	lava_vertex.push_back(glm::vec4(5, 16, 12, 1.0));

	lava_vertex.push_back(glm::vec4(5, 15, 12, 1.0));
	lava_vertex.push_back(glm::vec4(6, 15, 12, 1.0));
	lava_vertex.push_back(glm::vec4(5, 15, 13, 1.0));
	lava_vertex.push_back(glm::vec4(5, 15, 13, 1.0));
	lava_vertex.push_back(glm::vec4(6, 15, 12, 1.0));
	lava_vertex.push_back(glm::vec4(6, 15, 13, 1.0));

	lava_vertex.push_back(glm::vec4(5, 16, 12, 1.0));
	lava_vertex.push_back(glm::vec4(5, 16, 13, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 12, 1.0));
	lava_vertex.push_back(glm::vec4(5, 16, 13, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 13, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 12, 1.0));

	lava_vertex.push_back(glm::vec4(5, 15, 12, 1.0));
	lava_vertex.push_back(glm::vec4(5, 15, 13, 1.0));
	lava_vertex.push_back(glm::vec4(5, 16, 12, 1.0));
	lava_vertex.push_back(glm::vec4(5, 16, 12, 1.0));
	lava_vertex.push_back(glm::vec4(5, 15, 13, 1.0));
	lava_vertex.push_back(glm::vec4(5, 16, 13, 1.0));

	lava_vertex.push_back(glm::vec4(6, 15, 12, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 12, 1.0));
	lava_vertex.push_back(glm::vec4(6, 15, 13, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 12, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 13, 1.0));
	lava_vertex.push_back(glm::vec4(6, 15, 13, 1.0));

	lava_vertex.push_back(glm::vec4(5, 15, 12, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 12, 1.0));
	lava_vertex.push_back(glm::vec4(6, 15, 12, 1.0));
	lava_vertex.push_back(glm::vec4(5, 15, 12, 1.0));
	lava_vertex.push_back(glm::vec4(5, 16, 12, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 12, 1.0));

	lava_vertex.push_back(glm::vec4(5, 15, 13, 1.0));
	lava_vertex.push_back(glm::vec4(6, 15, 13, 1.0));
	lava_vertex.push_back(glm::vec4(5, 16, 13, 1.0));
	lava_vertex.push_back(glm::vec4(6, 15, 13, 1.0));
	lava_vertex.push_back(glm::vec4(6, 16, 13, 1.0));
	lava_vertex.push_back(glm::vec4(5, 16, 13, 1.0));
	

	for (int i = 0; i < 3; i++) {
		lava_texture.push_back(glm::vec2(0.49739583, 0.5294117647));
		lava_texture.push_back(glm::vec2(0.458333333f, 0.5294117647));
		lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));
		lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));
		lava_texture.push_back(glm::vec2(0.458333333f, 0.5294117647));
		lava_texture.push_back(glm::vec2(0.458333333f, 0.55698529));

		lava_texture.push_back(glm::vec2(0.458333333f, 0.5294117647));
		lava_texture.push_back(glm::vec2(0.458333333f, 0.55698529));
		lava_texture.push_back(glm::vec2(0.49739583, 0.5294117647));
		lava_texture.push_back(glm::vec2(0.458333333f, 0.55698529));
		lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));
		lava_texture.push_back(glm::vec2(0.49739583, 0.5294117647));

		lava_texture.push_back(glm::vec2(0.458333333f, 0.55698529));
		lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));
		lava_texture.push_back(glm::vec2(0.458333333f, 0.5294117647));
		lava_texture.push_back(glm::vec2(0.458333333f, 0.5294117647));
		lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));
		lava_texture.push_back(glm::vec2(0.49739583, 0.5294117647));

		lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));
		lava_texture.push_back(glm::vec2(0.49739583, 0.5294117647));
		lava_texture.push_back(glm::vec2(0.458333333f, 0.55698529));
		lava_texture.push_back(glm::vec2(0.49739583, 0.5294117647));
		lava_texture.push_back(glm::vec2(0.458333333f, 0.5294117647));
		lava_texture.push_back(glm::vec2(0.458333333f, 0.55698529));

		lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));
		lava_texture.push_back(glm::vec2(0.458333333f, 0.5294117647));
		lava_texture.push_back(glm::vec2(0.458333333f, 0.55698529));
		lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));
		lava_texture.push_back(glm::vec2(0.49739583, 0.5294117647));
		lava_texture.push_back(glm::vec2(0.458333333f, 0.5294117647));

		lava_texture.push_back(glm::vec2(0.458333333f, 0.55698529));
		lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));
		lava_texture.push_back(glm::vec2(0.458333333f, 0.5294117647));
		lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));
		lava_texture.push_back(glm::vec2(0.49739583, 0.5294117647));
		lava_texture.push_back(glm::vec2(0.458333333f, 0.5294117647));
	}

	//Gets the proper color for buffer and Cimg
	for (int i = 0; i < lava_vertex.size(); i++)
	{
		lava_color.push_back(glm::vec4(1.0, 1.0, 1.0, 1.0));
	}

	//Calculates the normals for the lava
	for (int i = 0; i < lava_vertex.size(); i += 3)
	{
		glm::vec3 A = lava_vertex[i];
		glm::vec3 B = lava_vertex[i + 1];
		glm::vec3 C = lava_vertex[i + 2];
		glm::vec3 AB = B - A;
		glm::vec3 AC = C - A;
		glm::vec3 ABxAC = (glm::cross(AB, AC));
		lava_normal.push_back(glm::vec4(ABxAC, 1.0f));
		lava_normal.push_back(glm::vec4(ABxAC, 1.0f));
		lava_normal.push_back(glm::vec4(ABxAC, 1.0f));
	}
}


void construct()
{
	// generate one tree at 16,16
	// max height is 12 + (int)round(noiseData[index] * 5
	// choose random index and add 1 to max height to generate tree on top of terrain
	tree(16, 13 + (int)round(noiseData[108] * 5), 16);
	int check = 0;
	//Assigns appropiate textures on array values as well only creating faces of a cube if it is non-zero and if it's non-blocked
	//This is done for optimization or else there would be too much vertex data and would slow the runtime and playtime
	//Accounts for culling and matches the cimg mapping
	for (int i = 1; i < 32; i++)
	{
		for (int k = 1; k < 32; k++)
		{
			for (int j = 1; j < 32; j++)
			{
				//face 1 of cube   
				if (!bloxs[i][j - 1][k] && bloxs[i][j][k])
				{
					vertex.push_back(glm::vec4(i, j, k, 1.0));      
					vertex.push_back(glm::vec4(i + 1, j, k, 1.0));  
					vertex.push_back(glm::vec4(i, j, k + 1, 1.0));
					vertex.push_back(glm::vec4(i, j, k + 1, 1.0));
					vertex.push_back(glm::vec4(i + 1, j, k, 1.0));
					vertex.push_back(glm::vec4(i + 1, j, k + 1, 1.0));

					if ((bloxs[i][j][k] == 2) || bloxs[i][j][k] == 1)
					{  //grass, dirt
						texture_1.push_back(glm::vec2(0.0833333f, 0.0f));
						texture_1.push_back(glm::vec2(0.122395833f, 0.0f));
						texture_1.push_back(glm::vec2(0.0833333f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.122395833f, 0.0f));
						texture_1.push_back(glm::vec2(0.0833333f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.122395833f, 0.0275735294f));
					}
					else if (bloxs[i][j][k] == 3)
					{ //stone
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0f));
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0f));
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0f));
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0275735294f));
					}
					else if (bloxs[i][j][k] == 4)
					{ // gold
						texture_1.push_back(glm::vec2(0.0390625f, 0.0588235294f));
						texture_1.push_back(glm::vec2(0.0f, 0.0588235294f));
						texture_1.push_back(glm::vec2(0.0390625f, 0.0863970588f));
						texture_1.push_back(glm::vec2(0.0390625f, 0.0863970588f));
						texture_1.push_back(glm::vec2(0.0f, 0.0588235294f));
						texture_1.push_back(glm::vec2(0.0f, 0.0863970588f));
					}
					else if (bloxs[i][j][k] == 5)
					{ // iron
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0588235294f));             
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0588235294f));             
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0863970588f));              
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0863970588f));             
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0588235294f));             
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0863970588f));             
					}
					else if (bloxs[i][j][k] == 6)
					{ // coal
						texture_1.push_back(glm::vec2(0.122395833f, 0.0588235294f));             
						texture_1.push_back(glm::vec2(0.0833333f, 0.0588235294f));             
						texture_1.push_back(glm::vec2(0.122395833f, 0.0863970588f));              
						texture_1.push_back(glm::vec2(0.122395833f, 0.0863970588f));             
						texture_1.push_back(glm::vec2(0.0833333f, 0.0588235294f));             
						texture_1.push_back(glm::vec2(0.0833333f, 0.0863970588f));             
					}
					else if (bloxs[i][j][k] == 7)
					{
						texture_1.push_back(glm::vec2(0.122395833f, 0.0882352941f));             
						texture_1.push_back(glm::vec2(0.0833333f, 0.0882352941f));             
						texture_1.push_back(glm::vec2(0.122395833f, 0.1158088235f));            
						texture_1.push_back(glm::vec2(0.122395833f, 0.1158088235f));           
						texture_1.push_back(glm::vec2(0.0833333f, 0.0882352941f));             
						texture_1.push_back(glm::vec2(0.0833333f, 0.1158088235f));             
					}
				}

				//face 2 of cube 
				if (!bloxs[i][j + 1][k] && bloxs[i][j][k])
				{
					vertex.push_back(glm::vec4(i, j + 1, k, 1.0));        
					vertex.push_back(glm::vec4(i, j + 1, k + 1, 1.0));
					vertex.push_back(glm::vec4(i + 1, j + 1, k, 1.0));
					vertex.push_back(glm::vec4(i, j + 1, k + 1, 1.0));
					vertex.push_back(glm::vec4(i + 1, j + 1, k + 1, 1.0));
					vertex.push_back(glm::vec4(i + 1, j + 1, k, 1.0));

					if (bloxs[i][j][k] == 1)
					{
						texture_1.push_back(glm::vec2(0.0390625f, 0.0f));
						texture_1.push_back(glm::vec2(0.0390625f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.0f, 0.0f));
						texture_1.push_back(glm::vec2(0.0390625f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.0390625f, 0.0f));
						texture_1.push_back(glm::vec2(0.0f, 0.0f));
					}
					else if (bloxs[i][j][k] == 2)
					{
						texture_1.push_back(glm::vec2(0.0833333f, 0.0f));
						texture_1.push_back(glm::vec2(0.0833333f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.122395833f, 0.0f));
						texture_1.push_back(glm::vec2(0.0833333f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.122395833f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.122395833f, 0.0f));
					}
					else if (bloxs[i][j][k] == 3)
					{
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0f));
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0f));
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0f));
					}
					else if (bloxs[i][j][k] == 4)
					{
						texture_1.push_back(glm::vec2(0.0f, 0.0588235294f));
						texture_1.push_back(glm::vec2(0.0f, 0.0863970588f));
						texture_1.push_back(glm::vec2(0.0390625f, 0.0588235294f));
						texture_1.push_back(glm::vec2(0.0f, 0.0863970588f));
						texture_1.push_back(glm::vec2(0.0390625f, 0.0863970588f));
						texture_1.push_back(glm::vec2(0.0390625f, 0.0588235294f));
					}
					else if (bloxs[i][j][k] == 5)
					{
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0588235294f));             
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0863970588f));             
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0588235294f));              
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0863970588f));             
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0863970588f));             
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0588235294f));             
					}
					else if (bloxs[i][j][k] == 6)
					{ // coal
						texture_1.push_back(glm::vec2(0.0833333f, 0.0588235294f));             
						texture_1.push_back(glm::vec2(0.0833333f, 0.0863970588f));             
						texture_1.push_back(glm::vec2(0.122395833f, 0.0588235294f));              
						texture_1.push_back(glm::vec2(0.0833333f, 0.0863970588f));             
						texture_1.push_back(glm::vec2(0.122395833f, 0.0863970588f));            
						texture_1.push_back(glm::vec2(0.122395833f, 0.0588235294f));            
					}
					else if (bloxs[i][j][k] == 7)
					{
						texture_1.push_back(glm::vec2(0.0833333f, 0.0882352941f));             
						texture_1.push_back(glm::vec2(0.0833333f, 0.1158088235f));             
						texture_1.push_back(glm::vec2(0.122395833f, 0.0882352941f));           
						texture_1.push_back(glm::vec2(0.0833333f, 0.1158088235f));             
						texture_1.push_back(glm::vec2(0.122395833f, 0.1158088235f));           
						texture_1.push_back(glm::vec2(0.122395833f, 0.0882352941f));           
					}
				}

				//face 3 of cube
				if (!bloxs[i - 1][j][k] && bloxs[i][j][k])
				{
					vertex.push_back(glm::vec4(i, j, k, 1.0));
					vertex.push_back(glm::vec4(i, j, k + 1, 1.0));
					vertex.push_back(glm::vec4(i, j + 1, k, 1.0));
					vertex.push_back(glm::vec4(i, j + 1, k, 1.0));
					vertex.push_back(glm::vec4(i, j, k + 1, 1.0));
					vertex.push_back(glm::vec4(i, j + 1, k + 1, 1.0));

					if (bloxs[i][j][k] == 1)
					{
						texture_1.push_back(glm::vec2(0.125f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.1640625f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.125f, 0.0f));
						texture_1.push_back(glm::vec2(0.125f, 0.0f));
						texture_1.push_back(glm::vec2(0.1640625f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.1640625f, 0.0f));
					}
					else if (bloxs[i][j][k] == 2)
					{
						texture_1.push_back(glm::vec2(0.0833333f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.122395833f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.0833333f, 0.0f));
						texture_1.push_back(glm::vec2(0.0833333f, 0.0f));
						texture_1.push_back(glm::vec2(0.122395833f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.122395833f, 0.0f));
					}
					else if (bloxs[i][j][k] == 3)
					{
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0f));
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0f));
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0f));
					}
					else if (bloxs[i][j][k] == 4)
					{
						texture_1.push_back(glm::vec2(0.0f, 0.0863970588f));
						texture_1.push_back(glm::vec2(0.0390625f, 0.0863970588f));
						texture_1.push_back(glm::vec2(0.0f, 0.0588235294f));
						texture_1.push_back(glm::vec2(0.0f, 0.0588235294f));
						texture_1.push_back(glm::vec2(0.0390625f, 0.0863970588f));
						texture_1.push_back(glm::vec2(0.0390625f, 0.0588235294f));
					}
					else if (bloxs[i][j][k] == 5)
					{
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0863970588f));             
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0863970588f));             
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0588235294f));              
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0588235294f));             
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0863970588f));             
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0588235294f));             
					}
					else if (bloxs[i][j][k] == 6)
					{ // coal
						texture_1.push_back(glm::vec2(0.0833333f, 0.0863970588f));             
						texture_1.push_back(glm::vec2(0.122395833f, 0.0863970588f));           
						texture_1.push_back(glm::vec2(0.0833333f, 0.0588235294f));              
						texture_1.push_back(glm::vec2(0.0833333f, 0.0588235294f));             
						texture_1.push_back(glm::vec2(0.122395833f, 0.0863970588f));           
						texture_1.push_back(glm::vec2(0.122395833f, 0.0588235294f));           
					}
					else if (bloxs[i][j][k] == 7)
					{
						texture_1.push_back(glm::vec2(0.0833333f, 0.1158088235f));             
						texture_1.push_back(glm::vec2(0.122395833f, 0.1158088235f));           
						texture_1.push_back(glm::vec2(0.0833333f, 0.0882352941f));              
						texture_1.push_back(glm::vec2(0.0833333f, 0.0882352941f));             
						texture_1.push_back(glm::vec2(0.122395833f, 0.1158088235f));           
						texture_1.push_back(glm::vec2(0.122395833f, 0.0882352941f));           
					}
				}

				//face 4 of cube
				if (!bloxs[i + 1][j][k] && bloxs[i][j][k])
				{
					vertex.push_back(glm::vec4(i + 1, j, k, 1.0));
					vertex.push_back(glm::vec4(i + 1, j + 1, k, 1.0));
					vertex.push_back(glm::vec4(i + 1, j, k + 1, 1.0));
					vertex.push_back(glm::vec4(i + 1, j + 1, k, 1.0));
					vertex.push_back(glm::vec4(i + 1, j + 1, k + 1, 1.0));
					vertex.push_back(glm::vec4(i + 1, j, k + 1, 1.0));

					if (bloxs[i][j][k] == 1)
					{
						texture_1.push_back(glm::vec2(0.1640625f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.1640625f, 0.0f));
						texture_1.push_back(glm::vec2(0.125f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.1640625f, 0.0f));
						texture_1.push_back(glm::vec2(0.125f, 0.0f));
						texture_1.push_back(glm::vec2(0.125f, 0.0275735294f));
					}
					else if (bloxs[i][j][k] == 2)
					{
						texture_1.push_back(glm::vec2(0.122395833f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.122395833f, 0.0f));
						texture_1.push_back(glm::vec2(0.0833333f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.122395833f, 0.0f));
						texture_1.push_back(glm::vec2(0.0833333f, 0.0f));
						texture_1.push_back(glm::vec2(0.0833333f, 0.0275735294f));
					}
					else if (bloxs[i][j][k] == 3)
					{
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0f));
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0f));
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0f));
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0275735294f));
					}
					else if (bloxs[i][j][k] == 4)
					{
						texture_1.push_back(glm::vec2(0.0390625f, 0.0863970588f));
						texture_1.push_back(glm::vec2(0.0390625f, 0.0588235294f));
						texture_1.push_back(glm::vec2(0.0f, 0.0863970588f));
						texture_1.push_back(glm::vec2(0.0390625f, 0.0588235294f));
						texture_1.push_back(glm::vec2(0.0f, 0.0588235294f));
						texture_1.push_back(glm::vec2(0.0f, 0.0863970588f));
					}
					else if (bloxs[i][j][k] == 5)
					{
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0863970588f));             
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0588235294f));             
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0863970588f));              
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0588235294f));             
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0588235294f));             
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0863970588f));             
					}
					else if (bloxs[i][j][k] == 6)
					{ // coal
						texture_1.push_back(glm::vec2(0.122395833f, 0.0863970588f));             
						texture_1.push_back(glm::vec2(0.122395833f, 0.0588235294f));             
						texture_1.push_back(glm::vec2(0.0833333f, 0.0863970588f));              
						texture_1.push_back(glm::vec2(0.122395833f, 0.0588235294f));             
						texture_1.push_back(glm::vec2(0.0833333f, 0.0588235294f));             
						texture_1.push_back(glm::vec2(0.0833333f, 0.0863970588f));             
					}
					else if (bloxs[i][j][k] == 7)
					{
						texture_1.push_back(glm::vec2(0.122395833f, 0.1158088235f));            
						texture_1.push_back(glm::vec2(0.122395833f, 0.0882352941f));            
						texture_1.push_back(glm::vec2(0.0833333f, 0.1158088235f));              
						texture_1.push_back(glm::vec2(0.122395833f, 0.0882352941f));           
						texture_1.push_back(glm::vec2(0.0833333f, 0.0882352941f));             
						texture_1.push_back(glm::vec2(0.0833333f, 0.1158088235f));             
					}
				}

				//face 5 of cube
				if (!bloxs[i][j][k - 1] && bloxs[i][j][k])
				{
					vertex.push_back(glm::vec4(i, j, k, 1.0));
					vertex.push_back(glm::vec4(i + 1, j + 1, k, 1.0));
					vertex.push_back(glm::vec4(i + 1, j, k, 1.0));
					vertex.push_back(glm::vec4(i, j, k, 1.0));
					vertex.push_back(glm::vec4(i, j + 1, k, 1.0));
					vertex.push_back(glm::vec4(i + 1, j + 1, k, 1.0));

					if (bloxs[i][j][k] == 1)
					{
						texture_1.push_back(glm::vec2(0.1640625f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.125f, 0.0f));
						texture_1.push_back(glm::vec2(0.125f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.1640625f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.1640625f, 0.0f));
						texture_1.push_back(glm::vec2(0.125f, 0.0f));
					}
					else if (bloxs[i][j][k] == 2)
					{
						texture_1.push_back(glm::vec2(0.122395833f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.0833333f, 0.0f));
						texture_1.push_back(glm::vec2(0.0833333f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.122395833f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.122395833f, 0.0f));
						texture_1.push_back(glm::vec2(0.0833333f, 0.0f));
					}
					else if (bloxs[i][j][k] == 3)
					{
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0f));
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0f));
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0f));
					}
					else if (bloxs[i][j][k] == 4)
					{
						texture_1.push_back(glm::vec2(0.0390625f, 0.0863970588f));
						texture_1.push_back(glm::vec2(0.0f, 0.0588235294f));
						texture_1.push_back(glm::vec2(0.0f, 0.0863970588f));
						texture_1.push_back(glm::vec2(0.0390625f, 0.0863970588f));
						texture_1.push_back(glm::vec2(0.0390625f, 0.0588235294f));
						texture_1.push_back(glm::vec2(0.0f, 0.0588235294f));
					}
					else if (bloxs[i][j][k] == 5)
					{
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0863970588f));             
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0588235294f));             
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0863970588f));              
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0863970588f));                
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0588235294f));             
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0588235294f));             
					}
					else if (bloxs[i][j][k] == 6)
					{ // coal
						texture_1.push_back(glm::vec2(0.122395833f, 0.0863970588f));            
						texture_1.push_back(glm::vec2(0.0833333f, 0.0588235294f));             
						texture_1.push_back(glm::vec2(0.0833333f, 0.0863970588f));              
						texture_1.push_back(glm::vec2(0.122395833f, 0.0863970588f));            
						texture_1.push_back(glm::vec2(0.122395833f, 0.0588235294f));            
						texture_1.push_back(glm::vec2(0.0833333f, 0.0588235294f));             
					}
					else if (bloxs[i][j][k] == 7)
					{
						texture_1.push_back(glm::vec2(0.122395833f, 0.1158088235f));            
						texture_1.push_back(glm::vec2(0.0833333f, 0.0882352941f));             
						texture_1.push_back(glm::vec2(0.0833333f, 0.1158088235f));              
						texture_1.push_back(glm::vec2(0.122395833f, 0.1158088235f));           
						texture_1.push_back(glm::vec2(0.122395833f, 0.0882352941f));           
						texture_1.push_back(glm::vec2(0.0833333f, 0.0882352941f));             
					}
				}


				//face 6 of cube
				if (!bloxs[i][j][k + 1] && bloxs[i][j][k])
				{
					vertex.push_back(glm::vec4(i, j, k + 1, 1.0));
					vertex.push_back(glm::vec4(i + 1, j, k + 1, 1.0));
					vertex.push_back(glm::vec4(i, j + 1, k + 1, 1.0));
					vertex.push_back(glm::vec4(i + 1, j, k + 1, 1.0));
					vertex.push_back(glm::vec4(i + 1, j + 1, k + 1, 1.0));
					vertex.push_back(glm::vec4(i, j + 1, k + 1, 1.0));

					if (bloxs[i][j][k] == 1)
					{
						texture_1.push_back(glm::vec2(0.125f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.1640625f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.125f, 0.0f));
						texture_1.push_back(glm::vec2(0.1640625f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.1640625f, 0.0f));
						texture_1.push_back(glm::vec2(0.125f, 0.0f));
					}
					else if (bloxs[i][j][k] == 2)
					{
						texture_1.push_back(glm::vec2(0.0833333f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.122395833f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.0833333f, 0.0f));
						texture_1.push_back(glm::vec2(0.122395833f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.122395833f, 0.0f));
						texture_1.push_back(glm::vec2(0.0833333f, 0.0f));
					}
					else if (bloxs[i][j][k] == 3)
					{
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0f));
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0275735294f));
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0f));
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0f));
					}
					else if (bloxs[i][j][k] == 4)
					{
						texture_1.push_back(glm::vec2(0.0f, 0.0863970588f));
						texture_1.push_back(glm::vec2(0.0390625f, 0.0863970588f));
						texture_1.push_back(glm::vec2(0.0f, 0.0588235294f));
						texture_1.push_back(glm::vec2(0.0390625f, 0.0863970588f));
						texture_1.push_back(glm::vec2(0.0390625f, 0.0588235294f));
						texture_1.push_back(glm::vec2(0.0f, 0.0588235294f));
					}
					else if (bloxs[i][j][k] == 5)
					{
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0863970588f));             
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0863970588f));             
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0588235294f));              
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0863970588f));             
						texture_1.push_back(glm::vec2(0.0807291667f, 0.0588235294f));             
						texture_1.push_back(glm::vec2(0.0416666667f, 0.0588235294f));             
					}
					else if (bloxs[i][j][k] == 6)
					{ // coal
						texture_1.push_back(glm::vec2(0.0833333f, 0.0863970588f));             
						texture_1.push_back(glm::vec2(0.122395833f, 0.0863970588f));           
						texture_1.push_back(glm::vec2(0.0833333f, 0.0588235294f));              
						texture_1.push_back(glm::vec2(0.122395833f, 0.0863970588f));           
						texture_1.push_back(glm::vec2(0.122395833f, 0.0588235294f));           
						texture_1.push_back(glm::vec2(0.0833333f, 0.0588235294f));             
					}
					else if (bloxs[i][j][k] == 7)
					{
						texture_1.push_back(glm::vec2(0.0833333f, 0.1158088235f));             
						texture_1.push_back(glm::vec2(0.122395833f, 0.1158088235f));
						texture_1.push_back(glm::vec2(0.0833333f, 0.0882352941f));
						texture_1.push_back(glm::vec2(0.122395833f, 0.1158088235f));
						texture_1.push_back(glm::vec2(0.122395833f, 0.0882352941f));
						texture_1.push_back(glm::vec2(0.0833333f, 0.0882352941f));
					}
				}
			}
			check++;
		}
	}
	//Gives the appropiate white for Cimg and the Shader
	for (int i = 0; i < vertex.size(); i++)
	{
		color.push_back(glm::vec4(1.0, 1.0, 1.0, 1.0));
	}
	//Calculates the normals for flat shading
	for (int i = 0; i < vertex.size(); i += 3)
	{
		glm::vec3 A = vertex[i];
		glm::vec3 B = vertex[i + 1];
		glm::vec3 C = vertex[i + 2];
		glm::vec3 AB = B - A;
		glm::vec3 AC = C - A;
		glm::vec3 ABxAC = (glm::cross(AB, AC));
		vert_Normal.push_back(glm::vec4(ABxAC, 1.0f));
		vert_Normal.push_back(glm::vec4(ABxAC, 1.0f));
		vert_Normal.push_back(glm::vec4(ABxAC, 1.0f));
	}
}


/*=================================================================================================
	HELPER FUNCTIONS
=================================================================================================*/

void window_to_scene(int wx, int wy, float& sx, float& sy)
{
	sx = (2.0f * (float)wx / WindowWidth) - 1.0f;
	sy = 1.0f - (2.0f * (float)wy / WindowHeight);
}

/*=================================================================================================
	SHADERS
=================================================================================================*/

void CreateTransformationMatrices(void)
{
	PerspModelMatrix = glm::scale(PerspModelMatrix, glm::vec3(perspZoom));
}

void CreateShaders(void)
{
	// Renders without any transformations
	PassthroughShader.Create("./shaders/simple.vert", "./shaders/simple.frag");

	// Renders using perspective projection
	PerspectiveShader.Create("./shaders/texpersplight.vert", "./shaders/texpersplight.frag");
}

/*=================================================================================================
	BUFFERS
=================================================================================================*/

void CreateAxisBuffers(void)
{
	//Corrects for the RGB values in Cimg
	int size = texture.width() * texture.height();
	unsigned char* rgb = new unsigned char[3 * size];
	for (int i = 0; i < size; i++) {
		rgb[3 * i + 0] = texture.data()[0 * size + i];
		rgb[3 * i + 1] = texture.data()[1 * size + i];
		rgb[3 * i + 2] = texture.data()[2 * size + i];
	}

	//Sends the proper information to the shader
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width(), texture.height(), 0,
		GL_RGB, GL_UNSIGNED_BYTE, rgb);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glGenVertexArrays(1, &axis_VAO);
	glBindVertexArray(axis_VAO);

	glGenBuffers(4, &axis_VBO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(glm::vec4), &vertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(glm::vec4), &color[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, vert_Normal.size() * sizeof(glm::vec4), &vert_Normal[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, texture_1.size() * sizeof(glm::vec2), &texture_1[0], GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}

//A seperate shader for the lava animation, so the runtime wouldn't be hindered by repeatdly redisplaying loads of vertex data

void Buffers() {
	glGenVertexArrays(1, &axis_VAO1);
	glBindVertexArray(axis_VAO1);

	glGenBuffers(4, &axis_VBO1[0]);

	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO1[0]);
	glBufferData(GL_ARRAY_BUFFER, lava_vertex.size() * sizeof(glm::vec4), &lava_vertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO1[1]);
	glBufferData(GL_ARRAY_BUFFER, lava_color.size() * sizeof(glm::vec4), &lava_color[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO1[2]);
	glBufferData(GL_ARRAY_BUFFER, lava_normal.size() * sizeof(glm::vec4), &lava_normal[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO1[3]);
	glBufferData(GL_ARRAY_BUFFER, lava_texture.size() * sizeof(glm::vec2), &lava_texture[0], GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}

//animation focuses on the swapping values for the textures
void animation() {
	lava_texture.clear();

	if (Food4Less == 0) {
		Food4Less = 1;
	}
	else {
		Food4Less = 0;
	}

	if (Food4Less == 1) {
		for (int i = 0; i < 3; i++) {
			lava_texture.push_back(glm::vec2(0.49739583, 0.5422794118));                   
			lava_texture.push_back(glm::vec2(0.458333333f, 0.5422794118));                 
			lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));                     
			lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));                     
			lava_texture.push_back(glm::vec2(0.458333333f, 0.5422794118));                 
			lava_texture.push_back(glm::vec2(0.458333333f, 0.55698529));                   

			lava_texture.push_back(glm::vec2(0.458333333f, 0.5422794118));           
			lava_texture.push_back(glm::vec2(0.458333333f, 0.55698529));             
			lava_texture.push_back(glm::vec2(0.49739583, 0.5422794118));              
			lava_texture.push_back(glm::vec2(0.458333333f, 0.55698529));             
			lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));               
			lava_texture.push_back(glm::vec2(0.49739583, 0.5422794118));             

			lava_texture.push_back(glm::vec2(0.458333333f, 0.55698529));             
			lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));               
			lava_texture.push_back(glm::vec2(0.458333333f, 0.5422794118));            
			lava_texture.push_back(glm::vec2(0.458333333f, 0.5422794118));           
			lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));               
			lava_texture.push_back(glm::vec2(0.49739583, 0.5422794118));             

			lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));                 
			lava_texture.push_back(glm::vec2(0.49739583, 0.5422794118));               
			lava_texture.push_back(glm::vec2(0.458333333f, 0.55698529));                
			lava_texture.push_back(glm::vec2(0.49739583, 0.5422794118));               
			lava_texture.push_back(glm::vec2(0.458333333f, 0.5422794118));             
			lava_texture.push_back(glm::vec2(0.458333333f, 0.55698529));               

			lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));
			lava_texture.push_back(glm::vec2(0.458333333f, 0.5422794118));
			lava_texture.push_back(glm::vec2(0.458333333f, 0.55698529));
			lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));
			lava_texture.push_back(glm::vec2(0.49739583, 0.5422794118));
			lava_texture.push_back(glm::vec2(0.458333333f, 0.5422794118));

			lava_texture.push_back(glm::vec2(0.458333333f, 0.55698529));
			lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));
			lava_texture.push_back(glm::vec2(0.458333333f, 0.5422794118));
			lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));
			lava_texture.push_back(glm::vec2(0.49739583, 0.5422794118));
			lava_texture.push_back(glm::vec2(0.458333333f, 0.5422794118));
		}
	}
	else {
		for (int i = 0; i < 3; i++) {
			lava_texture.push_back(glm::vec2(0.49739583, 0.5294117647));                   
			lava_texture.push_back(glm::vec2(0.458333333f, 0.5294117647));                 
			lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));                     
			lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));                     
			lava_texture.push_back(glm::vec2(0.458333333f, 0.5294117647));                 
			lava_texture.push_back(glm::vec2(0.458333333f, 0.55698529));                   

			lava_texture.push_back(glm::vec2(0.458333333f, 0.5294117647));           
			lava_texture.push_back(glm::vec2(0.458333333f, 0.55698529));             
			lava_texture.push_back(glm::vec2(0.49739583, 0.5294117647));              
			lava_texture.push_back(glm::vec2(0.458333333f, 0.55698529));             
			lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));               
			lava_texture.push_back(glm::vec2(0.49739583, 0.5294117647));             

			lava_texture.push_back(glm::vec2(0.458333333f, 0.55698529));             
			lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));               
			lava_texture.push_back(glm::vec2(0.458333333f, 0.5294117647));            
			lava_texture.push_back(glm::vec2(0.458333333f, 0.5294117647));           
			lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));               
			lava_texture.push_back(glm::vec2(0.49739583, 0.5294117647));             

			lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));                 
			lava_texture.push_back(glm::vec2(0.49739583, 0.5294117647));               
			lava_texture.push_back(glm::vec2(0.458333333f, 0.55698529));                
			lava_texture.push_back(glm::vec2(0.49739583, 0.5294117647));               
			lava_texture.push_back(glm::vec2(0.458333333f, 0.5294117647));             
			lava_texture.push_back(glm::vec2(0.458333333f, 0.55698529));               

			lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));
			lava_texture.push_back(glm::vec2(0.458333333f, 0.5294117647));
			lava_texture.push_back(glm::vec2(0.458333333f, 0.55698529));
			lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));
			lava_texture.push_back(glm::vec2(0.49739583, 0.5294117647));
			lava_texture.push_back(glm::vec2(0.458333333f, 0.5294117647));

			lava_texture.push_back(glm::vec2(0.458333333f, 0.55698529));
			lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));
			lava_texture.push_back(glm::vec2(0.458333333f, 0.5294117647));
			lava_texture.push_back(glm::vec2(0.49739583, 0.55698529));
			lava_texture.push_back(glm::vec2(0.49739583, 0.5294117647));
			lava_texture.push_back(glm::vec2(0.458333333f, 0.5294117647));
		}
	}
	Buffers();
}

/*=================================================================================================
	CALLBACKS
=================================================================================================*/

//-----------------------------------------------------------------------------
// CALLBACK DOCUMENTATION
// https://www.opengl.org/resources/libraries/glut/spec3/node45.html
// http://freeglut.sourceforge.net/docs/api.php#WindowCallback
//-----------------------------------------------------------------------------

static void update_vectors() {
	presson.x = sinf(angles.x);
	presson.y = 0;
	presson.z = cosf(angles.x);

	strafe.x = -cosf(angles.x);
	strafe.y = 0;
	strafe.z = sinf(angles.x);

	lookat.x = sinf(angles.x) * cosf(angles.y);
	lookat.y = sinf(angles.y);
	lookat.z = cosf(angles.x) * cosf(angles.y);

	Up = glm::cross(strafe, lookat);
}

void idle_func()
{
	static int pt = 0;
	static int ft = 0;
	static const float movespeed = 5.0f;

	now = time(0);
	int t = glutGet(GLUT_ELAPSED_TIME);

	float dt = (t - pt) * 1.0e-3;
	pt = t;

	float jt = (t - ft) * 1.0e-3;
	ft = t;

	if (playermovelr & 1) {
		pos -= strafe * movespeed * dt;
	}
	if (playermovelr & 2) {
		pos += strafe * movespeed * dt;
	}
	if (playermovefb & 4) {
		pos += presson * movespeed * dt;
	}
	if (playermovefb & 8) {
		pos -= presson * movespeed * dt;
	}
	if (playermovejump & 16) {

		if (jt < 0.5) {
			pos.y += movespeed * jt;
		} if (jt >= 0.5) {
			playermovejump = 64;
		}
	}
	if (playermovejump & 32) {
		pos.y -= movespeed * dt;
	}
	if (playermovejump & 64) {
		if (jt > 0.0) {
			pos.y -= movespeed * jt;
		} if (jt <= 0.0) {
			playermovejump &= 64;
		}
	}
	glutPostRedisplay();

}

void reshape_func(int width, int height)
{
	WindowWidth = width;
	WindowHeight = height;

	glViewport(0, 0, width, height);
	glutPostRedisplay();
}

void keyboard_func(unsigned char key, int x, int y)
{
	key_states[key] = true;
	switch (key)
	{

	case 'a':
	{
		playermovelr = 1;
		break;
	}

	case 'd':
	{
		playermovelr = 2;
		break;
	}

	case 'w':
	{
		playermovefb = 4;
		break;
	}

	case 's':
	{
		playermovefb = 8;
		break;
	}
	case '1':
	{
		baseline = 1;
		break;
	}
	case '2':
	{
		baseline = 2;
		break;
	}
	case '3':
	{
		baseline = 3;
		break;
	}
	case '4':
	{
		baseline = 4;
		break;
	}
	case '5':
	{
		baseline = 5;
		break;
	}
	case '6':
	{
		baseline = 6;
		break;
	}
	case '7':
	{
		baseline = 7;
		break;
	}
	case ' ':
	{
		playermovejump = 16;
		break;
	}

	case 'f':
	{
		playermovejump = 32;
		break;
	}

	// Exit on escape key press
	case '\x1B':
	{
		exit(EXIT_SUCCESS);
		break;
	}
	}
}

void key_released(unsigned char key, int x, int y)
{
	key_states[key] = false;
	switch (key) {
	case 'a': {
		playermovelr &= ~1;
		break;
	}
	case 'd': {
		playermovelr &= ~2;
		break;
	}
	case 'w': {
		playermovefb &= ~4;
		break;
	}
	case 's': {
		playermovefb &= ~8;
		break;
	}

	case ' ': {
		playermovejump &= ~16;
		break;
	}
	case 'f': {
		playermovejump &= ~32;
		break;
	}
	}
}



static void key_special_pressed(int key, int x, int y)
{
	key_special_states[key] = true;

	switch (key) {

	}
}




static void key_special_released(int key, int x, int y)
{
	key_special_states[key] = false;
	switch (key) {

	case 'a': {
		playermove = 0;
		break;
	}
	case 'd': {
		playermove = 0;
		break;
	}
	case 'w': {
		playermove = 0;
		break;
	}
	case 's': {
		playermove = 0;
		break;
	}
	}
}

void mouse_func(int button, int state, int x, int y)
{
	// Key 0: left button
	// Key 1: middle button
	// Key 2: right button
	// Key 3: scroll up
	// Key 4: scroll down

	if (x < 0 || x > WindowWidth || y < 0 || y > WindowHeight)
		return;

	float px, py;
	window_to_scene(x, y, px, py);

	if (button == 3)
	{
		perspZoom += 0.03f;
	}
	else if (button == 4)
	{
		if (perspZoom - 0.03f > 0.0f)
			perspZoom -= 0.03f;
	}

	mouse_states[button] = (state == GLUT_DOWN);

	LastMousePosX = x;
	LastMousePosY = y;

	//Removes blocks through left click, changes values of array
	if (button == 0 && state == GLUT_DOWN) {
		//We need to find depth in order to Ray Cast
		GLfloat depth;
		//We'll need to read the proper pixel value
		glReadPixels(x, WindowHeight - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
		//Following proper object coordinates we must find the viewport, window coordinates, Perspective, and Model info
		glm::vec4 viewport = glm::vec4(0, 0, WindowWidth, WindowHeight);
		glm::vec3 win = glm::vec3(x, WindowHeight - y - 1, depth);
		glm::vec3 ObjectVert = glm::unProject(win, PerspViewMatrix, PerspProjectionMatrix, viewport);


		ObjectVert.x += 0.003783;
		ObjectVert.y += 0.003783;
		ObjectVert.z += 0.003783;


		//Place conditions to filter for the plane we click on, then we set that block to an air block
		if ((ObjectVert.y - floor(ObjectVert.y)) > 0.003783 && ObjectVert.x - floor(ObjectVert.x) > ObjectVert.z - floor(ObjectVert.z)) {
			if (lookat.z > 0) {
				bloxs[(int)ObjectVert.x][(int)ObjectVert.y][(int)ObjectVert.z] = 0;
			}
			else {
				bloxs[(int)ObjectVert.x][(int)ObjectVert.y][(int)ObjectVert.z - 1] = 0;
			}
		}
		else if ((ObjectVert.y - floor(ObjectVert.y)) > 0.003783 && ObjectVert.x - floor(ObjectVert.x) < ObjectVert.z - floor(ObjectVert.z)) {
			if (lookat.x > 0) {
				bloxs[(int)ObjectVert.x][(int)ObjectVert.y][(int)ObjectVert.z] = 0;
			}
			else {
				bloxs[(int)ObjectVert.x - 1][(int)ObjectVert.y][(int)ObjectVert.z] = 0;
			}
		}
		else {
			if (lookat.y < 0) {
				bloxs[(int)ObjectVert.x][(int)ObjectVert.y - 1][(int)ObjectVert.z] = 0;
			}
			else {
				bloxs[(int)ObjectVert.x][(int)ObjectVert.y][(int)ObjectVert.z] = 0;
			}
		}
		//We clear the buffer info, recall buffer to demostrarte new changes
		vertex.clear();
		color.clear();
		texture_1.clear();
		vert_Normal.clear();
		construct();
		CreateAxisBuffers();
	}
	//Adds blocks through left click, changes values of array
	if (button == 2 && state == GLUT_DOWN) {
        //Refer to previous button about glm::unproject
		GLfloat depth;

		glReadPixels(x, WindowHeight - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
		glm::vec4 viewport = glm::vec4(0, 0, WindowWidth, WindowHeight);
		glm::vec3 win = glm::vec3(x, WindowHeight - y - 1, depth);
		glm::vec3 ObjectVert = glm::unProject(win, PerspViewMatrix, PerspProjectionMatrix, viewport);


		ObjectVert.x += 0.003783;
		ObjectVert.y += 0.003783;
		ObjectVert.z += 0.003783;

		//Place conditions to filter for the plane we click on, then we set that block to a block type of our choice
		if ((ObjectVert.y - floor(ObjectVert.y)) > 0.003783 && ObjectVert.x - floor(ObjectVert.x) > ObjectVert.z - floor(ObjectVert.z)) {
			if (lookat.z > 0) {
				bloxs[(int)ObjectVert.x][(int)ObjectVert.y][(int)ObjectVert.z - 1] = baseline;
			}
			else {
				bloxs[(int)ObjectVert.x][(int)ObjectVert.y][(int)ObjectVert.z] = baseline;
			}
		}
		else if ((ObjectVert.y - floor(ObjectVert.y)) > 0.003783 && ObjectVert.x - floor(ObjectVert.x) < ObjectVert.z - floor(ObjectVert.z)) {
			if (lookat.x > 0) {
				bloxs[(int)ObjectVert.x - 1][(int)ObjectVert.y][(int)ObjectVert.z] = baseline;
			}
			else {
				bloxs[(int)ObjectVert.x][(int)ObjectVert.y][(int)ObjectVert.z] = baseline;
			}
		}
		else {
			if (lookat.y < 0) {
				bloxs[(int)ObjectVert.x][(int)ObjectVert.y][(int)ObjectVert.z] = baseline;
			}
			else {
				bloxs[(int)ObjectVert.x][(int)ObjectVert.y - 1][(int)ObjectVert.z] = baseline;
			}
		}
		//Clear buffer info and calls buffer to implement changes
		vertex.clear();
		color.clear();
		texture_1.clear();
		vert_Normal.clear();
		construct();
		CreateAxisBuffers();
	}
}

void passive_motion_func(int x, int y)
{
	static bool wrap = false;
	const float mousespeed = 0.0015f;

	if (!wrap) {

		angles.x -= (x - WindowWidth / 2) * mousespeed;
		angles.y -= (y - WindowHeight / 2) * mousespeed;

		if (angles.x < -M_PI) {
			angles.x += M_PI * 2;
		}
		else if (angles.x > M_PI) {
			angles.x -= M_PI * 2;
		}

		if (angles.y < -M_PI / 2) {
			angles.y = -M_PI / 2;
		}
		else if (angles.y > M_PI / 2) {
			angles.y = M_PI / 2;
		}
		wrap = true;
		glutWarpPointer(WindowWidth / 2, WindowHeight / 2);
	}
	else {
		wrap = false;
	}
}

void active_motion_func(int x, int y)
{
	if (x < 0 || x > WindowWidth || y < 0 || y > WindowHeight)
		return;

	float px, py;
	window_to_scene(x, y, px, py);

	if (mouse_states[0] == true)
	{
		perspRotationY += (x - LastMousePosX) * perspSensitivity;
		perspRotationX += (y - LastMousePosY) * perspSensitivity;
	}

	LastMousePosX = x;
	LastMousePosY = y;
}

/*=================================================================================================
	RENDERING
=================================================================================================*/

void display_func(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	PerspViewMatrix = glm::lookAt(pos, pos + lookat, Up);
	PerspProjectionMatrix = glm::perspective(45.0f, 1.0f * WindowWidth / WindowHeight, 1.0f, 100.0f);

	CreateTransformationMatrices();

	PerspectiveShader.Use();
	PerspectiveShader.SetUniform("projectionMatrix", glm::value_ptr(PerspProjectionMatrix), 4, GL_FALSE, 1);
	PerspectiveShader.SetUniform("viewMatrix", glm::value_ptr(PerspViewMatrix), 4, GL_FALSE, 1);
	PerspectiveShader.SetUniform("modelMatrix", glm::value_ptr(PerspModelMatrix), 4, GL_FALSE, 1);
	PerspectiveShader.SetUniform("texId", textureId);

	if (draw_wireframe == true)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	//A timer for the animation, roughly 10 seconds
	ResetTimer++;
	if (ResetTimer == 18697) {
		animation();
		ResetTimer = 0;
	}

	glBindVertexArray(axis_VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertex.size());

	glBindVertexArray(0);

	//Seperate Draw call for the lava
	glBindVertexArray(axis_VAO1);
	glDrawArrays(GL_TRIANGLES, 0, lava_vertex.size());

	glBindVertexArray(0);

	if (draw_wireframe == true)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	update_vectors();

	glutSwapBuffers();
}

/*=================================================================================================
	INIT
=================================================================================================*/

void init(void)
{
	// Print some info
	std::cout << "Vendor:         " << glGetString(GL_VENDOR) << "\n";
	std::cout << "Renderer:       " << glGetString(GL_RENDERER) << "\n";
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
	std::cout << "GLSL Version:   " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n\n";

	// Set OpenGL settings
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // background color
	glEnable(GL_DEPTH_TEST); // enable depth test
	glEnable(GL_CULL_FACE); // enable back-face culling

	// Create shaders
	CreateShaders();

	pos = glm::vec3(7, 17, 7);
	angles = glm::vec3(0, -0.5, 0);
	update_vectors();

	// Create buffers
	CreateAxisBuffers();
	Buffers();

	std::cout << "Finished initializing...\n\n";

	std::cout << "Hello! This project is brought to you by Salvador Ruiz-Ramirez, Isaac Perez, Benjamin Hoang, and Wilfried Aubin Ngongang Yomba. We've spent a lot time and hard effort. We hope you enjoy our minecraft like clone. Here are the controls and interations: \n\n";
	std::cout << "Controls:" << std::endl;
	std::cout << "Hold W to move Forward" << std::endl;
	std::cout << "Hold S to move Backward" << std::endl;
	std::cout << "Hold A to move Left" << std::endl;
	std::cout << "Hold D to move Right" << std::endl;
	std::cout << "Hold F to make the camera fly down" << std::endl;
	std::cout << "Hold Spacebar to make the camera fly up" << std::endl;
	std::cout << "The camera movement is controled through your mouse/trackpad movement" << std::endl << std::endl;

	std::cout << "Interaction:" << std::endl;
	std::cout << "Left Click on Mouse/Trackpad removes the block you are facing" << std::endl;
	std::cout << "Right Click on Mouse/Trackpad adds a block type alongside a block surafce that was clicked (default grass block)" << std::endl;
	std::cout << "Number Key 1: Grass Block" << std::endl;
	std::cout << "Number Key 2: Dirt Block" << std::endl;
	std::cout << "Number Key 3: Stone Block" << std::endl;
	std::cout << "Number Key 4: Gold Block" << std::endl;
	std::cout << "Number Key 5: Iron Ore Block" << std::endl;
	std::cout << "Number Key 6: Coal Ore Block" << std::endl;
	std::cout << "Number Key 7: Diamond Ore Block" << std::endl;

}

/*=================================================================================================
	MAIN
=================================================================================================*/

int main(int argc, char** argv)
{
	//Loads texture bmp
	texture.load("spritesheet.bmp");
	//Finds Cube details
	cube();
	//Constructs info for lava
	lava();
	//Constructs the data for the terrain, with positon vertex, textures, normals, and color
	construct();

	glutInit(&argc, argv);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(InitWindowWidth, InitWindowHeight);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutCreateWindow("CSE-170 Computer Graphics");

	// Initialize GLEW
	GLenum ret = glewInit();
	if (ret != GLEW_OK) {
		std::cerr << "GLEW initialization error." << std::endl;
		glewGetErrorString(ret);
		return -1;
	}

	glutDisplayFunc(display_func);
	glutIdleFunc(idle_func);
	glutReshapeFunc(reshape_func);
	glutKeyboardFunc(keyboard_func);
	glutKeyboardUpFunc(key_released);
	glutSpecialFunc(key_special_pressed);
	glutSpecialUpFunc(key_special_released);
	glutMouseFunc(mouse_func);
	glutMotionFunc(active_motion_func);
	glutPassiveMotionFunc(passive_motion_func);

	init();

	glutMainLoop();

	return EXIT_SUCCESS;
}
