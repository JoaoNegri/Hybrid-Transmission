#ifndef HYBRIDTRANSMISSION_H
#define HYBRIDTRANSMISSION_H
#include "Robocore_Communicator.h"
#include <list>


class HybridTransmission
{
public:
    HybridTransmission(Communicator *comunicador, int8_t tam_msg, int8_t m =0, int8_t n =0, int8_t r=0);

    void send(char mensagem[]);

    void join();

protected:
    int8_t tam_msg;
    int n,m,r;
    Communicator *com;
    uint8_t count = 0;
    list<string> fila;

    char *enc;

    char *encode(string msg);

    void atualizaFila(char msg[]);

    char *criaCod(int n, char msgAtual[]);

    char *criaCabecalho(int cod);

    char *formataMSG(char str[], char cabecalho[]);
    
};

#endif