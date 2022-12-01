#include "../include/player.h"

Player::Player()
{
	this->setPosX(0.0);
	this->setPosY(0.0);
	this->setRadius(0.0);
	this->setAngle(0.0);
}
Player::Player(float x, float y)
{
	this->setPosX(x);
	this->setPosY(y);
	this->setRadius(0.0);
	this->setAngle(0.0);
}
Player::Player(float x, float y, float r)
{
	this->setPosX(x);
	this->setPosY(y);
	this->setRadius(r);
	this->setAngle(0.0);
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

void _transformSegment(float rx, float ry, float dx, float dy, float t, float &xOut, float &yOut)
{
	_rotatePoint(
			rx, ry, t,
			xOut, yOut);

	_translatePoint(
			dx, dy,
			xOut, yOut);
}

void _renderArm(
		float pos[], float radius, float playerAngle,
		float armsAngles[], float armsWidths[], float armsHeights[],
		float color[], float handColor[], float *handPos, bool invert)
{
	float armAngle = armsAngles[0], foreArmAngle = armsAngles[1];
	float armWidth = armsWidths[0], foreArmWidth = armsWidths[1];
	float armHeight = armsHeights[0], foreArmHeight = armsHeights[1];

	if (invert)
	{
		radius = -radius;
		armHeight = -armHeight;
	}

	// arm
	float armStartPos[2] = {0.0, radius};
	_rotatePoint(
			armStartPos[0], armStartPos[1], playerAngle,
			armStartPos[0], armStartPos[1]);

	float armP1[2] = {0.0, 0.0};
	_transformSegment(
			armP1[0], armP1[1],
			pos[0] + armStartPos[0], pos[1] + armStartPos[1], armAngle,
			armP1[0], armP1[1]);
	float armP2[2] = {0.0, armHeight};
	_transformSegment(
			armP2[0], armP2[1],
			pos[0] + armStartPos[0], pos[1] + armStartPos[1], armAngle,
			armP2[0], armP2[1]);
	float armP3[2] = {armWidth, armHeight};
	_transformSegment(
			armP3[0], armP3[1],
			pos[0] + armStartPos[0], pos[1] + armStartPos[1], armAngle,
			armP3[0], armP3[1]);
	float armP4[2] = {armWidth, 0.0};
	_transformSegment(
			armP4[0], armP4[1],
			pos[0] + armStartPos[0], pos[1] + armStartPos[1], armAngle,
			armP4[0], armP4[1]);

	_drawRectangle(
			armP1, armP2, armP3, armP4,
			color);

	// forearm
	const float defaultAngle = _degreeToRadian(-90.0);
	float startP1[2] = {armP3[0], armP3[1]};
	float startP2[2] = {armP2[0], armP2[1]};

	if (invert)
	{
		foreArmWidth = -foreArmWidth;
	}

	float foreArmP1[2] = {0.0, 0.0};
	_transformSegment(
			foreArmP1[0], foreArmP1[1],
			startP1[0], startP1[1], foreArmAngle + defaultAngle,
			foreArmP1[0], foreArmP1[1]);
	float foreArmP2[2] = {0.0, foreArmHeight};
	_transformSegment(
			foreArmP2[0], foreArmP2[1],
			startP2[0], startP2[1], foreArmAngle + defaultAngle,
			foreArmP2[0], foreArmP2[1]);
	float foreArmP3[2] = {foreArmWidth, foreArmHeight};
	_transformSegment(
			foreArmP3[0], foreArmP3[1],
			startP2[0], startP2[1], foreArmAngle + defaultAngle,
			foreArmP3[0], foreArmP3[1]);
	float foreArmP4[2] = {foreArmWidth, 0.0};
	_transformSegment(
			foreArmP4[0], foreArmP4[1],
			startP1[0], startP1[1], foreArmAngle + defaultAngle,
			foreArmP4[0], foreArmP4[1]);

	_drawRectangle(
			foreArmP1, foreArmP2, foreArmP3, foreArmP4,
			color);

	// hand
	float handPosition[2] = {foreArmP3[0], foreArmP3[1]};
	float handRadius = radius / 2.0f;

	handPos[0] = handPosition[0];
	handPos[1] = handPosition[1];
	_drawCircle(
			handPosition, handRadius,
			GL_TRIANGLE_FAN, handColor);
}

void Player::render()
{
	Utils util = Utils();
	float skinColor[3] = {0.0f, 0.0f, 0.0f}, defaultColor[3] = {0.0f, 0.0f, 0.0f};
	util.setColorArrayByColorName(BEIGE_COLOR, skinColor);
	util.setColorArrayByColorName(BLACK_COLOR, defaultColor);

	float *playerColor = this->getColor();
	float playerPos[2] = {this->getPosX(), this->getPosY()}; // x | y
	float playerRadius = this->getRadius(), playerAngle = this->getAngle();

	// head and nose
	_renderHead(
			playerPos,
			playerRadius, playerAngle,
			playerColor);

	// arms and forearms
	const float armWidth = playerRadius / 4.0f;
	const float armHeight = playerRadius * 1.0f;
	const float foreArmWidth = armWidth, foreArmHeight = armHeight * 2.0f;

	float armsWidths[2] = {armWidth, foreArmWidth};
	float armsHeights[2] = {armHeight, foreArmHeight};
	float *leftArmAngle = this->leftArmAngle, *rightArmAngle = this->rightArmAngle;

	_renderArm(
			playerPos, playerRadius,
			playerAngle, leftArmAngle,
			armsWidths, armsHeights,
			skinColor, playerColor, this->leftHandPos, false);
	_renderArm(
			playerPos, playerRadius,
			playerAngle, rightArmAngle,
			armsWidths, armsHeights,
			skinColor, playerColor, this->rightHandPos, true);

	if (this->enableCircles)
	{
		_drawCircle(
				playerPos, playerRadius * COLLISION_CIRCLE_TAX,
				GL_LINE_LOOP, defaultColor);
	}
}

void Player::move(float inc)
{
	float playerPos[2] = {this->getPosX(), this->getPosY()};
	float playerAngle = this->getAngle(), playerRadius = this->getRadius();

	_translatePoint(
			inc * cos(playerAngle),
			inc * sin(playerAngle),
			playerPos[0], playerPos[1]);

	// TODO - fix collision radius
	const float playerCollisionRadius = playerRadius;
	const float playerExtremityLeft = (playerPos[0] - playerCollisionRadius), playerExtremityRight = (playerPos[0] + playerCollisionRadius);
	const float playerExtremityBottom = (playerPos[1] - playerCollisionRadius), playerExtremityTop = (playerPos[1] + playerCollisionRadius);
	const float opponentRadius = this->opponent->getRadius();
	const float opponentExtremityLeft = (this->opponent->getPosX() - opponentRadius), opponentExtremityRight = (this->opponent->getPosX() + opponentRadius);
	const float opponentExtremityBottom = (this->opponent->getPosY() - opponentRadius), opponentExtremityTop = (this->opponent->getPosY() + opponentRadius);

	const bool canIMoveInX = (playerExtremityRight < opponentExtremityLeft) || (playerExtremityLeft > opponentExtremityRight);
	const bool canIMoveInY = (playerExtremityTop < opponentExtremityBottom) || (playerExtremityBottom > opponentExtremityTop);

	if (playerExtremityLeft > 0 &&
			playerExtremityRight < this->maxPos[0] &&
			(canIMoveInX || canIMoveInY))
	{
		this->setPosX(playerPos[0]);
	}
	if (playerExtremityBottom > 0 &&
			playerExtremityTop < this->maxPos[1] &&
			(canIMoveInY || canIMoveInX))
	{
		this->setPosY(playerPos[1]);
	}
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
void Player::goTo(float x, float y, float incMove, float incRot)
{
	// TODO - fix movement
	float dirVect[2] = {
			(x - this->getPosX()),
			(y - this->getPosY())};
	float dirVectNorm = sqrt(pow(dirVect[0], 2) + pow(dirVect[1], 2));

	float unitVect[2] = {
			dirVect[0] / dirVectNorm,
			dirVect[1] / dirVectNorm};
	float dirAngle = atan2(unitVect[1], unitVect[0]);
	if (isnan(dirAngle))
	{
		dirAngle = _degreeToRadian(90.0);
	}
	float actualAngle = this->getAngle();

	if (actualAngle < dirAngle)
		this->rotate(+incRot);
	else if (actualAngle > dirAngle)
		this->rotate(-incRot);

	if ((this->getAngle() == dirAngle) && (this->getPosX() != x || this->getPosY() != y))
		this->move(incMove);
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
void Player::setLimits(const float x, const float y)
{
	this->maxPos[0] = x;
	this->maxPos[1] = y;
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
	newAngle = _degreeToRadian(newAngle);

	this->setLeftArmAngle(newAngle, 0.0);
	this->setRightArmAngle(newAngle, 0.0);

	this->angle = newAngle;
}
const float Player::getRadius()
{
	return this->bodyRadius;
}
void Player::setRadius(const float r)
{
	this->bodyRadius = abs(r);
}

void Player::setLeftArmAngle(float radianAngle, float degreeAngle)
{
	this->leftArmAngle[0] = radianAngle + _degreeToRadian(degreeAngle) + _degreeToRadian(DEFAULT_ARM_INCLINATION);
	this->leftArmAngle[1] = radianAngle + (_degreeToRadian(degreeAngle) / 2.0);
}
void Player::setRightArmAngle(float radianAngle, float degreeAngle)
{
	this->rightArmAngle[0] = radianAngle - _degreeToRadian(degreeAngle) - _degreeToRadian(DEFAULT_ARM_INCLINATION);
	this->rightArmAngle[1] = radianAngle - (_degreeToRadian(degreeAngle) / 2.0);
}

const bool Player::getCollision()
{
	return this->wasCollision;
}
void Player::setCollision(const bool collision)
{
	this->wasCollision = collision;
}

Player *Player::getOpponent()
{
	return this->opponent;
}
void Player::setOpponent(Player *opponent)
{
	this->opponent = opponent;
}
