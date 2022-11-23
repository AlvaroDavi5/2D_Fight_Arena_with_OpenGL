#ifndef PLAYER_H

#define PLAYER_H

#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

using namespace std;

class Player
{
private:
	float posX = 0.0;
	float posY = 0.0;
	float bodyRadius = 0.0;
	float color[3] = {0.0f, 0.0f, 0.0f};
	bool wasCollision = false;

public:
	// ! Constructor(s) and Destructor
	Player();
	Player(float x, float y);
	Player(float x, float y, float r);
	~Player();

	// ? Member Functions
	void render();

	float *getColor();
	void setColor(float colorArray[]);

	const float getPosX();
	void setPosX(const float x);
	const float getPosY();
	void setPosY(const float y);

	const float getRadius();
	void setRadius(const float r);

	const bool getCollision();
	void setCollision(const bool collide);
};

#endif // PLAYER_H
