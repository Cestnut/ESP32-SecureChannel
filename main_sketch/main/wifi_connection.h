#include "WiFi.h"
#include "WiFiUdp.h"
#include "WireGuard-ESP32.h"
#include "esp_random.h"
#include "ctype.h"
#include "credentials.h"
#include "config.h"

void init_wifi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.print("\nConnected to WiFi! IP Address:");
  Serial.println(WiFi.localIP());    
}