from ClienteMQTT import TTNRead


def main():
    mqttc = TTNRead()
    mqttc.run()

if __name__ == "__main__":
    main()
