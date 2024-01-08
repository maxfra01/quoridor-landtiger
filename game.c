#ifndef GAME_WORLD
#define GAME_WORLD
#endif
#define SIZE 7
#define NOT_POSSIBLE 10
#include "stdio.h"
#include "game.h"
#include "lpc17xx.h"
#include "RIT/RIT.h"
#include "GLCD/GLCD.h" 
#include "timer/timer.h"
volatile int i, j, tmp_wall_i, tmp_wall_j, tmp_wall_orient;
volatile int player_turn, wall_mode;
volatile int a_remaining_walls, b_remaining_walls;
volatile int board_main[7][7];
volatile int board_walls[7][7];
volatile position pos;
volatile char string[100];

extern int seconds;
extern volatile char selected_move;

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
	
	player_turn=1;
	wall_mode=0;
	a_remaining_walls=8;
	b_remaining_walls=8;
	for(i =0 ; i< SIZE ; i++){
		for( j = 0; j<SIZE ;j++){
			board_main[i][j] = 0;
			board_walls[i][j]= -1 ;		}
	}
	board_main[6][3] = 1;
	board_main[0][3] = -1;
	a_position.i=6;
	a_position.j=3;
	b_position.i=0;
	b_position.j=3;
	
	tmp_wall_i=3;
	tmp_wall_j=4;
	tmp_wall_orient=0;
	
	//drawing stuff
	LCD_Clear(Black);
	for (i = 0; i<=7; i++){
		LCD_DrawLine(0, i*30, 210, i*30, Blue);
	}
	for (i = 0; i<=7; i++){
		LCD_DrawLine( i*30, 0, i*30, 210, Blue);
	}
	
	LCD_DrawToken(getCoordinateX(6,3), getCoordinateY(6,3), Red);
	LCD_DrawToken(getCoordinateX(0,3), getCoordinateY(0,3), Green);
	GUI_Text(80,230, (uint8_t *)  "Turn A", Red, Black);
	sprintf(string, "Player A has %d walls left", a_remaining_walls);
	GUI_Text(5,260, (uint8_t *)  string, Red, Black);
	sprintf(string, "Player B has %d walls left", b_remaining_walls);
	GUI_Text(5,280, (uint8_t *) string, Green, Black);
	GUI_Text(5,300, "Remaining time: 20", White, Black);
	
	//setup timer and start the game
	init_timer(0, 0x017D7840);		//1 sec timer
	NVIC_SetPriority(TIMER0_IRQn, -3);
	enable_timer(0);
	highlightPossibleMoves();
	NVIC_EnableIRQ(EINT1_IRQn);
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
	
	if (player_turn==1){
		LCD_DrawToken(getCoordinateX(i,j), getCoordinateY(i,j), Red);
	}
	else{
		LCD_DrawToken(getCoordinateX(i,j), getCoordinateY(i,j), Green);
	}
	
	drawWalls();
	
	sprintf(string, "Player A has %d walls left", a_remaining_walls);
	GUI_Text(5,260, (uint8_t *)  string, Red, Black);
	sprintf(string, "Player B has %d walls left", b_remaining_walls);
	GUI_Text(5,280, (uint8_t *) string, Green, Black);
	
	disable_RIT();
	LPC_RIT->RICOUNTER= 0;
	selected_move = 'x';
	
	player_turn = player_turn*(-1);
	if (player_turn==1){
			GUI_Text(80,230, (uint8_t *)  "Turn A", Red, Black);
		}
		else{
			GUI_Text(80,230, (uint8_t *) "Turn B", Green, Black);
		}
	wall_mode=0;
	highlightPossibleMoves();
	seconds=20;
	GUI_Text(5,300, "Remaining time: 20", White, Black);
	enable_RIT();
	return player_turn;
}

