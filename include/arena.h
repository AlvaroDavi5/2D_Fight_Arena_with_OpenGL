#ifndef ARENA_H

#define ARENA_H

#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

using namespace std;

class Arena
{
private:
	float posX = 0.0;
	float posY = 0.0;
	float width = 0.0;
	float height = 0.0;
	float color[3] = {0.0f, 0.0f, 0.0f};

public:
	// ! Constructor(s) and Destructor
	Arena();
	Arena(float x, float y);
	Arena(float x, float y, float w, float h);
	~Arena();

	// ? Member Functions
	void render(float windowWidth, float windowHeight);

	float *getColor();
	void setColor(float colorArray[]);

	const float getPosX();
	void setPosX(const float x);
	const float getPosY();
	void setPosY(const float y);

	const float getWidth();
	void setWidth(const float w);
	const float getHeight();
	void setHeight(const float h);
};

#endif // ARENA_H
