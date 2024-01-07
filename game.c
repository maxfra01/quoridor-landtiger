#ifndef GAME_WORLD
#define GAME_WORLD
#define SIZE 7
#define NOT_POSSIBLE 10
#include "stdio.h"
#include "game.h"
#include "lpc17xx.h"
#include "RIT/RIT.h"
#include "GLCD/GLCD.h" 
#include "timer/timer.h"
volatile int i, j;
volatile int player_turn, wall_mode;
volatile int a_remaining_walls, b_remaining_walls;
volatile int board_main[7][7];
volatile int board_walls[7][7];
volatile position pos;

extern int seconds;

typedef struct{
	char orientation;
	int row, column;
} wall;


volatile position a_position, b_position, possible_moves[4];

int getCoordinateX(int i, int j){
	return j*30 + 15;
}
int getCoordinateY(int i, int j){
	return i*30 + 15;
}

void gameInit(){
	
	//initialize variables
	player_turn=1;
	wall_mode=0;
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
	LCD_Clear(Black);
	for (i = 0; i<=7; i++){
		//LCD_DrawLine(15, 5+ i*30, 225, 5+i*30, Black);
		LCD_DrawLine(0, i*30, 210, i*30, Blue);
	}
	for (i = 0; i<=7; i++){
		//LCD_DrawLine(15+ i*30, 5, 15+i*30, 215, Black);
		LCD_DrawLine( i*30, 0, i*30, 210, Blue);
	}
	
	
	//LCD_SetPoint(getCoordinateX(6,3), getCoordinateY(6,3), Red);
	//LCD_SetPoint(getCoordinateX(0,3), getCoordinateY(0,3), Green);
	//LCD_SquareColor(getCoordinateX(6,3), getCoordinateY(6,3), Red);
	//LCD_SquareColor(getCoordinateX(0,3), getCoordinateY(0,3), Green);
	
	LCD_DrawToken(getCoordinateX(6,3), getCoordinateY(6,3), Red);
	LCD_DrawToken(getCoordinateX(0,3), getCoordinateY(0,3), Green);
	
	GUI_Text(5,300, "Remaining time: 20", White, Black);
	GUI_Text(5,240, (uint8_t *)  "Turn A", White, Black);
	
	
	//setup timer and start the game
	init_timer(0, 0x017D7840);		//1 sec timer
	NVIC_SetPriority(TIMER0_IRQn, -3);
	enable_timer(0);
	highlightPossibleMoves();
	enable_RIT();
	
	
	return;
}

position getPlayerPosition(int player){
	if (player == 1) return a_position;
	else return b_position;
}

int getI(int player){
	if (player==1) return a_position.i;
	return b_position.i;
}
int getJ(int player){
	if (player==1) return a_position.j;
	return b_position.j;
}

int changeActivePlayer(){
	pos = getPlayerPosition(player_turn);
	i = pos.i;
	j = pos.j;
	if (board_main[i-1][j] != player_turn*-1) 
		LCD_SquareColor(j*30, (i-1)*30, Black);
	if (board_main[i+1][j] != player_turn*-1) 
		LCD_SquareColor(j*30, (i+1)*30, Black);
	if (board_main[i][j+1] != player_turn*-1) 
		LCD_SquareColor((j+1)*30, (i)*30, Black);
	if (board_main[i][j-1] != player_turn*-1) 
		LCD_SquareColor((j-1)*30, (i)*30, Black);
	
	if (player_turn==1){
		LCD_DrawToken(getCoordinateX(i,j), getCoordinateY(i,j), Red);
	}
	else{
		LCD_DrawToken(getCoordinateX(i,j), getCoordinateY(i,j), Green);
	}
	
	LPC_RIT->RICOUNTER= 0;
	
	player_turn = player_turn*(-1);
	if (player_turn==1){
			GUI_Text(5,240, (uint8_t *)  "Turn A", White, Black);
		}
		else{
			GUI_Text(5,240, (uint8_t *) "Turn B", White, Black);
		}
	wall_mode=0;
	highlightPossibleMoves();
	seconds=20;
	GUI_Text(5,300, "Remaining time: 20", White, Black);
	return player_turn;
}

void highlightPossibleMoves(void){

	
	
	pos = getPlayerPosition(player_turn);
	i = pos.i;
	j = pos.j;
	
	//first possible move UP
	if (board_main[i-1][j]==player_turn*(-1)){
		//check if can jump over
		//highlight UP move [i-2][j]
	}
	if (i-1 < SIZE){
		LCD_SquareColor(j*30, (i-1)*30, Yellow);
		possible_moves[0].i= -1;
		possible_moves[0].j = 0; 
	} 
	else {
		possible_moves[0].i= NOT_POSSIBLE;
		possible_moves[0].j = NOT_POSSIBLE; 
	}
	
	//move RIGHT
	if (j+1 < SIZE) {
		LCD_SquareColor((j+1)*30, i*30, Yellow);
		possible_moves[1].i= 0;
		possible_moves[1].j = 1; 
	}
	else{
		possible_moves[1].i= NOT_POSSIBLE;
		possible_moves[1].j = NOT_POSSIBLE; 
	}
	
	//move LEFT
	if (j-1 < SIZE) {
		LCD_SquareColor((j-1)*30, i*30, Yellow);
		possible_moves[2].i= 0;
		possible_moves[2].j = -1; 
	}
		else{
		possible_moves[2].i= NOT_POSSIBLE;
		possible_moves[2].j = NOT_POSSIBLE; 
	}
	
	//move DOWN
	if (i+1 < SIZE){
		LCD_SquareColor(j*30, (i+1)*30, Yellow);
		possible_moves[3].i= 1;
		possible_moves[3].j = 0; 
	}
		else{
		possible_moves[3].i= NOT_POSSIBLE;
		possible_moves[3].j = NOT_POSSIBLE; 
	}
}

int tryMove(int v, int o){
	
	for (i = 0; i < 4; i++){
		if (possible_moves[i].i == v && possible_moves[i].j == o) return 1;
	}
	return 0;
}

void setNewPosition(int player, int v, int o){
	pos = getPlayerPosition(player_turn);
	i = pos.i;
	j = pos.j;
	board_main[i][j] = 0;
	LCD_SquareColor(j*30, i*30, Black);
	if (board_main[i-1][j] != player_turn*-1) 
		LCD_SquareColor(j*30, (i-1)*30, Black);
	if (board_main[i+1][j] != player_turn*-1) 
		LCD_SquareColor(j*30, (i+1)*30, Black);
	if (board_main[i][j+1] != player_turn*-1) 
		LCD_SquareColor((j+1)*30, (i)*30, Black);
	if (board_main[i][j-1] != player_turn*-1) 
		LCD_SquareColor((j-1)*30, (i)*30, Black);
	if (player == 1){
		a_position.i+= v;
		a_position.j+= o;
		board_main[a_position.i][a_position.j] = 1;
	}
	else{
		b_position.i+= v;
		b_position.j+= o;
		board_main[b_position.i][b_position.j] = -1;
	}
	LPC_RIT->RICOUNTER = 0;
	changeActivePlayer();
}

void switchMode(void){
	if (wall_mode==1){
		//go to move mode
		wall_mode=0;
		highlightPossibleMoves();
	}
	else {
		//go to wall mode
		wall_mode=1;
	}
}


#endif