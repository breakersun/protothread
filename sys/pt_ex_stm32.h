#ifndef __PT_EX_STM32_H__
#define __PT_EX_STM32_H__

#include "pt.h"
#include "stm32l0xx_hal.h"

//=====================================================================
//=== additions for stm32 =======================================
//=====================================================================
// macro to return system time
#define PT_GET_TIME_msec() (HAL_GetTick())

// macro to make a thread execution pause in usec
// max time of about one hour
#define PT_YIELD_msec(delay_time)  \
    do { static unsigned int time_thread ;\
    time_thread = PT_GET_TIME_msec() + (unsigned int)delay_time ; \
    PT_YIELD_UNTIL(pt, (PT_GET_TIME_msec() >= time_thread)); \
    } while(0);

// macros for interval yield
// attempts to make interval equal to specified value
#define PT_INTERVAL_INIT() static unsigned int pt_interval_marker
#define PT_YIELD_INTERVAL(interval_time)  \
    do { \
    PT_YIELD_UNTIL(pt, (PT_GET_TIME_msec() >= pt_interval_marker)); \
    pt_interval_marker = PT_GET_TIME_msec() + (unsigned int)interval_time; \
    } while(0);

#endif
