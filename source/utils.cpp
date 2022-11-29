#include "../include/utils.h"

Utils::Utils()
{
	//// do nothing
}
Utils::~Utils()
{
	//// do nothing
}

const float _scaleRGBValues(float value)
{
	return value / 255.0;
}
void Utils::setColorArrayByColorName(const string colorName, float colorArray[])
{
	if (colorName == RED_COLOR)
	{
		colorArray[0] = _scaleRGBValues(255);
		colorArray[1] = _scaleRGBValues(0);
		colorArray[2] = _scaleRGBValues(0);
	}
	if (colorName == GREEN_COLOR)
	{
		colorArray[0] = _scaleRGBValues(0);
		colorArray[1] = _scaleRGBValues(255);
		colorArray[2] = _scaleRGBValues(0);
	}
	if (colorName == BLUE_COLOR)
	{
		colorArray[0] = _scaleRGBValues(0);
		colorArray[1] = _scaleRGBValues(0);
		colorArray[2] = _scaleRGBValues(255);
	}
	if (colorName == YELLOW_COLOR)
	{
		colorArray[0] = _scaleRGBValues(255);
		colorArray[1] = _scaleRGBValues(255);
		colorArray[2] = _scaleRGBValues(0);
	}
	if (colorName == PURPLE_COLOR)
	{
		colorArray[0] = _scaleRGBValues(145);
		colorArray[1] = _scaleRGBValues(0);
		colorArray[2] = _scaleRGBValues(255);
	}
	if (colorName == BLACK_COLOR)
	{
		colorArray[0] = _scaleRGBValues(0);
		colorArray[1] = _scaleRGBValues(0);
		colorArray[2] = _scaleRGBValues(0);
	}
	if (colorName == WHITE_COLOR)
	{
		colorArray[0] = _scaleRGBValues(255);
		colorArray[1] = _scaleRGBValues(255);
		colorArray[2] = _scaleRGBValues(255);
	}
	if (colorName == BEIGE_COLOR)
	{
		colorArray[0] = _scaleRGBValues(255);
		colorArray[1] = _scaleRGBValues(200);
		colorArray[2] = _scaleRGBValues(168);
	}
}
