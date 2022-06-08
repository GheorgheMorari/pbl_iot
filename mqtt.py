from paho.mqtt import client as mqtt_client

broker = 'node02.myqtthub.com'
port = 1883
client_id = 'pbl_device'
username = 'pbl'
password = '13243546q'


class MQTTManager:
    def __init__(self):
        self.client = connect_mqtt()
        self.client.loop_start()

    def publish(self, message, topic):
        self.client.publish(topic, message)

    def subscribe(self, topic, callback):
        self.client.subscribe(topic)
        self.client.on_message = callback

    def unsubscribe(self, topic):
        self.client.unsubscribe(topic)

    def stop(self):
        self.client.loop_stop()
        self.client.disconnect()

    __del__ = stop


def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    # Set Connecting Client ID
    client = mqtt_client.Client(client_id)
    client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client


mqtt_manager = MQTTManager()
# mqtt_manager.connect()
# TODO create unique_id
# TODO return the unique id to the microcontroller
# TODO subscribe microcontroller to the unique_id+"/control" topic
# TODO subscribe the web server to the unique_id+"/data" topic
# TODO publish the data to the unique_id+"/data" topic from the microcontroller
# TODO publish the control to the unique_id+"/control" topic from the web server
# TODO change some variables on the microcontroller depending on the messages received from the control topic
