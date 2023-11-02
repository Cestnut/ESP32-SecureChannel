import json
import paho.mqtt.client as mqtt
from pymongo import MongoClient

def mongo_init(host, port, db_name):
    client = MongoClient("mongodb://{}:{}".format(host,port))
    database = client[db_name]
    return database
database = mongo_init("10.0.0.1", 27017, "IoT_network")


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("/data/+")
    client.subscribe("/config/+")


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    if(msg.topic.startswith('/data/')):
        device_name = msg.topic.split("/data/")[1]
        data_collection = database["data"]
        data = json.loads(msg.payload)
        data["device_name"] = device_name
        print(data)
        data_collection.insert_one(data)
    elif(msg.topic.startswith('/config/')):
        device_name = msg.topic.split("/config/")[1]
        data_collection = database["config"]
        data = json.loads(msg.payload)
        data["device_name"] = device_name
        print(data)
        data_collection.insert_one(data)
    
def main():
    MQTT_broker_IP = "10.0.0.1"
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(MQTT_broker_IP, 1234, 60)
    client.loop_forever()

if __name__ == "__main__":
    main()