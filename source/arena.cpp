#include "../include/arena.h"

Arena::Arena()
{
	//// do nothing
}
Arena::Arena(float x, float y)
{
	this->posX = x;
	this->posY = y;
}
Arena::Arena(float x, float y, float w, float h)
{
	this->posX = x;
	this->posY = y;
	this->width = w;
	this->height = h;
}
Arena::~Arena()
{
	//// do nothing
}

void _drawnRectangle(float x, float y, float w, float h, float R, float G, float B)
{
	glColor3f(R, G, B);
	glBegin(GL_POLYGON);
	glVertex2f(x, y);
	glVertex2f(x, y + h);
	glVertex2f(x + w, y + h);
	glVertex2f(x + w, y);
	glEnd();
}

void Arena::render()
{
	const float *arenaColor = this->getColor();

	_drawnRectangle(
			this->getPosX(), this->getPosY(),
			this->getWidth(), this->getHeight(),
			arenaColor[0], arenaColor[1], arenaColor[2]);
}

float *Arena::getColor()
{
	return this->color;
}
void Arena::setColor(float colorArray[])
{
	this->color[0] = colorArray[0];
	this->color[1] = colorArray[1];
	this->color[2] = colorArray[2];
}

const float Arena::getPosX()
{
	return this->posX;
}
void Arena::setPosX(const float x)
{
	this->posX = x;
}
const float Arena::getPosY()
{
	return this->posY;
}
void Arena::setPosY(const float y)
{
	this->posY = y;
}

const float Arena::getWidth()
{
	return this->width;
}
void Arena::setWidth(const float w)
{
	this->width = w;
}
const float Arena::getHeight()
{
	return this->height;
}
void Arena::setHeight(const float h)
{
	this->height = h;
}
