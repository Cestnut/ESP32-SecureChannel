#include "mqtt_connection.h"
#include "wifi_connection.h"
#include "wireguard_connection.h"
#include "esp_random.h"
#include "ctype.h"
#include "ArduinoJson.h"
#define UDP_PORT 5555
#define UDP_BUFF_SIZE 32

TaskHandle_t Task1;
TaskHandle_t Task2;

int min_number=-10, max_number=20;
char publish_topic[] = "/data/device1";
StaticJsonDocument<100> json_output;
StaticJsonDocument<100> json_input;

void setup() {
    Serial.begin(115200);

    init_wifi();
    init_wireguard_interface();
    init_mqtt_client();

    mqtt_client.subscribe("/config");
    mqtt_client.onMessage(MQTTmessageReceived);
}

void MQTTmessageReceived(String &topic, String &payload) {
  char char_array[payload.length() + 1];
  strcpy(char_array, payload.c_str());
  
  Serial.print("Received message ");
  Serial.print(payload);
  Serial.print(" on topic ");
  Serial.println(topic);

  if(topic == "/config"){
    parse_settings(char_array);
  }
}

int is_number(char *string, uint32_t size){
  for(int i=0; i<size && string[i]!='\0'; i++){
    if(!isdigit(string[i])) return 0;
  }
  return 1;
}

int parse_settings(char *command){
  char *token;
  int tmp_min_number, tmp_max_number;
  token = strtok(command, " ");
  if (token == NULL) return -1;
  if (!is_number(token, strlen(token))) return -1;
  tmp_min_number = atoi(token);
  
  token = strtok(NULL, " ");
  if (token == NULL) return -1;
  if (!is_number(token, strlen(token))) return -1;
  tmp_max_number = atoi(token);

  min_number=tmp_min_number;
  max_number=tmp_max_number;
  
  return 0;
}

void main_operation(){
  long long int number;
  char json_serialized[100];  
  number = random(min_number, max_number);
  json_output["number"] = number;
  serializeJson(json_output, json_serialized);

  mqtt_client.publish(publish_topic, json_serialized);
  Serial.println(json_serialized);
  delay(4000);
}

void loop() {
  mqtt_client.loop();
  main_operation();
}
