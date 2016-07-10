//**********************************************************************************************************
// Programmed by:	Justin Lucker
// Program:			Assignment5
// Date:			02/22/2014
// Class:			CSE240
// Description:		This program allows the user to play a maze game and their time is saved in a linked
//					list which can be loaded from a .dat file and then saved to the .dat file so there
//					is a persistant "high-score" list that lasts after the program is closed.
//**********************************************************************************************************

//INCLUDES
#include <stdio.h>
#include <time.h>
#include <sys/timeb.h>
#include "ConsoleUtil.h"
#include <string.h>

//MACROS (VS)

#pragma warning(disable: 4996)

//MACROS (CONSTANTS)

#define LEVEL_WIDTH CONSOLE_WIDTH		//use entire width of console.
#define LEVEL_HEIGHT (CONSOLE_HEIGHT-1)	//minus one, so one line free for text at bottom.

//set of ASCII characters to use as icons
#define ICON_BLANK ' '
#define ICON_WALL '#'
#define ICON_GOAL 'G'
#define ICON_PLAYER '!'

//maximum length of strings (including terminator).
#define MAX_STR_LEN 1024

//save file
#define SAVE_TIMES "times.dat"

//STRUCTURES

//stores player's position.
struct Vector
{
	//in our coordinate system, the top left character is the origin at 0.0.
	//x increases to the right while y increases downward.
	int x;
	int y;
};

//stores player's best time.
struct PlayerTime {
	char		name[MAX_STR_LEN];
	float		seconds;
	struct PlayerTime* next;
};

//GLOBAL VARIABLES
struct PlayerTime* list_head = NULL;


//FORWARD DECLARATIONS
void menu();
void menu_selection(char option);
void besttimes_view();
void besttimes_add(char name[MAX_STR_LEN], float seconds);
void besttimes_load();
void besttimes_save();
void sort_times();
void besttimes_free(struct PlayerTime* node);
void play();
void generate_level(char level[LEVEL_HEIGHT][LEVEL_WIDTH]);
void draw_level(char level[LEVEL_HEIGHT][LEVEL_WIDTH]);
int is_walkable(char level[LEVEL_HEIGHT][LEVEL_WIDTH], int x, int y);
void move_player(struct Vector* player_position, int move_x, int move_y);


//entry point
void main()
{
	char input_buffer;

	printf("\n\nWelcome to MAZE\n");

	//load best times
	besttimes_load();

	//menu and input loop
	do
	{
		menu();

		input_buffer = tolower(getchar());
		dump_key_buffer();
		menu_selection(input_buffer);
	}
	while (input_buffer != 'q');

	//save best times and free linked list
	besttimes_save();
	besttimes_free(list_head);
}

//displays a menu containing actions the user may choose.
void menu()
{
	printf("\nMenu Options\n");
	printf("------------------------------------------------------\n");
	printf("p: Play\n");
	printf("v: View Best Times\n");
	printf("q: Quit\n");

	printf("\n\nPlease enter a choice ---> ");
}

//takes a character representing an inputs menu choice and calls the appropriate
//function to fulfill that choice. Display an error message if the character is
//not recognized.
void menu_selection(char option)
{
	switch(option)
	{
	case 'p':
		play();
		break;

	case 'v':
		besttimes_view();
		break;

	case 'q':
		// main loop will take care of this.
		break;

	default:
		printf("\nError: Invalid Input.  Please try again...");
		break;
	}
}

void besttimes_view()
{
	//TODO: Implement besttimes_view(). This function will display the contents of the
	//      list_head  linked list. If there are no entries in the list, warn the user.
	//      [5 points] Expected output example:
	//	12.235000 : Ruben
	//	27.123000 : Calvin

	struct PlayerTime *temp;
	temp = (struct PlayerTime *) malloc(sizeof(struct PlayerTime));

	if (temp == NULL)
	{
		printf("\nThere are no Best times\n");
	}
	else
	{
		temp = list_head;

		while (temp != NULL)
		{
			printf("%f  :  ", temp->seconds);
			printf("%s \n", temp->name);
			if (temp->next == NULL)
			{
				break;
			}
			else
			{
				temp = temp->next;
			}
		}
	}
}

