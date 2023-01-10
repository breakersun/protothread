#include <stdint.h>
#include <stdio.h>

#include "hsm.h"
#include "demo_process.h"

#include "times.h"

/*
 *  --------------------- ENUMERATION ---------------------
 */

//! List of states the process state machine
typedef enum
{
  DISCHARGE_STATE, DETECT_STATE,
}process_state_t;

/*
 *  --------------------- Function prototype ---------------------
 */

extern struct timer input_timer;

static state_machine_result_t discharge_handler(state_machine_t* const State);
static state_machine_result_t discharge_entry_handler(state_machine_t* const State);
static state_machine_result_t discharge_exit_handler(state_machine_t* const State);
static state_machine_result_t detect_handler(state_machine_t* const State);
static state_machine_result_t detect_entry_handler(state_machine_t* const State);
static state_machine_result_t detect_exit_handler(state_machine_t* const State);
/*
 *  --------------------- Global variables ---------------------
 */

static const state_t Process_States[] =
{
  [DISCHARGE_STATE] = {
    .Handler = discharge_handler,
    .Entry   = discharge_entry_handler,
    .Exit    = discharge_exit_handler,
    .Id      = DISCHARGE_STATE,
  },

  [DETECT_STATE] = {
    .Handler = detect_handler,
    .Entry   = detect_entry_handler,
    .Exit    = detect_exit_handler,
    .Id      = DETECT_STATE,
  },
};

/*
 *  --------------------- Functions ---------------------
 */

void init_process(process_t* const pProcess, uint32_t processTime)
{
  pProcess->Machine.State = &Process_States[DISCHARGE_STATE];
  pProcess->Machine.Event = 0;
  pProcess->Set_Time = processTime;
  pProcess->Resume_Time = 0;

  discharge_entry_handler((state_machine_t *)pProcess);
}

static state_machine_result_t discharge_entry_handler(state_machine_t* const pState)
{
  process_t* const pProcess = (process_t*)pState;
  pProcess->Timer = pProcess->Set_Time;
  timer_set(&input_timer, 5000);

  printf("+++++++++++++++++Discharging: Close VSNS; Open DISCHARGE\n");
  return EVENT_HANDLED;
}

static state_machine_result_t discharge_handler(state_machine_t* const pState)
{
  switch(pState->Event)
  {
  case TIMEOUT:
    printf("timeout received\r\n");
    return switch_state(pState, &Process_States[DETECT_STATE]);

  default:
    return EVENT_UN_HANDLED;
  }
  return EVENT_HANDLED;
}

static state_machine_result_t discharge_exit_handler(state_machine_t* const pState)
{
  process_t* const pProcess = (process_t*)pState;
  pProcess->Timer = 0;
  timer_stop(&input_timer);
  printf("+++++++++++++++++Discharging: Done, Close DISCHARGE\n");
  return EVENT_HANDLED;
}

static state_machine_result_t detect_entry_handler(state_machine_t* const pState)
{
  process_t* const pProcess = (process_t*)pState;
  pProcess->Timer = 2;
  timer_set(&input_timer, 2000);
  printf("+++++++++++++++++Detecting: preparing: open VSNS\n");
  return EVENT_HANDLED;
}

static state_machine_result_t detect_handler(state_machine_t* const pState)
{
  switch(pState->Event)
  {
  case TIMEOUT:
    printf("+++++++++++++++++Detecting: Reading GPIO and decide next move\n");
    return switch_state(pState, &Process_States[DISCHARGE_STATE]);

  default:
    return EVENT_UN_HANDLED;
  }
  return EVENT_HANDLED;
}

static state_machine_result_t detect_exit_handler(state_machine_t* const pState)
{
  process_t* const pProcess = (process_t*)pState;
  printf("+++++++++++++++++Detecting: Done, Close VSNS\n");
  pProcess->Timer = 0;
  timer_stop(&input_timer);
  return EVENT_HANDLED;
}
