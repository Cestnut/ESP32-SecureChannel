#include <MQTT.h>
#include <WiFi.h>
#include "credentials.h"

WiFiClient net;
MQTTClient mqtt_client;

void init_mqtt_client() {
  mqtt_client.begin(MQTT_BROKER_IP, MQTT_BROKER_PORT, net);

  Serial.print("\nconnecting...");
  while (!mqtt_client.connect("")) {
    Serial.println("Connecting to MQTT broker...");
    delay(2000);
  }

  Serial.println("\nConnected to MQTT broker!");
}