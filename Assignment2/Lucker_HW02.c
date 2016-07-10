//**********************************************************************************************************
// Programmed by:	Justin Lucker
// Program:			Assignment2
// Date:			01/31/2014
// Class:			CSE240
// Description:		This program calculates the combined volume of a user specified amount of cylinders
//**********************************************************************************************************



//includes
#include <stdio.h>

//macros: visual studio flags
#pragma warning(disable: 4996)

// sets PI as a constant
#define PI 3.14159265

int main()
{
	int input;
	int x;
	int count;
	float height;
	float radius;
	float volume;
	float totalSum;

	printf ("How many cylinders would you like to sum? \n");
	scanf ("%d", &input);

	totalSum = 0;
	for (x = 0; x < input; x++)
	{
		// Prompts for height of cylinder and stores as float height
		printf("\nHow high is cylinder %d?\n",(x + 1));
		scanf("%f", &height);

		// Prompts for radius of cylinder and stores as float radius
		printf("\nWhat is cylinder %d's radius? \n", (x + 1));
		scanf("%f", &radius);

		// volume of each cylinder
		volume = (PI * (radius * radius) * height);

		// current total volume of all cylinders
		totalSum = totalSum + volume;

		//totalSum = (totalSum + (PI * (radius * radius) * height));  <-- w/o the use of an additional veriable
	}
	// Prints the totalSum
	printf("\nThe Total Sum is: %f", totalSum);
	getchar();
	return 0;

}