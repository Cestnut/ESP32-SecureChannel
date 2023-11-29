#include <time.h>
#include <sys/time.h>
#include <inttypes.h>
#include <NTPClient.h>
#define JAN_2023_TIMESTAMP 1672531200

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