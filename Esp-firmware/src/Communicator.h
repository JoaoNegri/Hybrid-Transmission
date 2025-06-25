#include <string>

using namespace std;
class Communicator
{
public:
    /**
     * Função responsável por enviar a mensagem msg[] de tamanho tam
     **/
    virtual bool send(char msg[], int tam) = 0;
    /**
     * Função responsável por conectar na rede LoRaWAN
     **/
    virtual void join() = 0;
    Communicator() {}
    virtual ~Communicator(){};
};
