
/* 
 * perf_examples/pe_llc_stats.c
 *
 * -- example program to get stats of llc
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
  int fd_cm, fd_cr;
  uint64_t value_cm, value_cr;

  /* llc cache misses */
  /* define and enable */
  memset(&pe_attr, 0, sizeof(pe_attr));
  pe_attr.type= PERF_TYPE_HARDWARE;
  pe_attr.size = sizeof(struct perf_event_attr);
  pe_attr.config = PERF_COUNT_HW_CACHE_MISSES;
  pe_attr.disabled = 1;
  pe_attr.exclude_kernel = 1;
  pe_attr.exclude_hv = 1;

  /* open perf event */
  fd_cm = perf_event_open(&pe_attr, 0, -1, -1, 0);
  if (fd_cm == -1)
  {
    fprintf(stderr, "error opening leader %llu\n", pe_attr.config);
    exit (EXIT_FAILURE);
  }

  /* llc cache references */
  /* define and enable */
  memset(&pe_attr, 0, sizeof(pe_attr));
  pe_attr.type= PERF_TYPE_HARDWARE;
  pe_attr.size = sizeof(struct perf_event_attr);
  pe_attr.config = PERF_COUNT_HW_CACHE_REFERENCES;
  pe_attr.disabled = 1;
  pe_attr.exclude_kernel = 1;
  pe_attr.exclude_hv = 1;

  /* open perf event */
  fd_cr = perf_event_open(&pe_attr, 0, -1, -1, 0);
  if (fd_cr == -1)
  {
    fprintf(stderr, "error opening leader %llu\n", pe_attr.config);
    exit (EXIT_FAILURE);
  }

  /* reset counters */
  ioctl(fd_cm, PERF_EVENT_IOC_RESET, 0);
  ioctl(fd_cr, PERF_EVENT_IOC_RESET, 0);

  /* enable event */
  ioctl(fd_cm, PERF_EVENT_IOC_ENABLE, 0);
  ioctl(fd_cr, PERF_EVENT_IOC_ENABLE, 0);

  /* do some work */
  compute_load();

  /* disable the event */
  ioctl(fd_cm, PERF_EVENT_IOC_DISABLE, 0);
  ioctl(fd_cr, PERF_EVENT_IOC_DISABLE, 0);

  /* read results */
  read(fd_cm, &value_cm, sizeof(value_cm));
  read(fd_cr, &value_cr, sizeof(value_cr));

  /* print result */
  fprintf(stdout, "Cache Misses     = %lu\n", value_cm);
  fprintf(stdout, "Cache References = %lu\n", value_cr);
  fprintf(stdout, "Cache Hit Ratio  = %lf\n",  1.0 - ((double)value_cm/value_cr));

  return EXIT_SUCCESS;
}
