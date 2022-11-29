#ifndef PLAYER_H

#define PLAYER_H

#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "./utils.h"

#define SHOW_COLLISION_CIRCLE true
#define DEFAULT_ARM_INCLINATION 15.0

using namespace std;

class Player
{
private:
	// * Attributes
	float angle = 0.0;
	float posX = 0.0, posY = 0.0;
	float maxPos[2] = {0.0, 0.0};
	float bodyRadius = 0.0;
	float color[3] = {0.0, 0.0, 0.0};
	bool wasCollision = false;
	float leftArmAngle[2] = {0.0, 0.0};
	float rightArmAngle[2] = {0.0, 0.0};
	float leftHandPos[2] = {0.0, 0.0};
	float rightHandPos[2] = {0.0, 0.0};
	Player *opponent = NULL;
	// // Private Methods
	bool collidedWithOpponent(bool invertRadius);

public:
	// ! Constructor(s) and Destructor Methods
	Player();
	Player(float x, float y);
	Player(float x, float y, float r);
	~Player();

	// ? Member Functions (Methods)
	void render();

	void move(float inc);
	void rotate(float inc);

	float *getColor();
	void setColor(float colorArray[]);

	const float getPosX();
	void setPosX(const float x);
	const float getPosY();
	void setPosY(const float y);
	void setLimits(const float x, const float y);

	const float getAngle();
	void setAngle(const float t);
	const float getRadius();
	void setRadius(const float r);

	void setLeftArmAngle(float radianAngle, float degreeAngle);
	void setRightArmAngle(float radianAngle, float degreeAngle);

	const bool getCollision();
	void setCollision(const bool collide);

	Player *getOpponent();
	void setOpponent(Player *opponent);

	void goTo(float x, float y, float incMove, float incAngle);
};

#endif // PLAYER_H
