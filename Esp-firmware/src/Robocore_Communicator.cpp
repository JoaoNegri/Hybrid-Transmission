#include "Robocore_Communicator.h"

Robocore_communicator::Robocore_communicator() : lorawan(LoRaSerial), LoRaSerial(2)
{}

void Robocore_communicator::inicializa(const char appKEY[], const char appEUI[], int fport)
{
    lorawan.setPinReset(5);
    lorawan.reset(); // Realiza um reset no modulo

    // Inicia a comunicacao serial com o modulo
    LoRaSerial.begin(115200, SERIAL_8N1, 16, 17);

    lorawan.set_AppEUI(appEUI);
    lorawan.set_AppKey(appKEY);
    lorawan.set_JoinMode(SMW_SX1276M0_JOIN_MODE_OTAA);
    this->fport =  to_string(fport);
}

bool Robocore_communicator::send(char msg[], int tam)
{
    Serial.println("enviando mensagem...");
    Stream *a(&LoRaSerial);
    a->flush();
    char comandoSend[] = "AT+SENDB ";
    char quebraLinha[] = "\n";

    LoRaSerial.print(comandoSend);

    for (char d : this->fport){
        LoRaSerial.print(d);
    }

    LoRaSerial.print(":");

    for (int i=0; i < tam; i++){
        if ((int)msg[i] < 16){
            LoRaSerial.print(0, HEX);
        }
        LoRaSerial.print((int)msg[i], HEX);
    }
    LoRaSerial.print(quebraLinha);

    for (int i=0; i < tam; i++){
        Serial.println((int)msg[i]);
    }

    // TODO ver sobre o delay
    delay(20000);

    return true;
}

void Robocore_communicator::configuraEvento(void (*func)(Event))
{
    lorawan.event_listener = func;
}

void Robocore_communicator::ouveEvento()
{
    lorawan.listen();
}

void Robocore_communicator::join()
{
    lorawan.join();
}

Robocore_communicator::~Robocore_communicator() {}
