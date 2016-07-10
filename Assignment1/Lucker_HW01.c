//********************************************************************************************
// Name:  Justin Lucker
// Program: Assignment1
// CSE 240
//********************************************************************************************

#include <stdio.h>

#pragma warning(disable: 4996)

#define NUM 2

int main()
{
	int input;
	int result;
	printf ("Enter an integer number: ");			// changed scanf to printf
	scanf ("%d", &input);							// added a %

	result = input % NUM;							// Added a semicolon

	if (result != 0)								// changed = into !=
	{
		printf ("\n\nNumber %d is Odd.", input);
	}
	else if (result == 0)							// changed = into ==
	{
		printf("\n\nNumber %d is Even.", input);
	}
	getchar();
	return 0;
}