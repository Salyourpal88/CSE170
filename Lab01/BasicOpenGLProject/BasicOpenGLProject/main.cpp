#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <iostream>

//=================================================================================================
// CALLBACKS
//=================================================================================================

//-----------------------------------------------------------------------------
// CALLBACK DOCUMENTATION
// https://www.opengl.org/resources/libraries/glut/spec3/node45.html
// http://freeglut.sourceforge.net/docs/api.php#WindowCallback
//-----------------------------------------------------------------------------

void idle_func()
{
	//uncomment below to repeatedly draw new frames
	//glutPostRedisplay();
}

void reshape_func(int width, int height)
{
	glViewport(0, 0, width, height);
	glutPostRedisplay();
}

GLfloat red = 1.0f, green = 1.0f, blue = 1.0f;
GLfloat x1 = 0.0f, Benson = 0.0f, z1 = 0.0f;
GLfloat Mall = 0.0f, Mart = 0.0f, Walmart = 0.0f, store = 0.0f;

void keyboard_func(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
	{
		glTranslatef(x1, Benson, z1);
		x1 = 0.0f;
		Benson = 0.05f;
		z1 = 0.0f;
		break;
	}

	case 'a':
	{
		glColor3f(red, green, blue);
		red = (float)(rand() % 255) / 255, green = (float)(rand() % 255) / 255, blue = (float)(rand() % 255) / 255;
		break;
	}

	case 's':
	{
		std::cout << "Press A to change color. Left click to rotate. Press W to shift up";
	}

	case 'd':
	{
		break;
	}

	// Exit on escape key press
	case '\x1B':
	{
		exit(EXIT_SUCCESS);
		break;
	}
	}

	glutPostRedisplay();
}

void key_released(unsigned char key, int x, int y)
{
}

void key_special_pressed(int key, int x, int y)
{
}

void key_special_released(int key, int x, int y)
{
}

void mouse_func(int button, int state, int x, int y)
{
	glRotatef(Mall, Mart, Walmart, store);
	switch (button) {
	case(GLUT_LEFT_BUTTON):
	{
		if (state == GLUT_DOWN) {
			Mall = 130.0f;
			Mart = 0.0f;
			Walmart = 0.0f;
			store = 1.0f;
			break;
		}
	}
	}
}

void passive_motion_func(int x, int y)
{
}

void active_motion_func(int x, int y)
{
}

//=================================================================================================
// RENDERING
//=================================================================================================

