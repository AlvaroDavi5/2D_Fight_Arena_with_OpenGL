#include <iostream>
#include <string>
#include <ctime>
#include <locale>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "../include/utils.h"

using namespace std;

int main(int argc, char *argv[])
{

	setlocale(LC_NUMERIC, "pt_BR");
	auto loc = locale("pt_BR.UTF-8");
	locale::global(loc);

	try
	{
		if (argc < 4)
		{
			cout << "Please enter the correct number of arguments (3 arguments)" << endl;
			return 1;
		}

		Utils util = Utils();
	}
	catch (const exception *e)
	{
		cout << "An unexpected error has ocurred!" << endl;
		cerr << e->what() << endl;
		return 1;
	}

	cout.imbue(locale("C"));

	return 0;
}
