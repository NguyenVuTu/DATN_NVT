import time
import paho.mqtt.client as mqtt


def on_publish(client, userdata, mid):
    print("message published")


client = mqtt.Client("rpi_client2") #this name should be unique
client.on_publish = on_publish
client.connect('127.0.0.1',1883)
# start a new thread
client.loop_start()
def publish_pass(room_id, password):
    try:
        if room_id == "room1":
            pubMsg = client.publish(
                topic='rpi/broadcast1',
                payload=str(password).encode('utf-8'),
                qos=0,
            )
            pubMsg.wait_for_publish()
            print(pubMsg.is_published())
        elif room_id == "room2":
            pubMsg = client.publish(
                topic='rpi/broadcast2',
                payload=str(password).encode('utf-8'),
                qos=0,
            )
            pubMsg.wait_for_publish()
            print(pubMsg.is_published())
    except Exception as e:
        print(e)
