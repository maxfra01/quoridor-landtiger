/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include "../game.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
volatile int seconds=20;
volatile char first_char[2];
extern volatile int player_turn;
extern volatile int a_remaining_walls, b_remaining_walls;

void TIMER0_IRQHandler (void)
{
	seconds--;
	sprintf(first_char, "Remaining time: %02d", (seconds));
	GUI_Text(5,300,  first_char, White, Black);
	//GUI_Text(120,300,  &second_char, Black, White);
	
	
	if(seconds==0){
		changeActivePlayer();
		GUI_Text(5,300, "Remaining time: 20", White, Black);
		if (player_turn==1){
			GUI_Text(5,240, (uint8_t *)  "Turn A", White, Black);
		}
		else{
			GUI_Text(5,240, (uint8_t *) "Turn B", White, Black);
		}
		seconds=20;
	}
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
