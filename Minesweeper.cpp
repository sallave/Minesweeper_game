// Minesweeper.cpp : Defines the entry point for the console application.
//Salla Vesterinen 1706248: MINESWEEPER

#include "stdafx.h"
#include <stdio.h>
#include <cstdlib>	//use of rand & srand
#include <ctime>	//use of time (for randomization)
#include <stdbool.h>	//use of bool for c
#pragma warning(disable:4996)	//use of scanf without '_s'

//making two dimentional arrays of the board values and what the player sees
int value[10][10];	//value of the square (-1 for mine, 0 for empty, 1-8 as mine count)
char shown[10][10];	//what the player sees as the square ('X' for unopened, '?' for flag, or the value ^)

void start();	//starts new game: makes game values
int countMines(int x, int y);	//function used by start: counts mines next to a opened square
void printField();	//prints border numbers and field values
void playerInput();	//asks user input and opens functions accordingly
void uncover();	//asks user input and checks if in range
void open(int x, int y);	//used by uncover: opens square and puts number (or if zeroes expands)
void setFlag();	//places flag onto board
bool newGame();	//asks whether to start again or not

//game flow structurally
int main() {
	printf("Minesweeper!\n\n");

	start();
	//^ function for starting new game: making game values
	bool play = true;
	while (play = true) {
		printField();
		//^ function that prints square
		playerInput();
		//^ functions that asks user input and opens functions accordingly

		if (!play) {
			play = newGame();
			//^ function that asks user if you want to start a new game
			//if answer was no the program would exit
			//if yes then goes to start();
			start();
		}
	}
	return 0;
}

//starts new game: makes game values
void start() {
	printf("New game\n");

	//set everything seen to X's
	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 10; x++) {
			shown[x][y] = 'X';
		}
	}
	//randomize mines
	srand((unsigned int)time(NULL));//debug with value instead of time, so the board stays the same
			//^ time() returns a 64-bit integer, and srand() wants on of 32- so conversion is needed

	int i, x, y;
	//this will guarantee max 10 mines, but not a min
	//(because it can choose the same square multiple times)
	for (i = 0; i < 10; i++) {
		x = rand() % 10;
		y = rand() % 10;
		value[x][y] = -1;
	}
	//place numbers
	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 10; x++) {
			value[x][y] = countMines(x, y);
			//using function for counting surrounding mines ^
		}
	}
}
//function used by start: counts mines next to a opened square
int countMines(int x, int y) {
	int number = 0;
	//checks if the square is a mine
	if (value[x][y] == -1) {
		return -1;
	}
	//checking surrounding squares (board goes from 0-9):
	//North-West sqaure
	if (x-1 >= 0 && y-1 >= 0) {
		if (value[x-1][y-1] == -1) {
			number++;
		}
	}	//N square
	if (y-1 >= 0) {
		if (value[x][y-1] == -1) {
			number++;
		}
	}	//NE square
	if (x+1 <= 9 && y-1 >= 0) {
		if (value[x+1][y-1] == -1) {
			number++;
		}
	}	//W square
	if (x-1 >= 0) {
		if (value[x-1][y] == -1) {
			number++;
		}
	}	//E square
	if (x+1 <= 9) {
		if (value[x+1][y] == -1) {
			number++;
		}
	}	//SW square
	if (x-1 >= 0 && y+1 <= 9) {
		if (value[x-1][y+1] == -1) {
			number++;
		}
	}	//S square
	if (y+1 <= 9) {
		if (value[x][y+1] == -1) {
			number++;
		}
	}	//SE square
	if (x+1 <= 9 && y+1 <= 9) {
		if (value[x+1][y+1] == -1) {
			number++;
		}
	}
	return number;
}

