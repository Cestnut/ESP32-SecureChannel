#include "WiFi.h"
#include "WiFiUdp.h"
#include "WireGuard-ESP32.h"
#include "esp_random.h"
#include "ctype.h"
#include "credentials.h"

#define HOTSPOT

class Custom_WiFiUDP : public WiFiUDP {
  public:
      int read_line(char* buffer, size_t len){
        int received_bytes=0, tmp_received_bytes;
        while(received_bytes < len){
          //Read 1 byte at a time
          tmp_received_bytes = WiFiUDP :: read(buffer+received_bytes, 1);
          //Calling function has to handle errors
          if(tmp_received_bytes < 1) return tmp_received_bytes;
          else{
            if(buffer[received_bytes] == '\n'){
                buffer[received_bytes] = '\0';
                received_bytes += tmp_received_bytes;
                break;
            }
            else{
                received_bytes += tmp_received_bytes;
            }
          }
        }
    return received_bytes;
    }
};

#if defined(UNI)
const char* ssid = "UNIME-WIFI";
const char* anon_identity = "";
const char* identity = UNI_IDENTITY;
const char* password = "";
const char* test_root_ca = "";
#elif defined(HOTSPOT)
const char* ssid = HOTSPOT_SSID;
const char* password = HOTSPOT_PASSWORD;
#endif

void init_wifi(){
  #if defined(UNI)
  WiFi.begin(ssid, WPA2_AUTH_PEAP, anon_identity, identity, password, test_root_ca);
  #elif defined(HOTSPOT)
  WiFi.begin(ssid, password);
  #endif

  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.print("\nConnected to WiFi! IP Address:");
  Serial.println(WiFi.localIP());    
}