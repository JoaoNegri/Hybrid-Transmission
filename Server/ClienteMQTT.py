from datetime import datetime
import paho.mqtt.client as mqtt
from HybridTransmissionReceiver import HybridTransmissionReceiver



class TTNRead(mqtt.Client):
    def __init__(self):
        super().__init__()
        self.HT = HybridTransmissionReceiver()


    def on_connect(self, mqttc, obj, flags, rc):
        print("rc: "+str(rc))

    def on_connect_fail(self, mqttc, obj):
        print("Connect failed")

    def on_message(self, mqttc, obj, msg):
        print(msg.topic)

        payloadstring = self.HT.recebemensagem(msg)
        teste = self.HT.filtramensagem(payloadstring)
        self.HT.verificaPerdidas()

    def on_publish(self, mqttc, obj, mid):
        print("mid: "+str(mid))

    def on_subscribe(self, mqttc, obj, mid, granted_qos):
        print("Subscribed: "+str(mid)+" "+str(granted_qos))

    def on_log(self, mqttc, obj, level, string):
        print(string)

    def run(self):
        self.username_pw_set(username="yourUsarname@ttn", password="yourPassword")
        self.connect("region.cloud.thethings.network", 1883, 60)
        self.subscribe("vX/yourUploadLink", 0)

        # loop do mqtt
        while True:
            self.loop()
        
