
/* 
 * perf_examples/pe_hw_cache_misses.c
 *
 * -- example program to count the last level cache misses
 *
 * PERF_FORMAT_TOTAL_TIME_ENABLED = disabled
 * PERF_FORMAT_TOTAL_TIME_RUNNING = disabled
 * PERF_FORMAT_ID = disabled
 * PERF_FORMAT_GROUP = disabled
 *
 * author: Srikanth Yalavarthi
 *
 */

#include "pe.h"

int main( void )
{

  /* perf variables */
  struct perf_event_attr pe_attr;
  int fd;
  uint64_t value;

  /* define perf event */
  memset(&pe_attr, 0, sizeof(pe_attr));
  pe_attr.type= PERF_TYPE_HARDWARE;
  pe_attr.size = sizeof(struct perf_event_attr);
  pe_attr.config = PERF_COUNT_HW_CACHE_MISSES;
  pe_attr.disabled = 1;
  pe_attr.exclude_kernel = 1;
  pe_attr.exclude_hv = 1;

  /* open perf event */
  fd = perf_event_open(&pe_attr, 0, -1, -1, 0);
  if (fd == -1)
  {
    fprintf(stderr, "error opening leader %llu\n", pe_attr.config);
    exit (EXIT_FAILURE);
  }

  /* reset counters and enable event */
  ioctl(fd, PERF_EVENT_IOC_RESET, 0);
  ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);

  /* do some work */
  compute_load();

  /* disable the event */
  ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);

  /* read results */
  read(fd, &value, sizeof(value));

  /* print result */
  fprintf(stdout, "Cache Misses = %lu\n", value);

  return EXIT_SUCCESS;
}
