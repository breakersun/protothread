# protothread
pull out protothread from latest contiki-ng

## Protothread Function Summary

The following table give the syntax for the thread macros and functions written by Adam Dunkels.

| Protothread Functions(from Adam Dunkels) | Description    |
| -------------- | -------------- |
| PT_INIT(pt) | Initialize a protothread. This macro inits a protothread, but do not use it directly. The pt_add() macro below will do this for you. |
| PT_THREAD(name_args) | Declaration of a protothread. name_args The name and arguments of the C function implementing the protothread.
| PT_WAIT_THREAD(pt, thread) | Wait until a child protothread completes.<br>pt A pointer to the protothread control structure.<br>thread The child protothread with arguments
| PT_WAIT_UNTIL(pt, condition) <br>PT_WAIT_WHILE(pt, condition) | Wait the thread and wait until/while condition is true. Conditions which may be set on either core are not safe.
| PT_BEGIN(pt) | Declare the start of a protothread inside the C function implementing the protothread.<br>pt A pointer to the protothread control structure.
| PT_END(pt) | Declare the end of a protothread. <br>pt A pointer to the protothread control structure.
| PT_EXIT(pt) | This macro causes the protothread to exit. If the protothread was spawned by another protothread, the parent protothread will become unblocked and can continue to run. <br>pt A pointer to the protothread control structure.
| PT_RESTART(pt) | This macro will block and cause the running protothread to restart its execution at the place of the PT_BEGIN()call. pt A pointer to the protothread control structure.
| PT_SCHEDULE(f) | This macro shedules a protothread, but do not use it directly!. The scheduler will do this for you! The return value of the function is non-zero if the protothread is running or zero if the protothread has exited. f The call to the C function implementing the protothread to be scheduled. Note that you will proably never use this macro.
| PT_SPAWN(pt, child, thread) | Spawn a child protothread (example) and block the thread until it exits. Parameters: pt A pointer to the current protothread control structure. child A pointer to the child protothread’s control structure. thread The child protothread with arguments
| PT_YIELD(pt) | yield the protothread, thereby allowing other processing to take place in the system.
| PT_YIELD_UNTIL(pt, cond) | Yield from the protothread until a condition occurs. Conditions which may be set on either core are not safe.
| PT_SEM_INIT(s, c) | This macro initializes a semaphore with a value for the counter. Internally, the semaphores use an "unsigned int" to represent the counter, and therefore the "count" argument should be within range of an unsigned int. s A pointer to the pt_sem struct representing the semaphore. Only use these semaphores for signals between threads running on one core!
| PT_SEM_SIGNAL(pt, s) | This macro carries out the "signal" operation on the semaphore. The signal operation increments the counter inside the semaphore, which eventually will cause waiting protothreads to continue executing. pt A pointer to the protothread (struct pt) in which the operation is executed. s A pointer to the pt_sem struct representing the semaphore. Only use these semaphores for signals between threads running on one core!
| PT_SEM_WAIT(pt, s) | This macro carries out the "wait" operation on the semaphore. The wait operation causes the protothread to yield while the counter is zero. When the counter reaches a value larger than zero, the protothread will continue. Only use these semaphores for signals between threads running on one core!

The following table has the protothread macro extensions and functions

| Added Protothread Functions  | Description   |
|-------------- | -------------- |
| pt_queue(T, size) | typedef pt_queue(uint8_t, 4096) byte_queue_t; |
| pt_queue_init() | byte_queue_t queue = pt_queue_init();     |
| pt_queue_empty(q) | | 
| pt_queue_full(q) | | 
| pt_queue_push(q, el) | |
| pt_queue_peek(q) | |
| pt_queue_pop(q) | |
| pt_queue_reset(q) | |