void besttimes_add(char name[MAX_STR_LEN], float seconds)
{
	//TODO: Implement besttimes_add() so it creates a new node (using heap memory) and
	//      inserts it into the linked list. Sort the list by time (seconds) in ascending
	//      order as they are inserted into the list. [10 points]

	struct PlayerTime *newTime;
	struct PlayerTime *temp;
	newTime = (struct PlayerTime *) malloc(sizeof(struct PlayerTime));
	temp = (struct PlayerTime *) malloc(sizeof(struct PlayerTime));
	// Loads the newTime node with player name and time
	strcpy(newTime->name, name);
	newTime->seconds = seconds;
	newTime->next = NULL;

	if (list_head == NULL || (list_head->seconds >= newTime->seconds))
	{
		newTime->next = list_head;
		list_head = newTime;
	}
	else
	{
		temp = list_head;
		while (temp->next != NULL && temp->next->seconds < newTime->seconds)
		{
			temp = temp->next;
		}
		newTime->next = temp->next;
		temp->next = newTime;
	}

	/*if (newTime == NULL)
	{
		printf("out of memory\n");  return -1;
	}
	// If there are no entries in the linked list then the first entry is the list_head
	else if (list_head == NULL)
	{
		list_head = newTime;
	}
	else
	{
		temp = list_head;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = newTime;
	}*/
}

void besttimes_load()
{
	//TODO: Implement besttimes_load(). Use C file IO to load the contents of a local
	//      file defined as SAVE_TIMES and store it in list_head. (Hint: Maybe
	//      besttimes_add is useful.)  [10 points]

	FILE *fileName;
	char name[MAX_STR_LEN];
	float seconds;
	fileName = fopen("SAVE_TIMES.dat", "rb");
	if (fileName != NULL)
	{
		while (fread(name, MAX_STR_LEN, 1, fileName) == 1)
		{
			fread(&seconds, sizeof(float), 1, fileName);

			besttimes_add(name, seconds);
		}
	}

}

void besttimes_save()
{
	struct PlayerTime *temp;
	temp = (struct PlayerTime *) malloc(sizeof(struct PlayerTime));

	FILE *fileName;
	fileName = fopen("SAVE_TIMES.dat", "wb");
	if (fileName != NULL)
	{
		temp = list_head;
		while (temp != NULL)
		{
			fwrite(temp->name, MAX_STR_LEN, 1, fileName);
			fwrite(&temp->seconds, sizeof(float), 1, fileName);
			temp = temp->next;
		}
	}
	else
	{
		printf("ERROR: Could not open file for saving data! \n");
	}
}

void besttimes_free(struct PlayerTime* node)
{
	//TODO: Implement besttimes_free(). This function will free the list_head linked-list
	//      from heap memory when the program is exited. (Hint: see slide 84.) [5 points]
	if (node != NULL)
	{
		besttimes_free(node->next);
		free(node);
	}
}

void play()
{
	//define the main 2d array. it has a size that is constant at compile time.
	char level[LEVEL_HEIGHT][LEVEL_WIDTH];

	//pick the goal to be the very last position in the map.
	int goal_x = LEVEL_WIDTH-2;
	int goal_y = LEVEL_HEIGHT-2;

	//buffer for the key pressed. initialized to a dummy value so compiler doesn't complain.
	char key_pressed = ' ';

	//for loops
	int i;

	//time for player's start and end. and name. used for best times.
	struct timeb start, end;
	char name[MAX_STR_LEN];

	//holds player's position, defaulted to 1,1.
	struct Vector player_position;
	player_position.x = 1;
	player_position.y = 1;

	//create level in the level 2D array.
	generate_level(level);

	//display the newly created level on the screen.
	console_activate();

	//only need to do this once since the level is static.
	draw_level(level);

	//draw the player - this is the result of calling move_player with a movement of 0,0.
	move_player(&player_position, 0, 0);

	//start the timer
	ftime(&start);

	while(key_pressed != 'q')
	{
		//waits for the user to press one key and then returns it.
		key_pressed = get_input();

		//handles player movement.
		//for each case, we check if the position the player wants to move is walkable,
		//then we move the player if it is possible.
		switch(key_pressed)
		{
		case 'w':
			if(is_walkable(level, player_position.x + 0, player_position.y - 1))
				move_player(&player_position, 0, -1);

			break;

		case 'a':
			if(is_walkable(level, player_position.x - 1, player_position.y + 0))
				move_player(&player_position, -1, 0);
			break;

		case 's':
			if(is_walkable(level, player_position.x + 0, player_position.y + 1))
				move_player(&player_position, 0, 1);
			break;

		case 'd':
			if(is_walkable(level, player_position.x + 1, player_position.y + 0))
				move_player(&player_position, 1, 0);
			break;

		default:
			//don't need to do anything.
			break;
		}

		//check if the player is at the goal and react accordingly
		if (player_position.x == goal_x && player_position.y == goal_y)
		{
			break;
		}
	}

	ftime(&end);

	//only add high score if player didn't force quit
	if(key_pressed != 'q')
	{
		for(i = 0; i < CONSOLE_HEIGHT; i++)
			print_string(0, i, "                                                                                ");

		print_string(0, 0, "COMPLETED! Enter name (name, no spaces, then press enter.): ");

		get_string(name);

		besttimes_add(name, (end.time - start.time) +  (end.millitm - start.millitm) / 1000.0f);
	}

	console_deactivate();

	//WARNING: it seems that on linux this doesn't show the name as its entered or restore the menu correctly.
}