//prints border numbers and field values
void printField() {
	printf("\n");
	//clearing the screen
	system("CLS");
	//printing top border numbers
	printf("   ");
	for (int x = 1; x <= 10; x++) {
		printf("%d ", x);
	}
	printf("\n");
	//printing left border number
	for (int y = 0; y < 10; y++) {
		//aligning the board (rows 1-9 have two spaces, row 10 has one becuase of double digits)
		if (y == 9) {
			printf("%d ", y + 1);
		}
		else {
			printf("%d  ", y + 1);
		}
		//printing content of that line
		for (int x = 0; x < 10; x++) {
			//printing an unopened square or a square with a flag
			if (shown[x][y] == 'X' || shown[x][y] == '?') {
				printf("%c ", shown[x][y]);
			}
			//if they've been opened
			else {
				//if it was empty
				if (value[x][y] == 0) {
					printf(". ");
				}
				//if the player lost
				else if (value[x][y] == -1) {
					printf("Bomb!\n");
					newGame();
					return;
				}
				//else print the number
				else {
					printf("%d ", value[x][y]);
				}
			}
		}
		printf("\n");
	}

	//CHECK IF PLAYER WON:
	//count bombs
	int bombCount = 0;
	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 10; y++) {
			if (value[x][y] == -1) {
				bombCount++;
			}
		}
	}
	//counting unopened squares
	int unOpened = 0;
	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 10; y++) {
			if (shown[x][y] == 'X' || shown[x][y] == '?') {
				unOpened++;
			}
		}
	}
	//seeing if number of unopened squares matches number of bombs
	if (unOpened == bombCount) {
		printf("WIN!\n");
		newGame();
	}
	return;
}

//asks user input and opens functions accordingly
void playerInput() {
	printf("Choices: o (open square), f (set flag), s (start new game)\n");
	bool selected = false;
	while (selected = true) {
		char choice = getchar();
		if (choice == '\n') {
			return;
		}
		else if (choice == 'o') {
			uncover();
			selected = true;
			return;
		}
		else if (choice == 'f') {
			setFlag();
			selected = true;
			return;
		}
		else if (choice == 's') {
			start();
			selected = true;
			return;
		}
		else {
			//will only barely be seen, since the screen is cleared right after
			printf("?\n");
		}
	}
}

//asks user input and checks if in range
void uncover() {
	bool selected = false;
	while (!selected) {
		int x,y = 0;
		printf("Input column (1-10)\n");
		scanf("%d", &x);
		//to get value in array:
		x--;
		printf("Input row (1-10)\n");
		scanf("%d", &y);
		y--;
		//check if x&y are in range:
		if ((x >= 0 && x <= 9) && (y >= 0 && y <= 9)) {
			open(x, y);
			//^ call function for opening and revealing the value of the square
			selected = true;
		}
		else {
			printf("values not in range, start again\n");
		}
	}
}
//used by uncover: opens square, if zeroes expands
void open(int x, int y) {
	//check if values are in range
	if (x >= 0 && x <= 9 && y >= 0 && y <= 9) {
		//if already open
		if (shown[x][y] != 'X') {
			return;
		}
		//set it to open (not seen by player)
		shown[x][y] = 'o';
		//if it's a mine or not empty
		if (value[x][y] == -1 || value[x][y] != 0) {
			return;
		}
		//doing those again for the ones touching the empty square
		//North
		open(x-1, y-1);
		open(x, y-1);
		open(x+1, y-1);
		//West and East
		open(x-1, y);
		open(x+1, y);
		//South
		open(x-1, y+1);
		open(x, y+1);
		open(x+1, y+1);
	}
	else {
		return;
	}
}

//places flag onto board
void setFlag() {
	bool selected = false;
	while (!selected) {
		int x,y = 0;
		printf("Input column (1-10)\n");
		scanf("%d", &x);
		//to get value in array:
		x--;
		printf("Input row (1-10)\n");
		scanf("%d", &y);
		y--;
		//check if value is in range
		if ((x >= 0 && x <= 9) && (y >= 0 && y <= 9)) {
			shown[x][y] = '?';
			selected = true;
		}
		else {
			printf("values not in range, start again\n");
		}
	}
}

//asks whether to start again or not
bool newGame() {
	bool selected = false;
	while (!selected) {
		char choice;
		printf("Continue? y or n\n");
		while ((getchar()) != '\n');	//clearing the input buffer
		choice = getchar();
		if (choice == 'n') {
			exit(0);
		}
		//goes back to main
		else if (choice == 'y') {
			while ((getchar()) != '\n');	//clearing the input buffer
			return true;
			selected = true;
		}
		else {
			printf("try again\n");
		}
	}
	return false;
}