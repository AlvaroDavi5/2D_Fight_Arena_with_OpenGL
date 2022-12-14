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
#define ARM_ROTATE_ANGLE 60.0

float viewingWidth = float(WINDOW_DEFAULT_SIZE) - 200.0;
float viewingHeight = float(WINDOW_DEFAULT_SIZE) - 200.0;
float moveIncKey = 2.0, rotateIncKey = 1.0;
bool enableCircles = false, moveOpponent = true;
int keyStatus[256];
float mousePosXVect[2] = {0.0, 0.0};
bool mouseFree = false;

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
	case '\'':
		player.enableCircles = !player.enableCircles;
		opponent.enableCircles = !opponent.enableCircles;
		break;
	case ' ':
		moveOpponent = !moveOpponent;
		break;
	case '+':
		if (moveIncKey < 10.0)
			moveIncKey += 0.5;
		break;
	case '-':
		if (moveIncKey > 0.0)
			moveIncKey -= 0.5;
		break;
	case 'w':
	case 'W':
		keyStatus[(int)('w')] = 1; // using keyStatus trick
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

void mouseControl(int button, int state, int x, int y)
{
	mouseFree = bool(state);
	float posX = float(x) / viewingWidth;

	if (!mouseFree)
	{
		mousePosXVect[0] = posX;
	}
}
void mouseMove(int x, int y)
{
	const float playerAngle = player.getAngle();
	float posX = float(x) / viewingWidth;

	mousePosXVect[1] = posX;
	float interval = mousePosXVect[1] - mousePosXVect[0];
	bool viewingWidthHasExceded = abs(interval * viewingWidth) >= abs(viewingWidth);

	if (interval > 0.0 && !viewingWidthHasExceded)
	{
		player.setRightArmAngle(playerAngle, -interval * ARM_ROTATE_ANGLE);
	}
	else if (interval < 0.0 && !viewingWidthHasExceded)
	{
		player.setLeftArmAngle(playerAngle, interval * ARM_ROTATE_ANGLE);
	}

	glutPostRedisplay();
}

void display(void)
{
	// clear display buffer for all pixels
	glClear(GL_COLOR_BUFFER_BIT);

	arena.render();
	player.render();
	opponent.render();

	// draw on frame buffer
	glutSwapBuffers();
}

void idle(void)
{
	const float playerAngle = player.getAngle();
	bool moved = false;

	// player movement
	if (keyStatus[(int)('w')])
	{
		player.move(+moveIncKey);
		moved = true;
	}
	if (keyStatus[(int)('s')])
	{
		player.move(-moveIncKey);
		moved = true;
	}
	// player rotation
	if (keyStatus[(int)('a')])
	{
		player.rotate(+rotateIncKey);
		moved = true;
	}
	if (keyStatus[(int)('d')])
	{
		player.rotate(-rotateIncKey);
		moved = true;
	}

	if (mouseFree || moved)
	{
		player.setLeftArmAngle(playerAngle, 0.0);
		player.setRightArmAngle(playerAngle, 0.0);
	}

	if (moveOpponent)
	{
		opponent.goTo(
				player.getPosX(),
				player.getPosY(),
				(moveIncKey / 2.0));
	}

	// TODO - treat punch

	glutPostRedisplay();
}

void registerCallbacks()
{
	glutDisplayFunc(display);
	glutReshapeFunc(NULL);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyUp);
	glutMouseFunc(mouseControl);
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
	glutInitWindowPosition(100.0, 100.0);						 // declares window position
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
		util.setColorArrayByColorName(arenaBackground->Attribute("fill"), arena.getColor());

		// load player attributes
		player.setPosX(atof(playerOnePosition->Attribute("cx")));
		player.setPosY(atof(playerOnePosition->Attribute("cy")));
		player.setRadius(atof(playerOnePosition->Attribute("r")));
		util.setColorArrayByColorName(playerOnePosition->Attribute("fill"), player.getColor());

		// load opponent attributes
		opponent.setPosX(atof(playerTwoPosition->Attribute("cx")));
		opponent.setPosY(atof(playerTwoPosition->Attribute("cy")));
		opponent.setRadius(atof(playerTwoPosition->Attribute("r")));
		util.setColorArrayByColorName(playerTwoPosition->Attribute("fill"), opponent.getColor());

		delete arenaFile;

		// adjust positions
		viewingWidth = arena.getWidth();
		viewingHeight = arena.getHeight();

		player.setPosX(player.getPosX() - arena.getPosX());
		player.setPosY(player.getPosY() - arena.getPosY());
		player.setLimits(arena.getWidth(), arena.getHeight());
		player.setOpponent(&opponent);

		opponent.setPosX(opponent.getPosX() - arena.getPosX());
		opponent.setPosY(opponent.getPosY() - arena.getPosY());
		opponent.setLimits(arena.getWidth(), arena.getHeight());
		opponent.setOpponent(&player);
		opponent.rotate(180.0);

		arena.setPosX(0.0);
		arena.setPosY(0.0);

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