void highlightPossibleMoves(void){

	pos = getPlayerPosition(player_turn);
	i = pos.i;
	j = pos.j;
	
	//first possible move UP
	if (board_main[i-1][j]==player_turn*(-1)){
		//check if can jump over 
		if (i-2>=0 && board_walls[i+1][j] != 0 && board_walls[i+1][j+1] != 0){
			possible_moves[0].i= -2;
			possible_moves[0].j = 0; 
			LCD_SquareColor(j*30, (i-2)*30, Yellow);
		}
		else {
			possible_moves[0].i= NOT_POSSIBLE;
			possible_moves[0].j = NOT_POSSIBLE; 
		}
	}
	else
	{
		if (i-1 < SIZE && i-1 >= 0 && board_walls[i][j]!= 0 && board_walls[i][j+1]!=0){
		LCD_SquareColor(j*30, (i-1)*30, Yellow);
		possible_moves[0].i= -1;
		possible_moves[0].j = 0; 
	} 
	else {
		possible_moves[0].i= NOT_POSSIBLE;
		possible_moves[0].j = NOT_POSSIBLE; 
	}
}
	
	
	//move RIGHT
	if (board_main[i][j+1]==player_turn*(-1)){
		//check if can jump over on right
		if (j+2<SIZE && board_walls[i][j+2]!=1 && board_walls[i+1][j+2] != 1){
			possible_moves[1].i= 0;
			possible_moves[1].j = 2; 
			LCD_SquareColor((j+2)*30, (i)*30, Yellow);
		}
		else{
			possible_moves[1].i= NOT_POSSIBLE;
			possible_moves[1].j = NOT_POSSIBLE;
		}
	}
	else{
		if (j+1 < SIZE && board_walls[i][j+1]!= 1 && board_walls[i+1][j+1] != 1) {
			LCD_SquareColor((j+1)*30, i*30, Yellow);
			possible_moves[1].i= 0;
			possible_moves[1].j = 1; 
		}
		else{
			possible_moves[1].i= NOT_POSSIBLE;
			possible_moves[1].j = NOT_POSSIBLE; 
	}
}
	//move LEFT
	if (board_main[i][j-1]==player_turn*(-1)){
		//check if can jump over on left
		if (j-2>=0 && board_walls[i][j-1]!= 1 && board_walls[i+1][j-1]!= 1){
			possible_moves[2].i= 0;
			possible_moves[2].j = -2; 
			LCD_SquareColor((j-2)*30, (i)*30, Yellow);
		}
		else{
			possible_moves[2].i= NOT_POSSIBLE;
			possible_moves[2].j = NOT_POSSIBLE;
		}
	}
	else{
		if (j-1 >=0 && board_walls[i][j] != 1 && board_walls[i+1][j] != 1) {
			LCD_SquareColor((j-1)*30, i*30, Yellow);
			possible_moves[2].i= 0;
			possible_moves[2].j = -1; 
		}
		else{
			possible_moves[2].i= NOT_POSSIBLE;
			possible_moves[2].j = NOT_POSSIBLE; 
		}
	}
	
	//move DOWN
	if (board_main[i+1][j]==player_turn*(-1)){
		//check if can jump over on down
		if (i+2<SIZE && board_walls[i+2][j] != 0 && board_walls[i+2][j+1]!= 0){
			possible_moves[3].i= 2;
			possible_moves[3].j = 0; 
			LCD_SquareColor(j*30, (i+2)*30, Yellow);
		}
		else{
			possible_moves[3].i= NOT_POSSIBLE;
			possible_moves[3].j = NOT_POSSIBLE;
		}
	}
	else{
		if (i+1 < SIZE && board_walls[i+1][j] != 0 && board_walls[i+1][j+1] !=0){
			LCD_SquareColor(j*30, (i+1)*30, Yellow);
			possible_moves[3].i= 1;
			possible_moves[3].j = 0; 
		}
			else{
			possible_moves[3].i= NOT_POSSIBLE;
			possible_moves[3].j = NOT_POSSIBLE; 
		}
	}
}

int tryMove(position p){
	
	return 1;
}

void setNewPosition(int player, position p){
	cleanMoves();
	pos = getPlayerPosition(player_turn);
	i = pos.i;
	j = pos.j;
	board_main[i][j] = 0;
	LCD_SquareColor(j*30, i*30, Black);

	if (player == 1){
		a_position.i+= p.i;
		a_position.j+= p.j;
		board_main[a_position.i][a_position.j] = 1;
	}
	else{
		b_position.i+= p.i;
		b_position.j+= p.j;
		board_main[b_position.i][b_position.j] = -1;
	}
	LPC_RIT->RICOUNTER = 0;
	if (!checkWin())
		changeActivePlayer();
	return;
}

void switchMode(void){
	if (wall_mode==1){
		//go to move mode
		LCD_DrawWall(tmp_wall_j*30, tmp_wall_i*30, Blue, tmp_wall_orient); //delete temporary wall
		wall_mode=0;
		tmp_wall_i=3;
		tmp_wall_j=4;
		tmp_wall_orient=0;
		NVIC_DisableIRQ(EINT2_IRQn);
		highlightPossibleMoves();
	}
	else {
		//go to wall mode
		cleanMoves();
		LCD_DrawWall(tmp_wall_j*30, tmp_wall_i*30, Yellow, tmp_wall_orient);
		wall_mode=1;
		NVIC_EnableIRQ(EINT2_IRQn);
		
	}
}

void cleanMoves(void){
	pos = getPlayerPosition(player_turn);
	for (i = 0; i< 4  ; i++){
		if (possible_moves[i].i != NOT_POSSIBLE)
			LCD_SquareColor(30*(pos.j + possible_moves[i].j), 30* (pos.i+ possible_moves[i].i), Black);
	}
	LCD_DrawWall(tmp_wall_j*30, tmp_wall_i*30, Blue, tmp_wall_orient);
}

void rotateWall(void){
	LCD_DrawWall(tmp_wall_j*30, tmp_wall_i*30, Blue, tmp_wall_orient);
	drawWalls();
	if (tmp_wall_orient == 0){ 
		tmp_wall_orient = 1;
	}
	else {
		tmp_wall_orient = 0;
	}
	LCD_DrawWall(tmp_wall_j*30, tmp_wall_i*30, Yellow, tmp_wall_orient);
}