//generates a new level with a goal.
void generate_level(char level[LEVEL_HEIGHT][LEVEL_WIDTH])
{
	int x, y;
	char data[LEVEL_HEIGHT][LEVEL_WIDTH] =
	{{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
	{'#', ' ', ' ', ' ', '#', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
	{'#', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
	{'#', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
	{'#', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', '#', ' ', '#', '#', '#', ' ', '#', ' ', '#', '#', ' ', '#', '#', '#'},
	{'#', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#', '#', '#', '#', '#', ' ', '#', ' ', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', '#'},
	{'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', ' ', '#', '#', '#', '#', '#', ' ', '#', '#', ' ', '#', ' ', ' ', '#', ' ', '#'},
	{'#', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', ' ', ' ', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', '#', ' ', '#', ' ', '#'},
	{'#', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#'},
	{'#', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', '#', ' ', '#', '#', '#', '#', ' ', '#', '#', '#', ' ', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#'},
	{'#', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
	{'#', ' ', ' ', ' ', '#', ' ', ' ', '#', ' ', '#', '#', ' ', '#', '#', '#', ' ', '#', ' ', '#', '#', ' ', '#', '#', '#', ' ', '#', ' ', '#', ' ', '#', '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', '#'},
	{'#', ' ', '#', ' ', '#', '#', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#', ' ', '#', '#', '#', '#', ' ', '#', '#', '#', ' ', '#', '#'},
	{'#', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', '#', '#', '#', '#', '#', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
	{'#', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
	{'#', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', '#', '#', ' ', '#', '#', ' ', '#', '#', ' ', '#', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
	{'#', '#', '#', ' ', '#', ' ', '#', '#', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', '#', ' ', ' ', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
	{'#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#'},
	{'#', ' ', '#', ' ', '#', '#', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
	{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
	{'#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
	{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
	{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'G', '#'},
	{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}};

	for (y = 0; y < LEVEL_HEIGHT; y++)
		for (x = 0; x < LEVEL_WIDTH; x++)
			level[y][x] = data[y][x];
}

//draws the level in the console.
void draw_level(char level[LEVEL_HEIGHT][LEVEL_WIDTH])
{
	int y, x;

	for (y = 0; y < LEVEL_HEIGHT; y++)
		for (x = 0; x < LEVEL_WIDTH; x++)
			print_char(x, y, level[y][x]);
}

//returns 1 if a location is walkable and 0 otherwise. this is a c-style boolean.
int is_walkable(char level[LEVEL_HEIGHT][LEVEL_WIDTH], int x, int y)
{
	if (level[y][x] == ICON_BLANK || level[y][x] == ICON_GOAL)
		return 1;
	else
		return 0;
}

//moves the player on the draw by erasing and redrawing them. adjusts the player's
//position after drawing.
void move_player(struct Vector* player_position, int move_x, int move_y)
{
	print_char(player_position->x, player_position->y, ICON_BLANK);

	player_position->x += move_x;
	player_position->y += move_y;

	print_char(player_position->x, player_position->y, ICON_PLAYER);

	//hack: move cursor out of the way
	print_char(CONSOLE_WIDTH-2, CONSOLE_HEIGHT-1, ' ');
}
