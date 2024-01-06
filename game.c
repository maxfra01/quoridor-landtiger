#ifndef GAME_WORLD
#define GAME_WORLD
#define SIZE 7
#include "stdio.h"
#include "game.h"
#include "lpc17xx.h"
#include "RIT/RIT.h"
#include "GLCD/GLCD.h" 
#include "timer/timer.h"
volatile int i, j;
volatile int player_turn;
volatile int a_remaining_walls, b_remaining_walls;
volatile int board_main[7][7];
volatile int board_walls[7][7];

extern int seconds;

typedef struct{
	char orientation;
	int row, column;
} wall;

typedef struct{
	int i,j;
} position;

volatile position a_position, b_position;

static int getCoordinateX(int i, int j){
	return j*30 + 15;
}
static int getCoordinateY(int i, int j){
	return i*30 + 15;
}

void gameInit(){
	
	//initialize variables
	player_turn=1;
	a_remaining_walls=8;
	b_remaining_walls=8;
	for(i =0 ; i< 7 ; i++){
		for( j = 0; j<7 ;j++){
			board_main[i][j] = 0;
			board_walls[i][j]= 0 ;		}
	}
	board_main[6][3] = 1;
	board_main[0][3] = -1;
	a_position.i=6;
	a_position.j=3;
	b_position.i=0;
	b_position.j=3;
	
	
	//drawing stuff
	LCD_Clear(White);
	for (i = 0; i<=7; i++){
		//LCD_DrawLine(15, 5+ i*30, 225, 5+i*30, Black);
		LCD_DrawLine(0, i*30, 210, i*30, Black);
	}
	for (i = 0; i<=7; i++){
		//LCD_DrawLine(15+ i*30, 5, 15+i*30, 215, Black);
		LCD_DrawLine( i*30, 0, i*30, 210, Black);
	}
	
	LCD_SetPoint(getCoordinateX(6,3), getCoordinateY(6,3), Red);
	LCD_SetPoint(getCoordinateX(0,3), getCoordinateY(0,3), Green);
	
	GUI_Text(105,300, "20", Black, White);
	
	
	//setup timer and start the game
	init_timer(0, 0x017D7840);		//1 sec timer
	enable_RIT();
	NVIC_SetPriority(TIMER0_IRQn, -3);
	enable_timer(0);
	
	
	return;
}

position getPlayerPosition(int player){
	if (player == 1) return a_position;
	else return b_position;
}

int changeActivePlayer(){
	player_turn = player_turn*(-1);
	return player_turn;
}
#endif