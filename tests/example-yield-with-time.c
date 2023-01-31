
#include "pt.h"
#include "pt_ex_win32.h"

#include <stdio.h> /* For printf(). */

static int protothread1_flag, protothread2_flag;

PT_INTERVAL_INIT();

static int
protothread1(struct pt *pt)
{
  PT_BEGIN(pt);
    PT_YIELD_INTERVAL(1000);
    printf("thread1\n");
  PT_END(pt);
}

static int protothread2(struct pt *pt)
{
  PT_BEGIN(pt);

    PT_YIELD_usec(500);
    printf("look I am thread2\n");
  PT_END(pt);
}

static struct pt pt1, pt2;

int main(void)
{
  PT_INIT(&pt1);
  PT_INIT(&pt2);

  while(1) {
    protothread1(&pt1);
    protothread2(&pt2);
  }
}
