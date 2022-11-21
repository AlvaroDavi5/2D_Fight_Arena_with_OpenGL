#include "../include/utils.h"

Utils::Utils()
{
	//// do nothing
}
Utils::~Utils()
{
	//// do nothing
}

time_t Utils::parseStringToTime(const char *strDate)
{
	struct tm date;
	int day, month, year;

	sscanf(strDate, "%d/%d/%d", &day, &month, &year);

	date.tm_mday = day;
	date.tm_mon = month - 1;
	date.tm_year = year - 1900;
	date.tm_hour = 0;
	date.tm_min = 0;
	date.tm_sec = 0;

	time_t time = mktime(&date);

	return time;
}
const string Utils::parseTimeToString(const time_t time)
{
	struct tm *date;
	char strDate[12];

	date = localtime(&time);
	strftime(strDate, 12, "%d/%m/%Y", date);

	return string(strDate);
}

const float _scaleRGBValues(float value)
{
	return value / 255.0;
}
float *Utils::getColorArrayByColorName(const string colorName)
{
	static float colorArray[3] = {0.0, 0.0, 0.0}; // BLACK_COLOR

	if (colorName == BLUE_COLOR)
	{
		colorArray[0] = _scaleRGBValues(0);
		colorArray[1] = _scaleRGBValues(0);
		colorArray[2] = _scaleRGBValues(255);
	}

	return colorArray;
}
