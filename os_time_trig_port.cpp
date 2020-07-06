/*
 * os_time_trig_port.c
 *
 *  Created on: Jun 9, 2020
 *      Author: Andrei Bragarenco
 */

#include "os_time_trig.h"


#ifdef ARDUINO_ARCH_AVR

#include "timer-api.h"

void OSTT_Init(void)
{

	timer_init_ISR_1KHz(TIMER_DEFAULT);
}

void timer_handle_interrupts(int timer)
{
	//timer;
	OSTT_Scheduler();
}
#else

#include "Arduino.h"

hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;




void IRAM_ATTR OSTT_onTimer()
{
	portENTER_CRITICAL_ISR(&timerMux);
	OSTT_SetSchedulerFlag();
	portEXIT_CRITICAL_ISR(&timerMux);
}

void  OSTT_Init()
{
	timer = timerBegin(0, 80, true); // prescaller at cpu 80 Mhz -> 1 Mhz
	timerAttachInterrupt(timer, &OSTT_onTimer, true);
	timerAlarmWrite(timer, 1000, true); // 1ms
	timerAlarmEnable(timer);
}

#endif
