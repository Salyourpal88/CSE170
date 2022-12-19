#define _USE_MATH_DEFINES 
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include "shader.h"
#include "shaderprogram.h"
#include <../../CImg-3.2.0/CImg.h>
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
ShaderProgram TexperspShader;
ShaderProgram TexpersplightShader;

glm::mat4 PerspProjectionMatrix(1.0f);
glm::mat4 PerspViewMatrix(1.0f);
glm::mat4 PerspModelMatrix(1.0f);

float perspZoom = 1.0f, perspSensitivity = 0.35f;
float perspRotationX = 0.0f, perspRotationY = 0.0f;

/*=================================================================================================
	OBJECTS
=================================================================================================*/
GLuint axis_VAO1;
GLuint axis_VBO1[4];
GLuint textureId;
std::vector <glm::vec4> points;
std::vector <glm::vec4> white;
std::vector <glm::vec4> Origin;
std::vector <glm::vec2> tex_1;

CImg<unsigned char> texture;


float n = 12;
float R = 0.5;
float r = 0.2;
int image = 0;
unsigned char* rgb;


void axis_vectors()
{
	if (image == 0) {
		texture.load("header.bmp");
		texture.mirror('x');
	}

	if (image == 1) {
		texture.load("logo.bmp");
		texture.mirror('x');
	}

	if (image == 2) {
		texture.load("art.bmp");
		texture.mirror('x');
	}

	float Axisx, Axisy, Axisz, theta = 0, phi = 0;
	for (float i = 0; i < n; i++)
	{
		for (float j = 0; j < n; j++)
		{
			Axisx = (R + r * cos(theta)) * cos(phi);
			Axisy = (R + r * cos(theta)) * sin(phi);
			Axisz = r * sin(theta);
			points.push_back(glm::vec4(Axisx, Axisy, Axisz, 1.0));
			tex_1.push_back(glm::vec2(i * (1 / n), (j) * (1 / n)));

			Axisx = (R + r * cos(theta)) * cos(phi + (2 * M_PI / n));
			Axisy = (R + r * cos(theta)) * sin(phi + (2 * M_PI / n));
			Axisz = r * sin(theta);
			points.push_back(glm::vec4(Axisx, Axisy, Axisz, 1.0));
			tex_1.push_back(glm::vec2((i + 1) * (1 / n), (j) * (1 / n)));

			Axisx = (R + r * cos(theta + (2 * M_PI / n))) * cos(phi);
			Axisy = (R + r * cos(theta + (2 * M_PI / n))) * sin(phi);
			Axisz = r * sin(theta + (2 * M_PI / n));
			points.push_back(glm::vec4(Axisx, Axisy, Axisz, 1.0));
			tex_1.push_back(glm::vec2((i) * (1 / n), (j + 1) * (1 / n)));

			Axisx = (R + r * cos(theta + (2 * M_PI / n))) * cos(phi);
			Axisy = (R + r * cos(theta + (2 * M_PI / n))) * sin(phi);
			Axisz = r * sin(theta + (2 * M_PI / n));
			points.push_back(glm::vec4(Axisx, Axisy, Axisz, 1.0));
			tex_1.push_back(glm::vec2((i) * (1 / n), (j + 1) * (1 / n)));

			Axisx = (R + r * cos(theta)) * cos(phi + (2 * M_PI / n));
			Axisy = (R + r * cos(theta)) * sin(phi + (2 * M_PI / n));
			Axisz = r * sin(theta);
			points.push_back(glm::vec4(Axisx, Axisy, Axisz, 1.0));
			tex_1.push_back(glm::vec2((i + 1) * (1 / n), (j) * (1 / n)));

			Axisx = (R + r * cos(theta + (2 * M_PI / n))) * cos(phi + (2 * M_PI / n));
			Axisy = (R + r * cos(theta + (2 * M_PI / n))) * sin(phi + (2 * M_PI / n));
			Axisz = r * sin(theta + (2 * M_PI / n));
			points.push_back(glm::vec4(Axisx, Axisy, Axisz, 1.0));
			tex_1.push_back(glm::vec2((i + 1) * (1 / n), (j + 1) * (1 / n)));

			Axisx = cos(theta) * cos(phi);
			Axisy = cos(theta) * sin(phi);
			Axisz = sin(theta);
			Origin.push_back(glm::vec4(Axisx, Axisy, Axisz, 1.0));

			Axisx = cos(theta) * cos(phi + (2 * M_PI / n));
			Axisy = cos(theta) * sin(phi + (2 * M_PI / n));
			Axisz = sin(theta);
			Origin.push_back(glm::vec4(Axisx, Axisy, Axisz, 1.0));

			Axisx = cos(theta + (2 * M_PI / n)) * cos(phi);
			Axisy = cos(theta + (2 * M_PI / n)) * sin(phi);
			Axisz = sin(theta + (2 * M_PI / n));
			Origin.push_back(glm::vec4(Axisx, Axisy, Axisz, 1.0));

			Axisx = cos(theta + (2 * M_PI / n)) * cos(phi);
			Axisy = cos(theta + (2 * M_PI / n)) * sin(phi);
			Axisz = sin(theta + (2 * M_PI / n));
			Origin.push_back(glm::vec4(Axisx, Axisy, Axisz, 1.0));

			Axisx = cos(theta) * cos(phi + (2 * M_PI / n));
			Axisy = cos(theta) * sin(phi + (2 * M_PI / n));
			Axisz = sin(theta);
			Origin.push_back(glm::vec4(Axisx, Axisy, Axisz, 1.0));

			Axisx = cos(theta + (2 * M_PI / n)) * cos(phi + (2 * M_PI / n));
			Axisy = cos(theta + (2 * M_PI / n)) * sin(phi + (2 * M_PI / n));
			Axisz = sin(theta + (2 * M_PI / n));
			Origin.push_back(glm::vec4(Axisx, Axisy, Axisz, 1.0));

			white.push_back(glm::vec4(1.0, 1.0, 1.0, 1.0));
			white.push_back(glm::vec4(1.0, 1.0, 1.0, 1.0));
			white.push_back(glm::vec4(1.0, 1.0, 1.0, 1.0));
			white.push_back(glm::vec4(1.0, 1.0, 1.0, 1.0));
			white.push_back(glm::vec4(1.0, 1.0, 1.0, 1.0));
			white.push_back(glm::vec4(1.0, 1.0, 1.0, 1.0));

			theta = theta + (2 * M_PI / n);
		}
		theta = 0.0;
		phi = phi + (2 * M_PI / n);
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
	// PROJECTION MATRIX
	PerspProjectionMatrix = glm::perspective<float>(glm::radians(60.0f), (float)WindowWidth / (float)WindowHeight, 0.01f, 1000.0f);

	// VIEW MATRIX
	glm::vec3 eye(0.0, 0.0, 2.0);
	glm::vec3 center(0.0, 0.0, 0.0);
	glm::vec3 up(0.0, 1.0, 0.0);

	PerspViewMatrix = glm::lookAt(eye, center, up);

	// MODEL MATRIX
	PerspModelMatrix = glm::mat4(1.0);
	PerspModelMatrix = glm::rotate(PerspModelMatrix, glm::radians(perspRotationX), glm::vec3(1.0, 0.0, 0.0));
	PerspModelMatrix = glm::rotate(PerspModelMatrix, glm::radians(perspRotationY), glm::vec3(0.0, 1.0, 0.0));
	PerspModelMatrix = glm::scale(PerspModelMatrix, glm::vec3(perspZoom));
}

void CreateShaders(void)
{
	// Renders Texture 1
	TexpersplightShader.Create("./shaders/texpersplight.vert", "./shaders/texpersplight.frag");
}

/*=================================================================================================
	BUFFERS
=================================================================================================*/

void CreateAxisBuffers(void)
{
	int size = texture.width() * texture.height();
	unsigned char* rgb = new unsigned char[3 * size];
	for (int i = 0; i < size; i++) {
		rgb[3 * i + 0] = texture.data()[0 * size + i];
		rgb[3 * i + 1] = texture.data()[1 * size + i];
		rgb[3 * i + 2] = texture.data()[2 * size + i];
	}

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

	glGenVertexArrays(1, &axis_VAO1);
	glBindVertexArray(axis_VAO1);

	glGenBuffers(4, &axis_VBO1[0]);

	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO1[0]);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec4), &points[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO1[1]);
	glBufferData(GL_ARRAY_BUFFER, white.size() * sizeof(glm::vec4), &white[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO1[2]);
	glBufferData(GL_ARRAY_BUFFER, Origin.size() * sizeof(glm::vec4), &Origin[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO1[3]);
	glBufferData(GL_ARRAY_BUFFER, tex_1.size() * sizeof(glm::vec2), &tex_1[0], GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}

/*=================================================================================================
	CALLBACKS
=================================================================================================*/

//-----------------------------------------------------------------------------
// CALLBACK DOCUMENTATION
// https://www.opengl.org/resources/libraries/glut/spec3/node45.html
// http://freeglut.sourceforge.net/docs/api.php#WindowCallback
//-----------------------------------------------------------------------------

void idle_func()
{
	//uncomment below to repeatedly draw new frames
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
	case 'f':
	{
		draw_wireframe = !draw_wireframe;
		if (draw_wireframe == true)
			std::cout << "Wireframes on.\n";
		else
			std::cout << "Wireframes off.\n";
		break;
	}

	case 'q':
	{
		points.clear();
		white.clear();
		Origin.clear();
		tex_1.clear();
		n++;
		axis_vectors();
		CreateAxisBuffers();
		break;
	}

	case 'a':
	{
		points.clear();
		white.clear();
		tex_1.clear();
		Origin.clear();
		n--;
		axis_vectors();
		CreateAxisBuffers();
		break;
	}

	case 'w':
	{
		points.clear();
		white.clear();
		tex_1.clear();
		Origin.clear();
		r = r + 0.05;
		axis_vectors();
		CreateAxisBuffers();
		break;
	}

	case 's':
	{
		points.clear();
		white.clear();
		Origin.clear();
		tex_1.clear();
		r = r - 0.05;
		axis_vectors();
		CreateAxisBuffers();
		break;
	}

	case 'e':
	{
		points.clear();
		white.clear();
		tex_1.clear();
		Origin.clear();
		R = R + 0.05;
		axis_vectors();
		CreateAxisBuffers();
		break;
	}

	case 'd':
	{
		points.clear();
		white.clear();
		tex_1.clear();
		Origin.clear();
		R = R - 0.05;
		axis_vectors();
		CreateAxisBuffers();
		break;
	}
	case ' ':
	{
		points.clear();
		white.clear();
		Origin.clear();
		tex_1.clear();
		image++;
		if ((image % 3 == 0) && (image != 0)) {
			image = 0;
		}
		axis_vectors();
		CreateAxisBuffers();
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
}

void key_special_pressed(int key, int x, int y)
{
	key_special_states[key] = true;
}

void key_special_released(int key, int x, int y)
{
	key_special_states[key] = false;
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
}

void passive_motion_func(int x, int y)
{
	if (x < 0 || x > WindowWidth || y < 0 || y > WindowHeight)
		return;

	float px, py;
	window_to_scene(x, y, px, py);

	LastMousePosX = x;
	LastMousePosY = y;
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

	CreateTransformationMatrices();

	TexpersplightShader.Use();
	TexpersplightShader.SetUniform("projectionMatrix", glm::value_ptr(PerspProjectionMatrix), 4, GL_FALSE, 1);
	TexpersplightShader.SetUniform("viewMatrix", glm::value_ptr(PerspViewMatrix), 4, GL_FALSE, 1);
	TexpersplightShader.SetUniform("modelMatrix", glm::value_ptr(PerspModelMatrix), 4, GL_FALSE, 1);
	TexpersplightShader.SetUniform("texId", textureId);


	if (draw_wireframe == true)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	glBindVertexArray(axis_VAO1);
	glDrawArrays(GL_TRIANGLES, 0, points.size());


	glBindVertexArray(0);

	if (draw_wireframe == true)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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

	// Create buffers
	CreateAxisBuffers();

	std::cout << "Finished initializing...\n\n";
}


/*=================================================================================================
	MAIN
=================================================================================================*/

int main(int argc, char** argv)
{
	axis_vectors();

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
