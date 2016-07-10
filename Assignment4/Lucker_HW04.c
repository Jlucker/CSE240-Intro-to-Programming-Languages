//**********************************************************************************************************
// Programmed by:	Justin Lucker
// Program:			Assignment4
// Date:			02/15/2014
// Class:			CSE240
// Description:		
//**********************************************************************************************************
#include<stdio.h>
#include <string.h>
#include <ctype.h>

#pragma warning(disable: 4996)

// Enumeration type called Subject to represent the courses
typedef enum Subject
{
	CSE = 0,
	MAE = 1,
	CHE = 2,
	CEE = 3,
	EEE = 4
};


// a node to that holds course information
struct Course 
{					
	enum Subject subject;
	int number;
	char teacher[1024];
	int creditHours;
};

struct Course CourseCollection[10];	
int end = 0;						
int creditHours = 0;
void flush();						
void menu_select(char c);
void course_insert();
void schedule_print();
int search_course();
void subject_select();
void course_drop();
int search();
void course_delete(int to_delete);

int main() 
{ 
	char ch = 'i';
	ungetc('\n', stdin);					// inject input buffer with a return character
	do 
	{
		printf("\nWelcome to ASU Class Schedule \n");
		printf("Menu Options\n");
		printf("----------------------------------------------\n");
		printf("a: Add a Class\n");
		printf("d: Drop a Class\n");
		printf("s: Show your Class\n");
		printf("q: Quit\n");
		printf(" \n");
		printf("Total Credits: %d \n", creditHours);
		printf("Please Enter a Choice ---> ");

		flush();							// flush input buffer
		ch = tolower(getchar());			// see tutorial
		menu_select(ch);
	} while (ch != 'q');

	return 0;
}

void flush() 
{
	int c;
	do 
	{
		c = getchar();
	} while (c != '\n' && c != EOF);
}

// Executes the user's selection
void menu_select(char selection) 
{    
	switch (selection) 
	{
		case 'a':
			course_insert();
		break;

		case 'd':
			course_drop();
		break;

		case 's':
			schedule_print();
		break;

		case 'q':
		break;

		default:
			printf("Invalid input\n");
	}	
}

void course_insert() 
{    
	if (end == 10) 
	{
		printf("You have added the Maximum Number of Courses Allowed!\n");
		return -1;
	}
	else 
	{
		int sub = 0;
		printf("What is the Subject? (CSE=0, MAE=1, CHE=2, CEE=3, EEE=4)?\n");
		scanf("%i", &CourseCollection[end].subject);
		// Calls subject_select function to set the subject
		

		printf("What is the Number? (e.g. 240)?\n");
		scanf("%d", &CourseCollection[end].number);

		printf("How many credits is the class (e.g. 3)?\n");
		scanf("%d", &CourseCollection[end].creditHours);

		printf("What is the name of the Teacher?\n");
		scanf("%s", &CourseCollection[end].teacher);
		printf("\n");

		creditHours = creditHours + CourseCollection[end].creditHours;
		end++;
		return 0;
	}
}

void subject_select(int sel)
{
	if (sel == 0)
	{
		printf("CSE");
	}
	else if (sel == 1)
	{
		printf("MAE");
	}
	else if (sel == 2)
	{
		printf("CHE");
	}
	else if (sel == 3)
	{
		printf("CEE");
	}
	else if (sel == 4)
	{
		printf("EEE");
	}
}

// Prints the students current schedule
void schedule_print() 
{	
	if (end == 0) 
	{
		printf("No Courses Added!");
	}
	else 
	{
		printf("\nClass Schedule: \n");
		for (int i = 0; i < end; i++) 
		{
			subject_select(CourseCollection[i].subject);
			printf("%d\t", CourseCollection[i].number);
			printf("%s\n", CourseCollection[i].teacher);
		}
	}
	return 0;
}

int search()
{
	int course_number = 0;

	printf("Enter Number \n");
	scanf("%d", &course_number);

	for (int i = 0; i < end; i++)
	{
		if (course_number == CourseCollection[i].number)
		{
			return i;
		}
	}
	return -1;
}

void course_delete(int to_delete)
{
	if (to_delete == -1)
	{
		printf("That Course does not exist");
	}
	else
	{
		creditHours = creditHours - CourseCollection[to_delete].creditHours;
		for (int i = to_delete; i < end; i++)
		{
			CourseCollection[i].subject = CourseCollection[i + 1].subject;
			strcpy(CourseCollection[i].teacher, CourseCollection[i + 1].teacher);
			CourseCollection[i].number = CourseCollection[i + 1].number;
			CourseCollection[i].creditHours = CourseCollection[i + 1].creditHours;

		}
		end--;
	}
}

void course_drop() 

{
	int i = search();
	course_delete(i);
}



