#include "button.h"
#include "lpc17xx.h"
#include "../GLCD/GLCD.h"
#include "../game.h"

extern int down;

void EINT0_IRQHandler (void)	  	/* INT0														 */
{		
	NVIC_DisableIRQ(EINT0_IRQn);
	NVIC_DisableIRQ(EINT1_IRQn);
	NVIC_DisableIRQ(EINT2_IRQn);
	gameInit();
	NVIC_EnableIRQ(EINT1_IRQn);
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{

	switchMode();
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	rotateWall();
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}


