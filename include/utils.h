#ifndef UTILS_H

#define UTILS_H

#include <iostream>
#include <string>
#include <ctime>

using namespace std;

#define RED_COLOR "red"
#define GREEN_COLOR "green"
#define BLUE_COLOR "blue"
#define YELLOW_COLOR "yellow"
#define PURPLE_COLOR "purple"
#define BLACK_COLOR "black"
#define WHITE_COLOR "white"
#define BEIGE_COLOR "beige"

class Utils
{
public:
	// ! Constructor(s) and Destructor
	Utils();
	~Utils();

	// ? Member Functions
	time_t parseStringToTime(const char *strDate);
	const string parseTimeToString(const time_t time);

	float *getColorArrayByColorName(const string colorName);
};

#endif // UTILS_H
