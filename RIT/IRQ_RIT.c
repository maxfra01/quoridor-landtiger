/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "string.h"
#include "RIT.h"
#include "../game.h"
#include "../led/led.h"
#include "../GLCD/GLCD.h"
#include "../button_EXINT/button.h"
#define NOT_POSSIBLE 10
#define SIZE 7

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int down=0;
volatile char selected_move;
extern int player_turn, wall_mode,  tmp_wall_i, tmp_wall_j,  tmp_wall_orient;
extern position possible_moves[4];


void RIT_IRQHandler (void)
{					
	static int up=0;
	static int position=0;
	
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
		/* Joytick UP pressed */
		up++;
		switch(up){
			case 1:
				if (wall_mode == 0){
					//move token UP
					if (possible_moves[0].i != NOT_POSSIBLE && tryMove(possible_moves[0])) {
						selected_move='u';
					}
				}
				else{
					//move wall UP
					if (tmp_wall_i-1>0){
						drawWalls();
						LCD_DrawWall(tmp_wall_j*30, tmp_wall_i*30, Blue, tmp_wall_orient); 
						tmp_wall_i--;
						LCD_DrawWall(tmp_wall_j*30, tmp_wall_i*30, Yellow, tmp_wall_orient); 
					}
				}
				break;
			default:
				break;
		}
	}
	else{
			up=0;
	}
	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){
		/* Joytick RIGHT pressed */
 		up++;
		switch(up){
			case 1:
				if (wall_mode==0){
					//move token RIGHT
					if (possible_moves[1].i != NOT_POSSIBLE && tryMove(possible_moves[1])){
						selected_move='r';
					}
				}
				else{
					//move wall RIGHT
						if (tmp_wall_j+1<SIZE){
							drawWalls();
							LCD_DrawWall(tmp_wall_j*30, tmp_wall_i*30, Blue, tmp_wall_orient); 
							tmp_wall_j++;
							LCD_DrawWall(tmp_wall_j*30, tmp_wall_i*30, Yellow, tmp_wall_orient); 
						}
				}
				break;
			default:
				break;
		}
	}
	else{
		up=0;
	}

	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){
		/* Joytick LEFT pressed */
		up++;
		switch(up){
			case 1:
				if (wall_mode==0){
					//move token LEFT
					if (possible_moves[2].i != NOT_POSSIBLE && tryMove(possible_moves[2])) {
						selected_move='l';
					}
				}
				else{
					//move wall LEFT
						if (tmp_wall_j-1>0){
							drawWalls();
							LCD_DrawWall(tmp_wall_j*30, tmp_wall_i*30, Blue, tmp_wall_orient); 
							tmp_wall_j--;
							LCD_DrawWall(tmp_wall_j*30, tmp_wall_i*30, Yellow, tmp_wall_orient); 
						}
				}
				break;
	
			default:
				break;
		}
	}
	else{
		up=0;
	}
	if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){
		/* Joytick DOWN pressed */
		up++;
		switch(up){
			case 1:
				if (wall_mode==0){
					//move token DOWN
					if (possible_moves[3].i != NOT_POSSIBLE && tryMove(possible_moves[3])){ 
						selected_move='d';
					}
				}
				else{
					//move wall DOWN
						if (tmp_wall_i+1< SIZE){
							drawWalls();
							LCD_DrawWall(tmp_wall_j*30, tmp_wall_i*30, Blue, tmp_wall_orient); 
							tmp_wall_i++;
							LCD_DrawWall(tmp_wall_j*30, tmp_wall_i*30, Yellow, tmp_wall_orient); 
						}
				}
				
				break;

			default:
				break;
		}
	}
	else{
		up=0;
	}
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){
		/* Joytick SELECT pressed */
		up++;
		switch(up){
			case 1:
				if (wall_mode==0){
					//confirm token move
					if (selected_move == 'u'){
						setNewPosition(player_turn, possible_moves[0]);
					}
					if (selected_move == 'r'){
						setNewPosition(player_turn, possible_moves[1]);
					}
					if (selected_move == 'l'){
						setNewPosition(player_turn, possible_moves[2]);
					}
					if (selected_move == 'd'){
						setNewPosition(player_turn, possible_moves[3]);
					}
				}
				else{
					//confirm wall placement
					placeWall();
				}
				break;
	
			default:
				break; 
		}
	}
	else{
		up=0;
	}
	disable_RIT();
	LPC_RIT->RICOUNTER = 0;
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	enable_RIT();
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
