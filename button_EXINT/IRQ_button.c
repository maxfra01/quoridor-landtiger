#include "button.h"
#include "lpc17xx.h"
#include "../GLCD/GLCD.h"
#include "../game.h"

extern int down_0, down_1, down_2;
extern int a_remaining_walls, b_remaining_walls, player_turn,  tmp_wall_orient;

void EINT0_IRQHandler (void)	  	/* INT0														 */
{		
	down_0=1;
	NVIC_DisableIRQ(EINT0_IRQn);
	NVIC_DisableIRQ(EINT1_IRQn);
	NVIC_DisableIRQ(EINT2_IRQn);
	LPC_PINCON->PINSEL4    &= ~(1 << 20);     /* GPIO pin selection */
	
	
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	down_1=1;
	NVIC_DisableIRQ(EINT1_IRQn);
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	down_2 = 1;
	NVIC_DisableIRQ(EINT2_IRQn);
	LPC_PINCON->PINSEL4    &= ~(1 << 24);     /* GPIO pin selection */
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}


