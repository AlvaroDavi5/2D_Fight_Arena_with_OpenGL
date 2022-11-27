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
	float angle = 0.0;
	float posX = 0.0, posY = 0.0;
	float bodyRadius = 0.0;
	float color[3] = {0.0, 0.0, 0.0};
	bool wasCollision = false;
	float leftArmAngle[2] = {0.0, 0.0};
	float rightArmAngle[2] = {0.0, 0.0};

public:
	// ! Constructor(s) and Destructor
	Player();
	Player(float x, float y);
	Player(float x, float y, float r);
	~Player();

	// ? Member Functions
	void render();

	void move(float inc);
	void rotate(float inc);

	float *getColor();
	void setColor(float colorArray[]);

	const float getPosX();
	void setPosX(const float x);
	const float getPosY();
	void setPosY(const float y);

	const float getAngle();
	void setAngle(const float t);
	const float getRadius();
	void setRadius(const float r);

	void setLeftArmAngle(float radianAngle, float degreeAngle);
	void setRightArmAngle(float radianAngle, float degreeAngle);

	const bool getCollision();
	void setCollision(const bool collide);
};

#endif // PLAYER_H
