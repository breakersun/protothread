#include "pt.h"
#include "pt-sem.h"
#include "pt_ex_win32.h"

#include <stdio.h>

static struct pt pt1, pt2;
static struct pt_sem detect_evt;

PT_INTERVAL_INIT();

static int main_thread(struct pt *pt)
{
  PT_BEGIN(pt);

  while (1) {
    PT_SEM_WAIT(pt, &detect_evt);
    printf("main thread: doing jobs\n");
  }

  PT_END(pt);
}

static int detect_thread(struct pt *pt)
{
  PT_BEGIN(pt);

  while (1) {
    PT_YIELD_INTERVAL(1000);
    printf("detect thread: something plugged!\n");
    PT_SEM_SIGNAL(pt, &detect_evt);
  }

  PT_END(pt);
}

int main(void)
{
  PT_INIT(&pt1);
  PT_INIT(&pt2);
  PT_SEM_INIT(&detect_evt, 10);

  while(1) {
    PT_SCHEDULE(main_thread(&pt1));
    PT_SCHEDULE(detect_thread(&pt2));
  }
}
