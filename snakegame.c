// full name: < rony yosef >
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>



#define _CRT_SECURE_NO_WARNINGS
#define MAXCOLS 75
#define MAXROWS 25
#define SIGN '@'
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
typedef struct SnakeNode
{
	int x;
	int y;
	struct SnakeNode* next;
} SnakeNode;

typedef struct snake
{
	SnakeNode* head;
	SnakeNode* tail;
} SNAKE;

typedef struct apple
{
	int x;
	int y;
} Apple;

// terminal escape codes/ANSI escape codes (or ANSI escape sequences)
// "\x1B" is the escape character that tells your terminal that what comes next is not meant to be printed on the screen, 
// but rather a command to the terminal (or most likely terminal emulatoion).

// Turn escape codes ON.
void init(void);

// Moves the cursor to position (x,y) on screen.
// Parameters:
//     x: the row of the posiiton.
//     y: the column of the posiiton.
void gotoxy(int x, int y);

// Delays the execution of the program.
// Parameters:
//     speed: the length of the delay in seconds. 
void sleep(float speed);

void setTheGame(mirror);

void printhashline();

void printFrame(); //print the frame(walls) of the game

void printGameOver();//clean the board and print GAME OVER in the middle of the screan 

void print_x_y(int x, int y, char to_print);//print a char in a specific coordinate

//this function is us to extand the sanke and add new nodes in before the tail
void mirrorBorders(bool mirror[MAXROWS + 3][MAXCOLS + 3]);

void sankeDirections(char currentDirect, SNAKE* snk);//change the sanke head direct

SnakeNode* newSnakeNode(int x, int y);//crate new snake node with (x,y) and pointer to null

void move(SnakeNode* head, int go_to_x, int go_to_y);//change the x,y of a snake node

//this function is us to extand the sanke and add new nodes in before the tail
void insertAfter(SnakeNode * p, int x, int y);// assume p is not NULL

//this function checks if the snake hit one of the walls or hit/eat him salfe
bool checkHit(SnakeNode* head, bool mirror[MAXROWS + 3][MAXCOLS + 3]); 

void freeAll(SNAKE* snk);

void addapple(Apple* apple, bool mirror);

void addapple(Apple* apple, bool mirror[MAXROWS + 3][MAXCOLS + 3])
{
	apple->x = rand() % 25;
	apple->y = rand() % 75;
	while (mirror[apple->x][apple->y]==true)
	{
		apple->x = rand() % 25;
		apple->y = rand() % 75;
	}
	gotoxy(apple->x, apple->y);
	printf("$");
}

int main()
{
	srand(time(0));
	float speed = 0.5;
	bool gameCont = true;
	char currentDirect = RIGHT;
	bool mirror[MAXROWS + 3][MAXCOLS + 3] = { false };
	setTheGame(mirror);
	SnakeNode* node = newSnakeNode(MAXROWS / 2, MAXCOLS / 2);
	SNAKE* snk = (SNAKE*)malloc(sizeof(SNAKE));
	snk->head = node;
	snk->tail = node;
	print_x_y(node->x, node->y, SIGN);

	Apple apple;
	addapple(&apple, mirror);
	
	while (gameCont)
	{
		print_x_y(snk->tail->x, snk->tail->y, ' ');//erase the tail
		mirror[snk->tail->x][snk->tail->y] = false;

		if (_kbhit())
			currentDirect = _getch();
		SnakeNode* node = newSnakeNode(snk->head->x , snk->head->y);
		snk->head->next = node;
		snk->head = node;

		sankeDirections(currentDirect,snk);

		print_x_y(snk->head->x, snk->head->y, SIGN);//print the new place of the head

		if (checkHit(snk->head, mirror))
		{
			gameCont = false;
			break;
		}
		mirror[snk->head->x][snk->head->y] = true;

		sleep(speed);

		if (snk->head->x == apple.x && snk->head->y == apple.y)
		{
			if (speed >= 0.10)
				speed -= 0.05;
			insertAfter(snk->tail, snk->tail->x, snk->tail->y);
			addapple(&apple, mirror);
		}
	
		SnakeNode*temp = snk->tail;
		snk->tail = snk->tail->next;
		free(temp);
	}
	freeAll(snk);
	printGameOver();//GAME OVER
}

