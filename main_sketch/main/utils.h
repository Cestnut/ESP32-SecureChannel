#include <time.h>
#include <sys/time.h>
#include <inttypes.h>
#include <NTPClient.h>
#define JAN_2023_TIMESTAMP 1672531200

void improved_delay(int msecs, NTPClient timeClient){
  uint64_t begin, end;
  begin = (uint64_t)timeClient.getEpochTime()*1000LL + (uint64_t)timeClient.get_millis();
  while((uint64_t)timeClient.getEpochTime()*1000LL + (uint64_t)timeClient.get_millis() - begin < msecs);
}

unsigned long get_unix_epoch() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return 0;
  }
  now = time(NULL);
  return now;
}

void syncTime(){
  int i = 0;
  while(get_unix_epoch() < JAN_2023_TIMESTAMP){
    delay(1000);
    i++;
  }
  Serial.print("Time synced in ");
  Serial.print(i);
  Serial.println(" seconds.");
}

uint64_t  xx_time_get_time() {
	struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
	gettimeofday(&tv, NULL);
  int64_t time_us = (int64_t)tv.tv_sec * 1000000L + (int64_t)tv.tv_usec;
  printf("time_us %" PRId64 "\n", time_us);
	uint64_t temp = tv.tv_sec * 1000LL + (tv.tv_usec / 1000LL);
  Serial.println((unsigned long)tv.tv_sec*1000LL + (unsigned long)tv.tv_usec / 1000LL);
  printf("TimeVal-sec  = %"PRIdMAX"\n", (long long) tv.tv_sec);
  printf("TimeVal-sec  = %"PRIdMAX"\n", (long) tv.tv_sec);
  printf("TimeVal-sec  = %ld\n", (long) tv.tv_sec);
  printf("Size of tv.tv_sec: %d\n", sizeof(tv.tv_sec));
  printf("Size of time_t: %d\n", sizeof(time_t));
  printf("Size of suseconds_t: %d\n", sizeof(suseconds_t));

  printf("TimeVal-usec  = %"PRIdMAX"\n", (long long) tv.tv_usec);
  printf("TimeVal-usec  = %"PRIdMAX"\n", (long) tv.tv_usec);
  printf("TimeVal-usec  = %ld\n", (long) tv.tv_usec);
    printf("tv_now.tv_sec %" PRId64 "\n", (int64_t)tv.tv_usec);

  return tv.tv_sec;
}