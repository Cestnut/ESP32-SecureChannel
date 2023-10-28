#include <MQTT.h>
#include <WiFi.h>

WiFiClient net;
MQTTClient mqtt_client;
const char* MQTT_broker_ip = "10.0.0.1";
int MQTT_broker_port = 1234;

void init_mqtt_client() {
  mqtt_client.begin(MQTT_broker_ip, MQTT_broker_port, net);

  Serial.print("\nconnecting...");
  while (!mqtt_client.connect("")) {
    Serial.println("Connecting to MQTT broker...");
    delay(2000);
  }

  Serial.println("\nConnected to MQTT broker!");
}