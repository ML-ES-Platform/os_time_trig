

#ifndef _OS_TIME_TRIG_H_
#define _OS_TIME_TRIG_H_

#include <stdint.h>
//#include "mcal_config.h"
#include "platform_config.h"

#ifndef MAX_NR_OF_TASKS
#define OSTT_MAX_NR_OF_TASKS 20
#endif

#ifndef TIME_FINISHED
#define TIME_FINISHED 0

#endif // _OS_TIME_TRIG_H_




///****************************************************************************
//	** Operating system execution control
//	*/
//
//	/* Type and constant definitions */
//	typedef U32 AppModeType;
//
//	extern const AppModeType OSDEFAULTAPPMODE;
//
//	/* Constructional Elements */
//
//	/* System Services */
//	AppModeType GetActiveApplicationMode( void );
//	void StartOS( AppModeType Mode );
//	void ShutdownOS( StatusType error );
//
//
//	/* System Services */
//	void ErrorHook( StatusType error );
//	void PreTaskHook( void );
//	void PostTaskHook( void );
//	void StartupHook( void );
//	void ShutdownHook( StatusType error );
//



typedef enum {RESET = 0, SET = !RESET}        OSTT_FlagStatus, OSTT_ITStatus;
typedef enum {DISABLE = 0, ENABLE = !DISABLE} OSTT_FunctionalState;
typedef enum {ERROR = 0, SUCCESS = !ERROR}    OSTT_ErrorStatus;

#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef unsigned char OSTT_StatusType;
#define OS_E_OK 0


#define TASKNAME(TaskName)      _##TaskName##_func
#define TASK(TaskName)          OSTT_StatusType TASKNAME(TaskName)(void)


typedef int64_t timeout_t;

typedef struct OSTT_TaskDescriptor{
	int32_t recurrence;
	int32_t recurrenceCnt;
	int32_t startupTimeout;
	timeout_t timeout;
	int32_t priority;
	Std_ReturnType (*task) ();
	OSTT_FunctionalState task_state;
} OSTT_TaskDescriptor_t;

OSTT_TaskDescriptor_t OSTT_TaskDescriptorDefault(void);
Std_ReturnType OSTT_TaskInit( Std_ReturnType (*task)(void), OSTT_TaskDescriptor_t* taskDescriptor );

OSTT_FunctionalState OSTT_EvalRecurrence(void);

void OSTT_TaskSetTimeout( timeout_t time);
timeout_t  OSTT_EvalTimeout( timeout_t * time);
timeout_t  OSTT_TaskEvalTimeout(void);
OSTT_FunctionalState  OSTT_TaskEvalState(void);

void  OSTT_Scheduler(void);
void  OSTT_Init(void);


int8_t OSTT_GetSchedulerFlag(void);
int8_t OSTT_SetSchedulerFlag(void);
int8_t OSTT_ResetSchedulerFlag(void);

#endif /* _OS_TIME_TRIG_H_ */

