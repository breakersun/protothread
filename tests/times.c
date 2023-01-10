#include "times.h"

#ifdef _WIN32

static int clock_time(void)
{ return (int)GetTickCount(); }

#else /* _WIN32 */

static int clock_time(void)
{
  struct timeval tv;
  struct timezone tz;
  gettimeofday(&tv, &tz);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

#endif /* _WIN32 */

int timer_expired(struct timer *t)
{
  if (t->start)
    return (int)(clock_time() - t->start) >= (int)t->interval;
  else
    return 0;
}

void timer_set(struct timer *t, int interval)
{ t->interval = interval; t->start = clock_time(); }

void timer_stop(struct timer *t)
{ t->start = 0; }
