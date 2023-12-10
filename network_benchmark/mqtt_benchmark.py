import json
import paho.mqtt.client as mqtt
import time
import math
import sys

batch_sizes = [1,5,10,50,100]
max_messages_number = batch_sizes[-1]+1

def reset_globals():
    global received_messages_number
    global first_recv_timestamp
    global times_list
    
    received_messages_number = 0
    first_recv_timestamp = 0
    times_list = list()

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("/data/device1")

def on_message(client, userdata, msg):
    recv_timestamp = math.floor(time.time()*1000)
    
    data = json.loads(msg.payload)
    send_timestamp = data["timestamp"]

    global received_messages_number
    global first_recv_timestamp
    global max_messages_number
    global batch_sizes

    if received_messages_number == 0:
        first_recv_timestamp = recv_timestamp

    received_messages_number = received_messages_number + 1
    if received_messages_number-1 in batch_sizes:
        times_list.append(recv_timestamp - first_recv_timestamp)

def benchmark(output_file, mqtt_client):
    while received_messages_number < max_messages_number: mqtt_client.loop()
    output = ""
    for i in range(len(times_list)):
        output += str(times_list[i])
        output += " "
    output += "\n"
    output_file.write(output)
    print(times_list)

def main():
    MQTT_broker_IP = sys.argv[1]
    qos = sys.argv[2]
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.username_pw_set("user", password="password")
    client.connect(MQTT_broker_IP, 1234, 60)

    with open("{}_mqtt_qos{}_benchmark_results".format(MQTT_broker_IP, qos), "a") as output_file:
            for _ in range(10):
                reset_globals()
                benchmark(output_file, client)

    client.disconnect()    

if __name__ == "__main__":
    main()