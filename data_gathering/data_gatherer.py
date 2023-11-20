import json
import paho.mqtt.client as mqtt
from pymongo import MongoClient

def mongo_init(host, port, username, password, db_name):
    client = MongoClient('{}:{}'.format(host, port),
                     username=username,
                     password=password,
                     authSource='admin',
                     authMechanism='SCRAM-SHA-256')
    database = client[db_name]
    return database
database = mongo_init("10.0.0.1", 27017, "GATHERER_USERNAME","GATHERER_PASSWORD", "IoT_network")


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("/data/+")


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    
    try:
        data = json.loads(msg.payload)

        if(msg.topic.startswith('/data/')):
            device_name = msg.topic.split("/data/")[1]
            data_collection = database["data"]
            data["device_name"] = device_name
            print(data)
            data_collection.insert_one(data)
    except json.decoder.JSONDecodeError as e:
        print("An exception occurred: {}".format(e))

def main():
    MQTT_broker_IP = "10.0.0.1"
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(MQTT_broker_IP, 1234, 60)
    client.loop_forever()

if __name__ == "__main__":
    main()