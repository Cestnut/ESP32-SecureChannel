#include <MQTT.h>
#include <WiFi.h>
#include "credentials.h"

WiFiClient net;
MQTTClient mqtt_client;

void connect_mqtt(){
  while (!mqtt_client.connect("", MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.println("Connecting to MQTT broker...");
    delay(2000);
  }

  Serial.println("\nConnected to MQTT broker!");
}

void init_mqtt_client() {
  mqtt_client.begin(MQTT_BROKER_IP, MQTT_BROKER_PORT, net);
  connect_mqtt();
}