
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif
#include <stdio.h>

#include "pt-sem.h"
#include "pt.h"

#include <stdint.h>
#include "hsm.h"
#include "demo_process.h"
#include "times.h"

//! Instance of process_t
process_t SampleProcess;

//! Create and initialize the array of state machines.
state_machine_t * const State_Machines[] = {(state_machine_t *)&SampleProcess};
//! Callback function to log the events dispatched by state machine framework.
void event_logger(uint32_t stateMachine, uint32_t state, uint32_t event)
{
  printf("State Machine: %d, State: %d, Event: %d\n", stateMachine, state, event);
}

//! Callback function to log the result of event processed by state machine
void result_logger(uint32_t state, state_machine_result_t result)
{
  printf("Result: %d, New State: %d\n", result, state);
}

/*
 * This example uses two timers: one for the code lock protothread and
 * one for the simulated key input protothread.
 */
struct timer input_timer;
/*---------------------------------------------------------------------------*/
/*
 * This example has two protothread and therefor has two protothread
 * control structures of type struct pt. These are initialized with
 * PT_INIT() in the main() function below.
 */
static struct pt timer_pt, statemachine_pt;
static struct pt_sem timer_expired_sema;

static PT_THREAD(timer_thread(struct pt *pt))
{
  PT_BEGIN(pt);
  // block here for timer expired
  PT_WAIT_UNTIL(pt, timer_expired(&input_timer));
  // timer is one time, so we stoped it here
  timer_stop(&input_timer);

  on_process_timedout(&SampleProcess);
  PT_SEM_SIGNAL(pt, &timer_expired_sema);

  PT_END(pt);
}

static PT_THREAD(statemachine_thread(struct pt *pt))
{
  PT_BEGIN(pt);
  PT_SEM_WAIT(pt, &timer_expired_sema);
  if(dispatch_event(State_Machines, 1, event_logger, result_logger) == EVENT_UN_HANDLED)
  {
    printf("invalid event entered\n");
  }
  PT_END(pt);
}

int main(void)
{
  init_process(&SampleProcess, 10);

  PT_INIT(&statemachine_pt);
  PT_INIT(&timer_pt);

  PT_SEM_INIT(&timer_expired_sema, 1);
  /*
   * Schedule the two protothreads until the timer_thread() exits.
   */
  while(1) {
    PT_SCHEDULE(statemachine_thread(&statemachine_pt));
    PT_SCHEDULE(timer_thread(&timer_pt));

    /*
     * When running this example on a multitasking system, we must
     * give other processes a chance to run too and therefore we call
     * usleep() resp. Sleep() here. On a dedicated embedded system,
     * we usually do not need to do this.
     */
#ifdef _WIN32
    Sleep(0);
#else
    usleep(10);
#endif
  }

  return 0;
}
/*---------------------------------------------------------------------------*/
/*
 * Finally, the implementation of the simple timer library follows.
 */
/*---------------------------------------------------------------------------*/
