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
#include "../include/tinyxml2.h"
#include "../include/utils.h"
#include "../include/arena.h"
// #include "../include/player.h"

using namespace std;
using namespace tinyxml2;

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 600
#define INC_KEY 1
#define INC_KEYIDLE 2.0

const float viewingWidth = float(WINDOW_WIDTH) - 200.0;
const float viewingHeight = float(WINDOW_HEIGHT) - 200.0;
int keyStatus[256];
int animate = 0;
Arena arena = Arena();

void resetKeyStatus()
{
	int i;

	for (i = 0; i < 256; i++)
		keyStatus[i] = 0;
}
void keyUp(unsigned char key, int x, int y)
{
	keyStatus[(int)(key)] = 0;

	// reload display
	glutPostRedisplay();
}
void keyPress(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ' ':
		animate = !animate;
		break;
	case 'w':
	case 'W':
		keyStatus[(int)('w')] = 1;
		break;
	case 's':
	case 'S':
		keyStatus[(int)('s')] = 1;
		break;
	case 'a':
	case 'A':
		keyStatus[(int)('a')] = 1;
		break;
	case 'd':
	case 'D':
		keyStatus[(int)('d')] = 1;
		break;
	case 27:
		exit(0);
	}

	glutPostRedisplay();
}

void mouseMove(int x, int y)
{
	float gX = 0.0, gY = 0.0, s = 1.0;
	float posX = 0.0, posY = 0.0;

	y = viewingWidth - y;
	posX = float(x) / float(viewingWidth);
	posY = float(y) / float(viewingWidth);

	gX = (s * posX) + s * (-0.5);
	gY = (s * posY) + s * (-0.5);

	glutPostRedisplay();
}

void display(void)
{
	// clear display buffer for all pixels
	glClear(GL_COLOR_BUFFER_BIT);

	arena.render(viewingWidth, viewingHeight);

	// drawn on frame buffer
	glutSwapBuffers();
}

void idle(void)
{
	float inc = INC_KEYIDLE;

	if (keyStatus[(int)('w')])
	{
		arena.setPosY(arena.getPosY() + inc);
	}
	if (keyStatus[(int)('s')])
	{
		arena.setPosY(arena.getPosY() - inc);
	}
	if (keyStatus[(int)('a')])
	{
		arena.setPosX(arena.getPosX() - inc);
	}
	if (keyStatus[(int)('d')])
	{
		arena.setPosX(arena.getPosX() + inc);
	}

	// treat collisons

	// control animation

	glutPostRedisplay();
}

void registerCallbacks()
{
	glutDisplayFunc(display);
	glutReshapeFunc(NULL);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyUp);
	glutMotionFunc(mouseMove);
	glutIdleFunc(idle);
}

void init(int argc, char *argv[])
{
	// GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// view window initialization
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // declares window size
	glutInitWindowPosition(100, 100);								 // declares window position
	glutCreateWindow("Fight");											 // create window with name

	registerCallbacks();
	resetKeyStatus();

	// init view system
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // set background color as black, no opacity(alpha)
	glMatrixMode(GL_PROJECTION);					// select the projection matrix
	glOrtho(0.0,													// X coordinate of left edge
					0.0,													// X coordinate of right edge
					0.0,													// Y coordinate of bottom edge
					0.0,													// Y coordinate of top edge
					-1.0,													// Z coordinate of the 'near' plane
					1.0);													// Z coordinate of the 'far' plane
	glMatrixMode(GL_MODELVIEW);						// select the projection matrix
	glLoadIdentity();											// load identity matrix
}

int main(int argc, char *argv[])
{
	setlocale(LC_NUMERIC, "pt_BR");
	auto loc = locale("pt_BR.UTF-8");
	locale::global(loc);

	cout.imbue(locale("C"));

	try
	{
		if (argc < 2)
		{
			cout << "Please enter the correct number of arguments (1 argument)." << endl;

			if (argv[1] == NULL)
			{
				cout << "Arena filepath not found." << endl;
			}

			return 1;
		}

		XMLDocument *arenaFile = new XMLDocument();
		arenaFile->LoadFile(argv[1]);
		if (arenaFile->Error())
		{
			cout << "Error to load SVG file." << endl;

			return 1;
		}

		Utils util = Utils();
		XMLElement *arenaSchema = arenaFile->RootElement();
		XMLElement *arenaBackground = arenaSchema->FirstChildElement();
		XMLElement *playerOnePosition = arenaBackground->NextSiblingElement();
		XMLElement *playerTwoPosition = playerOnePosition->NextSiblingElement();

		arena.setPosX(atof(arenaBackground->Attribute("x")));
		arena.setPosY(atof(arenaBackground->Attribute("y")));
		arena.setWidth(atof(arenaBackground->Attribute("width")));
		arena.setHeight(atof(arenaBackground->Attribute("height")));
		arena.setColor(
				util.getColorArrayByColorName(arenaBackground->Attribute("fill")));

		delete arenaFile;

		init(argc, argv);

		// call render loop
		glutMainLoop();
	}
	catch (const exception &e)
	{
		cout << "An unexpected error has ocurred!" << endl;
		cerr << e.what() << endl;
		return 1;
	}

	return 0;
}
