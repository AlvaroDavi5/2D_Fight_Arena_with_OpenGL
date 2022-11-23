#include "../include/player.h"

Player::Player()
{
	//// do nothing
}
Player::Player(float x, float y)
{
	this->posX = x;
	this->posY = y;
}
Player::Player(float x, float y, float r)
{
	this->posX = x;
	this->posY = y;
	this->bodyRadius = r;
}
Player::~Player()
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

void _drawnCircle(float cx, float cy, float r, GLenum mode, float R, float G, float B)
{
	const float lineAmount = 36.0; // 360/10=18
	const float twicePi = 2.0f * M_PI;

	glColor3f(R, G, B);
	glBegin(mode); // GL_LINE_LOOP | GL_TRIANGLE_FAN
	for (int i = 1; i <= lineAmount; i++)
	{
		glVertex2f(
				cx + (r * cos(i * twicePi / lineAmount)),
				cy + (r * sin(i * twicePi / lineAmount)));
	}
	glEnd();
}

void Player::render()
{
	const float *playerColor = this->getColor();

	_drawnCircle(
			this->getPosX(), this->getPosY(),
			this->getRadius(), GL_TRIANGLE_FAN,
			playerColor[0], playerColor[1], playerColor[2]);
}

float *Player::getColor()
{
	return this->color;
}
void Player::setColor(float colorArray[])
{
	this->color[0] = colorArray[0];
	this->color[1] = colorArray[1];
	this->color[2] = colorArray[2];
}

const float Player::getPosX()
{
	return this->posX;
}
void Player::setPosX(const float x)
{
	this->posX = x;
}
const float Player::getPosY()
{
	return this->posY;
}
void Player::setPosY(const float y)
{
	this->posY = y;
}

const float Player::getRadius()
{
	return this->bodyRadius;
}
void Player::setRadius(const float r)
{
	this->bodyRadius = r;
}

const bool Player::getCollision()
{
	return this->wasCollision;
}
void Player::setCollision(const bool collision)
{
	this->wasCollision = collision;
}
