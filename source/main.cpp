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
#include "../include/player.h"

using namespace std;
using namespace tinyxml2;

#define WINDOW_DEFAULT_SIZE 600
#define INC_KEY 1
#define INC_KEYIDLE 2.0

float viewingWidth = float(WINDOW_DEFAULT_SIZE) - 200.0;
float viewingHeight = float(WINDOW_DEFAULT_SIZE) - 200.0;
int keyStatus[256];
int animate = 0;
Arena arena = Arena();
Player opponent = Player();
Player player = Player();

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

	arena.render();
	player.render();
	opponent.render();

	// drawn on frame buffer
	glutSwapBuffers();
}

void idle(void)
{
	float inc = INC_KEYIDLE;

	// player movement
	if (keyStatus[(int)('w')])
	{
		player.setPosY(player.getPosY() + inc);
	}
	if (keyStatus[(int)('s')])
	{
		player.setPosY(player.getPosY() - inc);
	}
	if (keyStatus[(int)('a')])
	{
		player.setAngle(player.getAngle() - inc);
	}
	if (keyStatus[(int)('d')])
	{
		player.setAngle(player.getAngle() + inc);
	}

	// TODO - treat collisons

	// TODO - control animation

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
	glutInitWindowSize(viewingWidth, viewingHeight); // declares window size
	glutInitWindowPosition(100, 100);								 // declares window position
	glutCreateWindow("Fight");											 // create window with title

	registerCallbacks();
	resetKeyStatus();

	// init view system
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // set background color as black, no opacity(alpha)
	glMatrixMode(GL_PROJECTION);					// select the projection matrix
	glOrtho(0.0,													// X coordinate of left edge
					viewingWidth,									// X coordinate of right edge
					0.0,													// Y coordinate of bottom edge
					viewingHeight,								// Y coordinate of top edge
					1.0,													// Z coordinate of the 'near' plane
					-1.0);												// Z coordinate of the 'far' plane
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

		// load arena attributes
		arena.setPosX(atof(arenaBackground->Attribute("x")));
		arena.setPosY(atof(arenaBackground->Attribute("y")));
		arena.setWidth(atof(arenaBackground->Attribute("width")));
		arena.setHeight(atof(arenaBackground->Attribute("height")));
		arena.setColor(
				util.getColorArrayByColorName(arenaBackground->Attribute("fill")));

		// load player attributes
		player.setPosX(atof(playerOnePosition->Attribute("cx")));
		player.setPosY(atof(playerOnePosition->Attribute("cy")));
		player.setRadius(atof(playerOnePosition->Attribute("r")));
		player.setColor(
				util.getColorArrayByColorName(playerOnePosition->Attribute("fill")));

		// load opponent attributes
		opponent.setPosX(atof(playerTwoPosition->Attribute("cx")));
		opponent.setPosY(atof(playerTwoPosition->Attribute("cy")));
		opponent.setRadius(atof(playerTwoPosition->Attribute("r")));
		opponent.setColor(
				util.getColorArrayByColorName(playerTwoPosition->Attribute("fill")));

		delete arenaFile;

		viewingWidth = arena.getWidth();
		viewingHeight = arena.getHeight();

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