void display_func(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(-0.5f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(0.5f, 0.0f);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex2f(0.0f, -0.5f);
	glVertex2f(0.0f, 0.5f);
	glEnd();

	//glColor3f( 1.0f, 1.0f, 1.0f );
	//glBegin( GL_TRIANGLES );
	//	glVertex2f( 0.1f, 0.1f );
	//	glVertex2f( 0.4f, 0.1f );
	//	glVertex2f( 0.25f, 0.4f );
	//glEnd();

	//Triangle 1 of "S"
	//glRotatef(Mall, Mart, Walmart, store);
	//glTranslatef(x1, Benson, z1);
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(-1.0f, 0.00f);
	glVertex2f(-1.0f, 0.05f);
	glVertex2f(-0.7f, 0.00f);
	glEnd();

	//Triangle 2
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.7f, 0.05f);
	glVertex2f(-1.0f, 0.05f);
	glVertex2f(-0.7f, 0.00f);
	glEnd();

	//Triangle 3
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.6f, 0.05f);
	glVertex2f(-0.7f, 0.05f);
	glVertex2f(-0.7f, 0.15f);
	glEnd();

	//Triangle 4
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.6f, 0.05f);
	glVertex2f(-0.6f, 0.15f);
	glVertex2f(-0.7f, 0.15f);
	glEnd();

	//Triangle 5
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.9f, 0.2f);
	glVertex2f(-0.9f, 0.15f);
	glVertex2f(-0.7f, 0.15f);
	glEnd();

	//Triangle 6
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.7f, 0.15f);
	glVertex2f(-0.7f, 0.2f);
	glVertex2f(-0.9f, 0.2f);
	glEnd();

	//Triangle 7
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.9f, 0.25f);
	glVertex2f(-0.9f, 0.2f);
	glVertex2f(-1.0f, 0.2f);
	glEnd();

	//Triangle 8
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(-1.0f, 0.2f);
	glVertex2f(-1.0f, 0.25f);
	glVertex2f(-0.9f, 0.25f);
	glEnd();

	//Triangle 9
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.9f, 0.25f);
	glVertex2f(-0.9f, 0.3f);
	glVertex2f(-0.6f, 0.3f);
	glEnd();

	//Triangle 10
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.6f, 0.3f);
	glVertex2f(-0.6f, 0.25f);
	glVertex2f(-0.9f, 0.25f);
	glEnd();

	//Triangle 1 of "A"
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.4f, 0.00f);
	glVertex2f(-0.5f, 0.00f);
	glVertex2f(-0.5f, 0.25f);
	glEnd();

	//Triangle 2
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.5f, 0.25f);
	glVertex2f(-0.4f, 0.25f);
	glVertex2f(-0.4f, 0.00f);
	glEnd();

	//Triangle 3
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.4f, 0.3f);
	glVertex2f(-0.4f, 0.25f);
	glVertex2f(-0.1f, 0.25f);
	glEnd();

	//Triangle 4
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.1f, 0.25f);
	glVertex2f(-0.1f, 0.3f);
	glVertex2f(-0.4f, 0.3f);
	glEnd();

	//Triangle 5
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.4f, 0.15f);
	glVertex2f(-0.4f, 0.2f);
	glVertex2f(-0.1f, 0.2f);
	glEnd();

	//Triangle 6
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.1f, 0.2f);
	glVertex2f(-0.1f, 0.15f);
	glVertex2f(-0.4f, 0.15f);
	glEnd();

	//Triangle 7
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.1f, 0.25f);
	glVertex2f(0.0f, 0.25f);
	glVertex2f(0.0f, 0.0f);
	glEnd();

	//Triangle 8
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(-0.1f, 0.0f);
	glVertex2f(-0.1f, 0.25f);
	glEnd();

	//Triangle 1 of "L"
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.2f, 0.3f);
	glVertex2f(0.1f, 0.3f);
	glVertex2f(0.1f, 0.0f);
	glEnd();

	//Triangle 2
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.1f, 0.0f);
	glVertex2f(0.2f, 0.0f);
	glVertex2f(0.2f, 0.3f);
	glEnd();

	//Triangle 3
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.2f, 0.05f);
	glVertex2f(0.2f, 0.0f);
	glVertex2f(0.4f, 0.0f);
	glEnd();

	//Triangle 4
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.4f, 0.0f);
	glVertex2f(0.4f, 0.05f);
	glVertex2f(0.2f, 0.05f);
	glEnd();

	//Triangle 1 of "V"
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.5f, 0.3f);
	glVertex2f(0.6f, 0.3f);
	glVertex2f(0.6f, 0.15f);
	glEnd();

	//Triangle 2
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.6f, 0.15f);
	glVertex2f(0.5f, 0.15f);
	glVertex2f(0.5f, 0.3f);
	glEnd();

	//Triangle 3
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.7f, 0.15f);
	glVertex2f(0.6f, 0.15f);
	glVertex2f(0.6f, 0.05f);
	glEnd();

	//Triangle 4
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.6f, 0.05f);
	glVertex2f(0.7f, 0.05f);
	glVertex2f(0.7f, 0.15f);
	glEnd();

	//Triangle 5
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.7f, 0.05f);
	glVertex2f(0.7f, 0.0f);
	glVertex2f(0.8f, 0.0f);
	glEnd();

	//Triangle 6
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.7f, 0.05f);
	glVertex2f(0.8f, 0.05f);
	glVertex2f(0.8f, 0.0f);
	glEnd();

	//Triangle 7
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.8f, 0.15f);
	glVertex2f(0.8f, 0.05f);
	glVertex2f(0.9f, 0.05f);
	glEnd();

	//Triangle 8
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.8f, 0.15f);
	glVertex2f(0.9f, 0.15f);
	glVertex2f(0.9f, 0.05f);
	glEnd();

	//Triangle 9
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.9f, 0.15f);
	glVertex2f(1.0f, 0.15f);
	glVertex2f(0.9f, 0.3f);
	glEnd();

	//Triangle 10
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.9f, 0.3f);
	glVertex2f(1.0f, 0.3f);
	glVertex2f(1.0f, 0.15f);
	glEnd();

	glutSwapBuffers();
}

//=================================================================================================
// INIT
//=================================================================================================

void init(void)
{
	// Print some info
	std::cout << "Vendor:         " << glGetString(GL_VENDOR) << "\n";
	std::cout << "Renderer:       " << glGetString(GL_RENDERER) << "\n";
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n\n";

	// Set the background color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	std::cout << "Finished initializing...\n\n";
	std::cout << "Press S for more instructions:";
}

//=================================================================================================
// MAIN
//=================================================================================================

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutCreateWindow("CSE-170 Computer Graphics");

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
