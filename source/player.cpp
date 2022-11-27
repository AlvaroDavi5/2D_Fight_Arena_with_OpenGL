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

float _degreeToRadian(float deg)
{
	const float rad = (deg * M_PI) / 180.0;

	return rad;
}
float _radianToDegree(float rad)
{
	const float deg = (rad * 180.0) / M_PI;

	return deg;
}

void _translatePoint(float dx, float dy, float &xOut, float &yOut)
{
	xOut += dx;
	yOut += dy;
}
void _rotatePoint(float x, float y, float angle, float &xOut, float &yOut)
{
	float s = sin(angle), c = cos(angle);

	xOut = (x * c) - (y * s);
	yOut = (x * s) + (y * c);
}

void _drawRectangle(float p1[], float p2[], float p3[], float p4[], float color[])
{
	glColor3f(color[0], color[1], color[2]);
	glBegin(GL_POLYGON);
	glVertex2f(p1[0], p1[1]); // L.B.
	glVertex2f(p2[0], p2[1]); // L.T.
	glVertex2f(p3[0], p3[1]); // R.T.
	glVertex2f(p4[0], p4[1]); // R.B.
	glEnd();
}
void _drawCircle(float cp[], float r, GLenum mode, float color[])
{
	const float lineAmount = 36.0; // 360/10=18
	const float twicePi = 2.0f * M_PI;

	glColor3f(color[0], color[1], color[2]);
	glBegin(mode); // GL_LINE_LOOP | GL_TRIANGLE_FAN
	for (int i = 1; i <= lineAmount; i++)
	{
		glVertex2f(
				cp[0] + (abs(r) * cos(i * twicePi / lineAmount)),
				cp[1] + (abs(r) * sin(i * twicePi / lineAmount)));
	}
	glEnd();
}

void _renderHead(float pos[], float r, float t, float color[])
{
	Utils util = Utils();
	float lineColor[3] = {0.0f, 0.0f, 0.0f};
	util.setColorArrayByColorName(BLACK_COLOR, lineColor);

	// head
	_drawCircle(
			pos, r,
			GL_LINE_LOOP, lineColor);
	_drawCircle(
			pos, r,
			GL_TRIANGLE_FAN, color);

	// nose
	float faceDir[2] = {r, 0.0};
	float noseRadius = (r / 4.0);

	_rotatePoint(
			faceDir[0], faceDir[1],
			t, faceDir[0], faceDir[1]);
	_translatePoint(
			pos[0], pos[1],
			faceDir[0], faceDir[1]);

	_drawCircle(
			faceDir, noseRadius,
			GL_LINE_LOOP, lineColor);
	_drawCircle(
			faceDir, noseRadius,
			GL_TRIANGLE_FAN, color);
}

void _renderArm(float pos[], float r, float t, float w, float h, float color[])
{
	float p1[2] = {0.0, 0.0};
	float p2[2] = {0.0, h};
	float p3[2] = {w, h};
	float p4[2] = {w, 0.0};

	_drawRectangle(
			p1, p2, p3, p4,
			color);
}

void Player::render()
{
	Utils util = Utils();
	float skinColor[3] = {0.0f, 0.0f, 0.0f};
	util.setColorArrayByColorName(BEIGE_COLOR, skinColor);
	float *playerColor = this->getColor();
	float playerPos[2] = {this->getPosX(), this->getPosY()}; // x | y
	float playerRadius = this->getRadius(), playerAngle = this->getAngle();

	// head and nose
	_renderHead(
			playerPos,
			playerRadius, playerAngle,
			playerColor);

	// arms
	float armDim[2] = {(playerRadius / 4.0f), (playerRadius * 1.5f)}; // width | height
	/*
	_renderArm(
			playerPos, playerRadius,
			_degreeToRadian(60.0) + playerAngle,
			armDim[0], armDim[1], skinColor);
	*/

	if (SHOW_PLAYER_CIRCLE)
	{
		// TODO - enable player circle
	}
}

void Player::move(float inc)
{
	float playerPos[2] = {this->getPosX(), this->getPosY()};
	float playerRadius = this->getRadius(), playerAngle = this->getAngle();

	_translatePoint(
			inc * cos(playerAngle),
			inc * sin(playerAngle),
			playerPos[0], playerPos[1]);

	this->setPosX(playerPos[0]);
	this->setPosY(playerPos[1]);
}
void Player::rotate(float inc)
{
	float newAngle = _degreeToRadian(_radianToDegree(this->getAngle()) + inc);

	float dirVect[2] = {this->getRadius(), 0.0};
	_rotatePoint(
			dirVect[0], dirVect[1],
			newAngle, dirVect[0], dirVect[1]);

	float dirVectNorm = sqrt(pow(dirVect[0], 2) + pow(dirVect[1], 2));
	float unitVect[2] = {
			dirVect[0] / dirVectNorm,
			dirVect[1] / dirVectNorm};
	float dirAngle = atan2(unitVect[1], unitVect[0]);
	if (isnan(dirAngle))
	{
		dirAngle = _degreeToRadian(90.0);
	}

	this->setAngle(dirAngle);
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

const float Player::getAngle()
{
	return this->angle;
}
void Player::setAngle(const float t)
{
	const float maxDegree = 360.0f;
	float newAngle = _radianToDegree(t);

	if (newAngle > 0.0f)
	{
		while (newAngle > maxDegree)
		{
			newAngle -= maxDegree;
		}
	}
	else if (newAngle < 0.0f)
	{
		while (newAngle < -maxDegree)
		{
			newAngle += maxDegree;
		}
	}

	if (newAngle < 0.0f)
	{
		newAngle = maxDegree + newAngle;
	}

	this->angle = _degreeToRadian(newAngle);
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
