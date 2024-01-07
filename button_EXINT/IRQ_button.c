#include "button.h"
#include "lpc17xx.h"
#include "../GLCD/GLCD.h"
#include "../game.h"

extern int down;
extern int a_remaining_walls, b_remaining_walls, player_turn,  tmp_wall_orient;

void EINT0_IRQHandler (void)	  	/* INT0														 */
{		
	NVIC_DisableIRQ(EINT0_IRQn);
	NVIC_DisableIRQ(EINT1_IRQn);
	NVIC_DisableIRQ(EINT2_IRQn);
	gameInit();
	
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	if (player_turn==1 && a_remaining_walls> 0 ){
		switchMode();
	}
	if (player_turn==-1 && b_remaining_walls> 0){
		switchMode();
	}
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	
	rotateWall();
	drawWalls();
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}


