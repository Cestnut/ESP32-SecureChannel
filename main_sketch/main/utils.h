#include <time.h>

unsigned long get_unix_epoch() {
  time_t now;
  time(&now);
  return now;
}