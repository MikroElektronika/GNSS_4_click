/*

Use mikroE Timer Calculator to generate proper timer configuration
and timer ISR.

https://www.mikroe.com/timer-calculator

*/
#include "Click_GNSS4_types.h"

uint32_t timerCounter = 0;

static void gnss4_configTimer()
{


    // Configure Timer
}

void Timer_interrupt()
{
    gnss4_tick();
	timerCounter++;
    // Reset Flag
}