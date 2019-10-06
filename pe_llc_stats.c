
/* 
 * perf_examples/pe_llc_stats.c
 *
 * -- example program to get stats of llc
 *
 * PERF_FORMAT_TOTAL_TIME_ENABLED = disabled
 * PERF_FORMAT_TOTAL_TIME_RUNNING = disabled
 * PERF_FORMAT_ID = enabled
 * PERF_FORMAT_GROUP = enabled
 *
 * author: Srikanth Yalavarthi
 *
 */

#include "pe.h"

struct read_format {
  uint64_t nr;
  struct {
    uint64_t value;
    uint64_t id;
  } values[];
};

int main( void )
{

  /* perf variables */
  struct perf_event_attr pe_attr;
  int fd_cm, fd_cr;
  uint64_t id_cm, id_cr;
  uint64_t value_cm, value_cr;
  struct read_format *rf;
  char buffer[(1 + 2 * 2) * 8];
  rf = (struct read_format*) buffer;
  int i;

  /* llc cache misses */
  /* define and enable */
  memset(&pe_attr, 0, sizeof(pe_attr));
  pe_attr.type= PERF_TYPE_HARDWARE;
  pe_attr.size = sizeof(struct perf_event_attr);
  pe_attr.config = PERF_COUNT_HW_CACHE_MISSES;
  pe_attr.disabled = 1;
  pe_attr.exclude_kernel = 1;
  pe_attr.exclude_hv = 1;
  pe_attr.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;

  /* open perf event */
  fd_cm = perf_event_open(&pe_attr, 0, -1, -1, 0);
  if (fd_cm == -1)
  {
    fprintf(stderr, "error opening leader %llu\n", pe_attr.config);
    exit (EXIT_FAILURE);
  }

  /* get id_cm */
  ioctl(fd_cm, PERF_EVENT_IOC_ID, &id_cm);

  /* llc cache references */
  /* define and enable */
  memset(&pe_attr, 0, sizeof(pe_attr));
  pe_attr.type= PERF_TYPE_HARDWARE;
  pe_attr.size = sizeof(struct perf_event_attr);
  pe_attr.config = PERF_COUNT_HW_CACHE_REFERENCES;
  pe_attr.disabled = 1;
  pe_attr.exclude_kernel = 1;
  pe_attr.exclude_hv = 1;
  pe_attr.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;

  /* open perf event */
  fd_cr = perf_event_open(&pe_attr, 0, -1, fd_cm, 0);

  /* get id_cr */
  ioctl(fd_cr, PERF_EVENT_IOC_ID, &id_cr);

  /* reset all counters */
  ioctl(fd_cm, PERF_EVENT_IOC_RESET, 0);
  ioctl(fd_cr, PERF_EVENT_IOC_RESET, 0);

  /* enable all event */
  ioctl(fd_cm, PERF_EVENT_IOC_ENABLE, 0);
  ioctl(fd_cr, PERF_EVENT_IOC_ENABLE, 0);

  /* do some work */
  compute_load();

  /* disable all event */
  ioctl(fd_cm, PERF_EVENT_IOC_DISABLE, 0);
  ioctl(fd_cr, PERF_EVENT_IOC_DISABLE, 0);

  /* read results */
  read(fd_cm, buffer, sizeof(buffer));

  for (i = 0; i < (int)rf->nr; i++)
  {
    if (rf->values[i].id == id_cm)
    {
      value_cm = rf->values[i].value;
    }
    else if (rf->values[i].id == id_cr)
    {
      value_cr = rf->values[i].value;
    }
  }

  /* print result */
  fprintf(stdout, "Cache Misses     = %lu\n", value_cm);
  fprintf(stdout, "Cache References = %lu\n", value_cr);
  fprintf(stdout, "Cache Hit Ratio  = %lf\n",  1.0 - ((double)value_cm/value_cr));

  return EXIT_SUCCESS;
}
