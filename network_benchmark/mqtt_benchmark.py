import json
import paho.mqtt.client as mqtt
import time
import math

batch_sizes = [1,5,10,50,100]
max_messages_number = batch_sizes[-1]

received_messages_number = 0
first_send_timestamp = 0
first_recv_timestamp = 0

times_list = list()
arrivals_list = list()

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("/data/device1")

def on_message(client, userdata, msg):
    recv_timestamp = math.floor(time.time()*1000)
    
    data = json.loads(msg.payload)
    send_timestamp = data["timestamp"]

    global received_messages_number
    global first_send_timestamp
    global first_recv_timestamp
    global max_messages_number
    global batch_sizes

    if received_messages_number == 0:
        first_send_timestamp = send_timestamp
        first_recv_timestamp = recv_timestamp

    if received_messages_number < max_messages_number:
        received_messages_number = received_messages_number + 1
        arrivals_list.append(recv_timestamp)
        if received_messages_number in batch_sizes:
            print("{} {} {} {} {} {}".format(received_messages_number, send_timestamp, recv_timestamp, recv_timestamp-send_timestamp, recv_timestamp-first_send_timestamp, recv_timestamp - first_recv_timestamp))
            times_list.append([recv_timestamp - first_send_timestamp, recv_timestamp - first_recv_timestamp])

def main():
    MQTT_broker_IP = "10.0.0.1"
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.username_pw_set("user", password="password")
    client.connect(MQTT_broker_IP, 1234, 60)

    with open("{}_mqtt_benchmark_results".format(MQTT_broker_IP), "w") as output_file:
        output_file.write("MESSAGES_NUMBER TOTAL_TIME PARTIAL_TIME\n")
        while received_messages_number < max_messages_number: client.loop()
        for i in range(len(times_list)):
            output = "{} {} {}\n".format(batch_sizes[i], times_list[i][0], times_list[i][1])
            output_file.write(output)
        
        latency = list()
        for i in range(len(arrivals_list)-1):
            latency.append(arrivals_list[i+1]-arrivals_list[i])

    print(latency)
    print(times_list)
    client.disconnect()    

if __name__ == "__main__":
    main()