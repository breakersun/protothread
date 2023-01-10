
#include <windows.h>


struct timer { int start, interval; };
int  timer_expired(struct timer *t);
void timer_set(struct timer *t, int usecs);
void timer_stop(struct timer *t);