int checkWin(void){
	if (player_turn == 1 ) {
		for (i = 0 ; i < SIZE ; i++){
			if (board_main[0][i] == 1){
				NVIC_DisableIRQ(TIMER0_IRQn);
				disable_timer(0);
				LPC_TIM0->IR = 1;			/* clear interrupt flag */
				disable_RIT();
				reset_RIT();
				LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
				NVIC_DisableIRQ(EINT1_IRQn);
				NVIC_DisableIRQ(EINT2_IRQn);
				LCD_Clear(Black);
				GUI_Text(5, 150, (uint8_t *) "Player A wins!", Red, Black);
				NVIC_EnableIRQ(EINT0_IRQn);
				return 1;
			}
		}
	}
	else{
		for (i = 0 ; i < SIZE ; i++){
			if (board_main[6][i] == -1){
				NVIC_DisableIRQ(TIMER0_IRQn);
				disable_timer(0);
				LPC_TIM0->IR = 1;			/* clear interrupt flag */
				disable_RIT();
				reset_RIT();
				LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
				NVIC_DisableIRQ(EINT1_IRQn);
				NVIC_DisableIRQ(EINT2_IRQn);
				LCD_Clear(Black);
				GUI_Text(5, 150, (uint8_t *) "Player B wins!", Green, Black);
				NVIC_EnableIRQ(EINT0_IRQn);
				return 1;
			}
		}
	}
	return 0;
}


void placeWall(){ 
	volatile int ok=0;	
	if (board_walls[tmp_wall_i][tmp_wall_j] == -1){
		if (tmp_wall_orient==0){
			if (!(board_walls[tmp_wall_i][tmp_wall_j+1]== 0) && !(board_walls[tmp_wall_i][tmp_wall_j-1]== 0)){
				ok=1;
			}
		}
		else{
			if (!(board_walls[tmp_wall_i+1][tmp_wall_j]== 1) && !(board_walls[tmp_wall_i-1][tmp_wall_j]== 1)){
				ok=1;
			}
		}
	}
		
	if (ok && checkPath()){
		board_walls[tmp_wall_i][tmp_wall_j] =tmp_wall_orient;
		LCD_DrawWall(tmp_wall_j*30, tmp_wall_i*30, Red, tmp_wall_orient);
		if (player_turn==1){
			a_remaining_walls--;
		}
		else{
			b_remaining_walls--;
		}
		changeActivePlayer();
	}
	return;
}


void drawWalls(void){
	for(i =0 ; i< SIZE ; i++){
		for( j = 0; j<SIZE ;j++){
			if (board_walls[i][j] != -1){
				LCD_DrawWall(j*30, i*30, Red, board_walls[i][j]);
			}	
		}
	}
}

int checkPath(void){
	volatile int tmp_board[7][7], tmp_board_walls[7][7], tmp_i, tmp_j,end, write;
	for(i =0 ; i< SIZE ; i++){
		for( j = 0; j<SIZE ;j++){
			tmp_board[i][j] = 0;
			tmp_board_walls[i][j] = board_walls[i][j];
		}
	}
	tmp_board_walls[tmp_wall_i][tmp_wall_j] = tmp_wall_orient;
	tmp_i = getPlayerPosition(player_turn*-1).i;
	tmp_j = getPlayerPosition(player_turn*-1).j;
	tmp_board[tmp_i][tmp_j]=1;
	end = 0;
	write = 1;
	
	while(!end && write){
		write = 0;
		for(i =0 ; i< SIZE ; i++){
			for( j = 0; j<SIZE ;j++){
				
				//explore board
				if (tmp_board[i][j]==1){
					
					if (player_turn*-1==1 && i == 0 && tmp_board[i][j]==1){
					end = 1;
				}
					if (player_turn*-1==-1 && i == 6 && tmp_board[i][j]==1){
					end = 1;
				}
					
					if (i-1>=0 && tmp_board_walls[i][j] != 0 && tmp_board_walls[i][j+1] !=0 && tmp_board[i-1][j]==0){
						tmp_board[i-1][j] = 1;
						write = 1;
					}
					if (j+1<SIZE && tmp_board_walls[i][j+1] != 1 && tmp_board_walls[i+1][j+1] !=1 && tmp_board[i][j+1]==0){
						tmp_board[i][j+1] = 1;
						write = 1;
					}
					if (j-1>=0 && tmp_board_walls[i][j] != 1 && tmp_board_walls[i+1][j] !=1 && tmp_board[i][j-1]==0){
						tmp_board[i][j-1] = 1;
						write = 1;
					}
					if (i+1<SIZE && tmp_board_walls[i+1][j] != 0 && tmp_board_walls[i+1][j+1] !=0 && tmp_board[i+1][j]==0){
						tmp_board[i+1][j] = 1;
						write = 1;
					}
					tmp_board[i][j]=2;
				}
			}
		}
	}
	if (end){
		//valid wall
		return 1;
	}
	else{
		//not valid
		return 0;
	}
	
}