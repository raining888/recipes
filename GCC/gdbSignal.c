#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig);

void handler(int sig)
{
  signal(sig, handler);
  printf("Receive signal: %d\n", sig);
}

int main(void) {
  signal(SIGHUP, handler); // CTRl + c 触发，gdb用catch signal捕获

  while (1)
  {
    sleep(1);
  }
  return 0;
}
