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
volatile char first_char, second_char;
extern int player_turn;
extern int a_remaining_walls, b_remaining_walls, tmp_wall_i, tmp_wall_j, tmp_wall_orient;

void TIMER0_IRQHandler (void)
{
	seconds--;
	first_char = (int) seconds/10 +'0';
	second_char = seconds%10 + '0';
	GUI_Text(135, 300, &first_char, White, Black);
	//GUI_Text(150, 300, &second_char, White, Black);
	
	
	if(seconds==0){
		LCD_DrawWall(tmp_wall_j*30, tmp_wall_i*30, Blue, tmp_wall_orient);
		cleanMoves();
		changeActivePlayer();
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
