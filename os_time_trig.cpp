/*
 * os.c
 *
 *  Created on: 11 ����. 2013 �.
 *      Author: Andrei Bragarenco
 */

#include "os_time_trig.h"

volatile int8_t OSTT_Scheduler_flag = 0;

int8_t OSTT_GetSchedulerFlag(void){
	return OSTT_Scheduler_flag;
}

int8_t OSTT_SetSchedulerFlag(void){
	return OSTT_Scheduler_flag = 1;
}
int8_t OSTT_ResetSchedulerFlag(void){
	return OSTT_Scheduler_flag = 0;
}

OSTT_TaskDescriptor_t* OSTT_taskList[OSTT_MAX_NR_OF_TASKS];

int32_t OSTT_nrOfTasks   = 0;	//  nr of existing tasks
int32_t OSTT_currentTask = 0;    //  current executing task number


void OSTT_Scheduler(void) {



//#ifdef WATCHDOG_ENABLE
//	WatchdogReloadSet(WATCHDOG0_BASE, 0xFEEFEE);
//#endif
	for (OSTT_currentTask = 0; OSTT_currentTask < OSTT_nrOfTasks; OSTT_currentTask++) {

		// the startup offset is set as a timeout on startup
		// evaluate timeout and recurrence
		// also evaluate if the task is enable
		if (OSTT_TaskEvalState() == ENABLE && OSTT_TaskEvalTimeout() == TIME_FINISHED &&
			OSTT_EvalRecurrence()  == ENABLE)  {

			if (OSTT_taskList[OSTT_currentTask]->task)
				(*(OSTT_taskList[OSTT_currentTask]->task))();
		}

	}

}


Std_ReturnType OSTT_TaskInit( Std_ReturnType (*task)(void),  OSTT_TaskDescriptor_t* taskDescriptor  ){

	if(OSTT_nrOfTasks < OSTT_MAX_NR_OF_TASKS-1){
		taskDescriptor->task = task;
		taskDescriptor->timeout = taskDescriptor->startupTimeout;
		OSTT_taskList[OSTT_nrOfTasks++] = taskDescriptor;

	}
	else{
		return -1;
	}

	return OSTT_nrOfTasks-1;

}

/*
 * A default initialization template for tasks
 */

OSTT_TaskDescriptor_t OSTT_TaskDescriptorDefault(void){

	OSTT_TaskDescriptor_t taskDescriptor;
	taskDescriptor.task           = 0;
	taskDescriptor.recurrence     = 1;
	taskDescriptor.recurrenceCnt  = 0;
	taskDescriptor.timeout        = 0;
	taskDescriptor.startupTimeout = 0;
	taskDescriptor.priority        = 1;
	taskDescriptor.task_state     = ENABLE;

	return taskDescriptor;
}


/*
 * Evaluate recurrence of the task , to see if
 * we should let it execute in the current sys tick or no
 */
OSTT_FunctionalState OSTT_EvalRecurrence(void) {

	OSTT_FunctionalState result = DISABLE;
	if (OSTT_taskList[OSTT_currentTask]->recurrence) {

		if (++OSTT_taskList[OSTT_currentTask]->recurrenceCnt >= OSTT_taskList[OSTT_currentTask]->recurrence) {
			OSTT_taskList[OSTT_currentTask]->recurrenceCnt = 0;
			result = ENABLE;
		}

	}else{
		OSTT_taskList[OSTT_currentTask]->recurrenceCnt = 0;
	}
	return result;
}


void OSTT_TaskSetTimeout( timeout_t time){
	OSTT_taskList[OSTT_currentTask]->timeout = time;
}

timeout_t OSTT_TaskEvalTimeout(void){
	return OSTT_EvalTimeout(&(OSTT_taskList[OSTT_currentTask]->timeout));
}

OSTT_FunctionalState OSTT_TaskEvalState(void)
{
	return OSTT_taskList[OSTT_currentTask]->task_state;
}

timeout_t OSTT_EvalTimeout( timeout_t * time){
	if(*time>0)
		(*time)--;
	else
		(*time)=0;
	return *time;
}




