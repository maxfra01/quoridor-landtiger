#include "stdio.h"
#include "string.h"

//player_turn = 0 means player A turn, =1 player B turn

typedef struct{
	int i,j;
} position;

position getPlayerPosition(int player);
void setNewPosition(int player, position p);
void gameInit(void);
int changeActivePlayer();
void placeWall(int i, int j, int orient);
void highlightPossibleMoves(void);
int tryMove(position p);
int checkWin(void);
void switchMode(void);
void checkwalls(void);
int getCoordinateX(int i, int j);
int getCoordinateY(int i, int j);
int getI(int player);
int getJ(int player);
void cleanMoves(void);
void rotateWall(void);
