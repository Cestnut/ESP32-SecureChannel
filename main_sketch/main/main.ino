#include "mqtt_connection.h"
#include "wifi_connection.h"
#include "wireguard_connection.h"
#include "esp_random.h"
#include "ArduinoJson.h"
#include "utils.h"
#define UDP_PORT 5555
#define UDP_BUFF_SIZE 32

TaskHandle_t Task1;
TaskHandle_t Task2;

int min_number=-10, max_number=20;
char publish_topic[] = "/data/device1";
char command_topic[] = "/config/device1";
StaticJsonDocument<100> json_output;
StaticJsonDocument<100> json_input;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void setup() {
    Serial.begin(115200);

    init_wifi();
    configTime(2 * 60 * 60, 0, "pool.ntp.org", "time.google.com");
    timeClient.begin();
    timeClient.update();

    init_wireguard_interface();
    init_mqtt_client();

    mqtt_client.subscribe(command_topic);
    mqtt_client.onMessage(MQTTmessageReceived);

}

void MQTTmessageReceived(String &topic, String &payload) {  
  Serial.print("Received message ");
  Serial.print(payload);
  Serial.print(" on topic ");
  Serial.println(topic);

  DeserializationError error;
  error = deserializeJson(json_input, payload.c_str());
  if (error == DeserializationError::Ok){
      if(topic == command_topic){
        if(validate_settings(json_input["min"], json_input["max"])){
        update_settings(json_input["min"], json_input["max"]);
      }
    }
  }
}

void update_settings(const char *min, const char *max){
  min_number=atoi(min);
  max_number=atoi(max);
}

int validate_settings(const char *min, const char *max){
  return(is_number(min) && is_number(max));
}

int is_number(const char *string){
  for(int i=0; i<strlen(string) && string[i]!='\0'; i++){
    if(!isdigit(string[i])) return 0;
  }
  return 1;
}

void main_operation(){
  long long int number;
  char json_serialized[100];  
  number = random(min_number, max_number);
  json_output["number"] = number;
  json_output["timestamp"] = (uint64_t)timeClient.getEpochTime()*1000LL + (uint64_t)timeClient.get_millis();
  serializeJson(json_output, json_serialized);

  mqtt_client.publish(publish_topic, json_serialized);
  Serial.println(json_serialized);
}

void loop() {
  mqtt_client.loop();
  main_operation();
  if(!(mqtt_client.connected())){
    connect_mqtt();
  }
  
  improved_delay(1000, timeClient);
}
