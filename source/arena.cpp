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

void Arena::render(float windowWidth, float windowHeight)
{
	const float *arenaColor = this->getColor();
	const float arenaPosX = (this->getPosX() / windowWidth),
							arenaPosY = (this->getPosY() / windowHeight),
							arenaWidth = (this->getWidth() / windowWidth),
							arenaHeight = (this->getHeight() / windowHeight);
	glColor3f(arenaColor[0], arenaColor[1], arenaColor[2]);
	glBegin(GL_POLYGON);
	glVertex2f(arenaPosX, arenaPosY);
	glVertex2f(arenaPosX, arenaPosY + arenaHeight);
	glVertex2f(arenaPosX + arenaWidth, arenaPosY + arenaHeight);
	glVertex2f(arenaPosX + arenaWidth, arenaPosY);
	glEnd();
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
