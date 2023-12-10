#include "mqtt_connection.h"
#include "wifi_connection.h"
#include "wireguard_connection.h"
#include "esp_random.h"
#include "ArduinoJson.h"
#include "utils.h"
#include <NTPClient.h>


int min_number=-10, max_number=20;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void setup() {
    Serial.begin(115200);
    
    init_wifi();
    //configTime(2 * 60 * 60, 0, "pool.ntp.org", "time.google.com");
    timeClient.begin();
    timeClient.update();

    init_wireguard_interface();
    init_mqtt_client();

    mqtt_client.subscribe(COMMAND_TOPIC);
    mqtt_client.onMessage(MQTTmessageReceived);

}

void MQTTmessageReceived(String &topic, String &payload) {  
  Serial.print("Received message ");
  Serial.print(payload);
  Serial.print(" on topic ");
  Serial.println(topic);

  StaticJsonDocument<100> json_input;
  DeserializationError error;
  error = deserializeJson(json_input, payload.c_str());
  if (error == DeserializationError::Ok){
    if(topic == COMMAND_TOPIC){
      if(json_input["command_type"] == "new_range"){
        if(validate_settings_range(json_input["min"], json_input["max"])){
        update_settings_range(json_input["min"], json_input["max"]);
        }
      }
    }
  }
}

void update_settings_range(const char *min, const char *max){
  min_number=atoi(min);
  max_number=atoi(max);
}

int validate_settings_range(const char *min, const char *max){
  return(is_number(min) && is_number(max));
}

int is_number(const char *string){
  for(int i=0; i<strlen(string) && string[i]!='\0'; i++){
    if(!isdigit(string[i])) return 0;
  }
  return 1;
}

void main_operation(int array_size){
  long number=0;
  StaticJsonDocument<200> json_output;
  char json_serialized[200];
  json_output["timestamp"] = (uint64_t)timeClient.getEpochTime()*1000LL + (uint64_t)timeClient.get_millis();
  json_output["type"] = "random_numbers";
  JsonArray numbers = json_output.createNestedArray("numbers");
  for(int i=0; i<array_size;i++){
    number = random(min_number, max_number);
    numbers.add(number);
  }
  
  serializeJson(json_output, json_serialized);

  mqtt_client.publish(PUBLISH_TOPIC, json_serialized);
  Serial.println(json_serialized);
}

void loop() {
  for(int i=0;i<101;i++){ 
    main_operation(10);
    mqtt_client.loop();
    if(!(mqtt_client.connected())){
      connect_mqtt();
    }
    delay(50);  
  }
  
  delay(10000); 
}