#include "stdio.h"
#include "string.h"

typedef struct{
	int i,j;
} position;

void gameInit(void);
int changeActivePlayer(void);
void saveMove(int m);
int checkWin(void);

int getCoordinateX(int i, int j);
int getCoordinateY(int i, int j);
int getI(int player);
int getJ(int player);
position getPlayerPosition(int player);

void highlightPossibleMoves(void);
void switchMode(void);
void cleanMoves(void);
void setNewPosition(int player, position p);
int tryMove(position p);

void drawWalls(void);
void rotateWall(void);
void placeWall();
int checkPath(void);
