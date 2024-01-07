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
extern volatile int player_turn, wall_mode;


void RIT_IRQHandler (void)
{					
	static int up=0;
	static int position=0;	
	
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
		/* Joytick UP pressed */
		up++;
		switch(up){
			case 1:
				if (tryMove(-1,0)) {
					selected_move='u';

				}
				break;
			default:
				up=0;
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
				if (tryMove(0,1)){
					selected_move='r';
				}
				
				break;
			default:
				up=0;
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
				if (tryMove(0,-1)) {
					selected_move='l';
				}
				
				break;
	
			default:
				up=0;
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
				if (tryMove(1,0)){ 
					selected_move='d';
				}
				
				break;

			default:
				up=0;
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
				if (selected_move == 'u'){
					setNewPosition(player_turn, -1, 0);
				}
				if (selected_move == 'r'){
					setNewPosition(player_turn, 0, 1);
				}
				if (selected_move == 'l'){
					setNewPosition(player_turn, 0, -1);
				}
				if (selected_move == 'd'){
					setNewPosition(player_turn, 1, 0);
				}
				
				break;
	
			default:
				up=0;
				break; 
		}
	}
	else{
		up=0;
	}
	
	LPC_RIT->RICOUNTER = 0;
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
