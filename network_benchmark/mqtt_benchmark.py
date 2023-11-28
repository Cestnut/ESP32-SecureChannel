import json
import paho.mqtt.client as mqtt
import time
import math

batch_sizes = [1,5,10,50,100,500]
received_messages_number = 0
begin_timestamp = 0
begin_counter = 0
current_iteration = 0
times_list = list()

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("/data/device1")

def on_message(client, userdata, msg):
    #print(math.floor(time.time()*1000))
    recv_timestamp = math.floor(time.time()*1000)
    recv_counter = math.floor(time.perf_counter()*1000)
    data = json.loads(msg.payload)
    send_timestamp = data["timestamp"]

    #print("send_timestamp:{} recv_timestamp:{} makes_sense:{} offset:{}".format(send_timestamp, recv_timestamp, recv_timestamp>send_timestamp, recv_timestamp-send_timestamp))

    global received_messages_number
    global begin_timestamp
    global begin_counter
    global current_iteration

    if received_messages_number == 0:
        begin_timestamp = send_timestamp
        begin_counter = math.floor(time.perf_counter()*1000)
        begin_counter = begin_counter - (recv_timestamp - begin_timestamp)
        #print(begin_timestamp)

    if current_iteration < len(batch_sizes):
        max_messages_number = batch_sizes[current_iteration]

    if received_messages_number < max_messages_number:
        received_messages_number = received_messages_number + 1
        #print("{} {} {}".format(received_messages_number, send_timestamp, recv_timestamp))
        if received_messages_number == max_messages_number:
            print("{} {} {} {} {}".format(send_timestamp, recv_timestamp, recv_timestamp-send_timestamp, recv_timestamp-begin_timestamp, recv_counter-begin_counter))
            #print(recv_timestamp - begin_timestamp)
            current_iteration = current_iteration + 1
            received_messages_number = 0
            times_list.append(recv_counter - begin_counter)
    #print("\n")

def main():
    MQTT_broker_IP = "10.0.0.1"
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.username_pw_set("user", password="password")
    client.connect(MQTT_broker_IP, 1234, 60)

    with open("{}_mqtt_benchmark_results".format(MQTT_broker_IP), "w") as output_file:
        output_file.write("TOTAL TIME\n")
        while current_iteration < len(batch_sizes): client.loop()
        for i in range(len(times_list)):
            print(i)
            output = "{} {}\n".format(batch_sizes[i], times_list[i]-((batch_sizes[i]-1)*1000))
            output_file.write(output)
    print(times_list)
    client.disconnect()    

if __name__ == "__main__":
    main()