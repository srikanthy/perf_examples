
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <linux/perf_event.h>
#include <unistd.h>
#include <asm/unistd.h>
#include <sys/ioctl.h>

/* wrapper fucntion for perf_event_open */
int perf_event_open( struct perf_event_attr *attr, pid_t pid, int cpu, int group_fd, unsigned long flags)
{

  int fd;

  fd = syscall( __NR_perf_event_open, attr, pid, cpu, group_fd, flags );

  return fd;

}

/* compute load - simple compute load*/
void compute_load( void )
{

  int N = 1048576;
  float A[N];
  int i;

  for (i = 0; i < N; i++)
  {
    A[i]++;
  }

}
