#ifndef ROBOCORE_COMMUNICATOR_H
#define ROBOCORE_COMMUNICATOR_H
#include "Communicator.h"
#include <RoboCore_SMW_SX1276M0.h>
#include <HardwareSerial.h>

class Robocore_communicator : public Communicator
{

public:
    Robocore_communicator();

    void inicializa(const char appKEY[], const char appEUI[], int fport);

    bool send(char msg[], int tam);

    void configuraEvento(void (*func)(Event));

    void ouveEvento();

    void join();

    virtual ~Robocore_communicator();

private:
    SMW_SX1276M0 lorawan;
    HardwareSerial LoRaSerial;
    string fport;
};
#endif