//void add_apple(add_apple)

// Turn escape codes ON.
void init(void) {
	system("cls");
}

// Moves the cursor to position (x,y) on screen.
// Parameters:
//     x: the row of the posiiton.
//     y: the column of the posiiton.
void gotoxy(int x, int y) {
	printf("\x1b[%d;%df", x, y);
}

// Delays the execution of the program.
// Parameters:
//     speed: the length of the delay in seconds. 
void sleep(float speed) {
	clock_t clocks_start = clock();

	while (clock() - clocks_start < speed * CLOCKS_PER_SEC)
		;
}

void setTheGame(mirror)
{
	mirrorBorders(mirror);
	init();
	printFrame();
}

void printFrame()
{
	gotoxy(1, 1);
	printhashline();
	for (int i = 2; i <= MAXROWS + 1; i++)
	{
		gotoxy(i, 1);
		printf("#");
		gotoxy(i, MAXCOLS+1);
		printf("#");
	}
	gotoxy(MAXROWS + 2, 1);
	printhashline();
}

void printhashline()
{
	for (int i = 0; i <= MAXCOLS; i++)
		printf("#");
}

void mirrorBorders(bool mirror[MAXROWS + 3][MAXCOLS + 3])
{
	for (int i = 1; i < MAXCOLS + 2; i++)
		mirror[1][i] = true;
	for (int i = 2; i < MAXROWS + 2; i++)
	{
		mirror[i][1] = true;
		mirror[i][MAXCOLS + 1] = true;
	}
	for (int i = 1; i < MAXCOLS + 2; i++)
		mirror[MAXROWS + 2][i] = true;
}

void printGameOver()
{
	init();
	printFrame();
	gotoxy(MAXROWS / 2, (MAXCOLS / 2) - 3);
	printf("GAME OVER");
	gotoxy(MAXROWS+2, MAXCOLS);
}

void print_x_y(int x, int y, char to_print)
{
	gotoxy(x, y);
	printf("%c", to_print);
	gotoxy(MAXROWS + 3, 0);
}

void  sankeDirections(char currentDirect, SNAKE* snk)
{
	switch (currentDirect) // move the head to the currentDirect
	{
	case LEFT:
		move(snk->head, snk->head->x, snk->head->y - 1);
		break;
	case DOWN:
		move(snk->head, snk->head->x + 1, snk->head->y);
		break;
	case RIGHT:
		move(snk->head, snk->head->x, snk->head->y + 1);
		break;
	case UP:
		move(snk->head, snk->head->x - 1, snk->head->y);
		break;
	}
}

SnakeNode* newSnakeNode(int x, int y)
{
	SnakeNode* p = (SnakeNode*)malloc(sizeof(SnakeNode));
	if (p != NULL)
	{
		p->x = x;
		p->y = y;
		p->next = NULL;
	}
	return p;
}

void move(SnakeNode* head, int go_to_x, int go_to_y)
{
	head->x = go_to_x;
	head->y = go_to_y;
}

void insertAfter(SnakeNode * p, int x, int y)
{
	SnakeNode *q = newSnakeNode(x,y);
	q->next = p->next;
	p->next = q;
}

bool checkHit(SnakeNode* head, bool mirror[MAXROWS + 3][MAXCOLS + 3])
{
	if (mirror[head->x][head->y])
		return true;
	else
		return false;
}

void freeAll(SNAKE* snk)
{
	while (snk->tail->next)
	{
		SnakeNode*temp = snk->tail;
		snk->tail = snk->tail->next;
		free(temp);
	}
	free(snk);
}
