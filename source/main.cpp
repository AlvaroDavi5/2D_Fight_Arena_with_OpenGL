#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <locale>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "../include/utils.h"

using namespace std;

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 600
#define POSITION_ADJUST 0.50

float gX = 0.0, gY = 0.0, s = 1.0;

void initOpenGL(int argc, char *argv[])
{
	// GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// view window initialization
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // declares window size
	glutInitWindowPosition(100, 100);								 // declares window position
	glutCreateWindow("Fight");											 // create window with name
	glClearColor(0.0, 0.0, 0.0, 0.0);								 // set background color

	// init view system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 0.0, 0.0, 0.0, -1.0, 1.0);
}

void display(void)
{
	// clear display buffer for all pixels
	glClear(GL_COLOR_BUFFER_BIT);

	// set vertex colors using RGB scale ranging between 0.0 and 1.0
	glColor3f(0.5, 1.0, 0.7);

	// drawn a green polygon (square)
	glBegin(GL_POLYGON);
	glVertex3f(0.25 + gX, 0.25 + gY, 0.0);
	glVertex3f(0.75 + gX, 0.25 + gY, 0.0);
	glVertex3f(0.75 + gX, 0.75 + gY, 0.0);
	glVertex3f(0.25 + gX, 0.75 + gY, 0.0);
	glEnd();

	// drawn on frame buffer
	glutSwapBuffers();
}

void keyPress(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		gY += (0.01);
		break;
	case 's':
		gY -= (0.01);
		break;
	case 'a':
		gX -= (0.01);
		break;
	case 'd':
		gX += (0.01);
		break;
	default:
		break;
	}

	// reload display
	glutPostRedisplay();
}

void mouseMove(int x, int y)
{
	float posX = 0.0, posY = 0.0;

	y = WINDOW_HEIGHT - y;
	posX = float(x) / float(WINDOW_WIDTH);
	posY = float(y) / float(WINDOW_HEIGHT);

	gX = (s * posX) + s * (-POSITION_ADJUST);
	gY = (s * posY) + s * (-POSITION_ADJUST);

	glutPostRedisplay();
}

void registerCallbacks()
{
	glutDisplayFunc(display);
	glutReshapeFunc(NULL);
	glutKeyboardFunc(keyPress);
	glutMotionFunc(mouseMove);
}

int main(int argc, char *argv[])
{
	setlocale(LC_NUMERIC, "pt_BR");
	auto loc = locale("pt_BR.UTF-8");
	locale::global(loc);

	try
	{
		Utils util = Utils();

		initOpenGL(argc, argv);
		registerCallbacks();

		// render loop
		glutMainLoop();
	}
	catch (const exception *e)
	{
		cout << "An unexpected error has ocurred!" << endl;
		cerr << e->what() << endl;
		return 1;
	}

	cout.imbue(locale("C"));

	return 0;
}
