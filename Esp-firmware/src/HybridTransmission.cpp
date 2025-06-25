#include "HybridTransmission.h"

// tamanho do cabecalho (bits)
#define tamHead 2
// maximo de cada parametro de codificacao
#define MAXm 7
#define MAXn 3
#define MAXr 7

Communicator *com;
uint8_t count = 0;
int8_t tam_msg;
int n, m, r;
list<string> fila;

/*
 * Construtor da classe, devem ser fornecidos os parâmetros de configuração m, n e r, bem como
 * o tamanho da mensagem e o comunicador com o módulo LoRaWAN
 */
HybridTransmission::HybridTransmission(Communicator *comunicador, int8_t tam_msg, int8_t m, int8_t n, int8_t r)
{
    // o tamanho maximo para numero me mensagens codificadas é 3
    if (n > MAXn)
    {
        this->n = MAXn;
    }
    else
        this->n = n;
    // o tamanho maximo para as repetições de mensagens é 7
    if (m > MAXm)
    {
        this->m = MAXm;
    }
    else
        this->m = m;
    // o tamanho maximo para a repetição de mensagens codificadas é 7
    if (r > MAXr)
    {
        this->r = MAXr;
    }
    else
        this->r = r;

    this->tam_msg = tam_msg;
    // comunicador com o módulo LoRaWAN
    com = comunicador;
    // vetor que armazena as mensagens codificadas
    this->enc = new char[tam_msg];
    for (int i = 0; i < tam_msg; i++)
    {
        this->enc[i] = 0;
    }
}
/**
 * Método responsável por atualizar a fila de mensagens que serão codificadas, e realizar a codificação
 */
void HybridTransmission::atualizaFila(char msg[])
{
    // deixa a fila com o tamanho maximo (n)
    if (this->fila.size() < n)
    {
        this->fila.push_front(msg);
    }
    else
    {

        this->fila.pop_back();

        this->fila.push_front(msg);
    }
}
char *HybridTransmission::criaCod(int n, char msgAtual[])
{

    char *codemsg = new char[this->tam_msg + tamHead];
    // encode começa em 00..., e vai fazendo xor com cada elemento da fila
    for (int i = 0; i < this->tam_msg; i++)
    {
        this->enc[i] = 0;
    }

    int aux = 0;
    bool flag = false;

    for (string s : this->fila)
    {

        if (aux == n)
        {
            flag = true;
            codemsg = encode(s);
        }
        aux++;
    }


    codemsg = encode(msgAtual);

    for (int i = 0; i < 10; i++)
    {
        Serial.println((int)codemsg[i]);
    }
    return codemsg;
}
/**
 * Método responsável por criar as mensagens codificadas e enviar para o comunicador
 **/
void HybridTransmission::send(char mensagem[])
{

    Serial.begin(115200);

    char *completeMSG = new char[(this->tam_msg) + tamHead];

    // parte das mensagens de codificação
    if (this->m >= 1)
    {
        // cria cabeçalho para mensagem de informacao
        completeMSG = criaCabecalho(0);

        completeMSG = formataMSG(mensagem, completeMSG);

        for (int i = 0; i < this->m; i++)
        {
            com->send(completeMSG, (this->tam_msg) + tamHead);
        }
    }
    // parte das mensagens codificadas
    if (this->n > 0)
    {
    char *codemsg = new char[(this->tam_msg) + tamHead];

        for (int j = 0; j < n; j++)
        {


            codemsg = criaCod(j, mensagem);
            
            char *aux = codemsg;


            // cria o cabeçalho para mensagem codificada
            completeMSG = criaCabecalho(j + 1);

            aux = formataMSG(aux, completeMSG);

            for (int i = 0; i < r; i++)
            {
                Serial.println("enviando: ");
                com->send(aux, (this->tam_msg) + tamHead);
            }
            int tmp = 0;

        }

        atualizaFila(mensagem);
    }
    // caso o contador ultrapasse 7 bits, ele retorna para 0
    this->count++;
    if (this->count == 256)
    {
        this->count = 0;
    }
}
/**
 * Método que manda o comunicador conectar na rede LoRaWAN
 **/
void HybridTransmission::join()
{
    com->join();
}
/**
 * Método responsável por codificar as mensagens
 **/
char *HybridTransmission::encode(string msg)
{


    for (int i = 0; i < this->tam_msg; i++)
    {
        this->enc[i] = this->enc[i] ^ msg[i];
    }


    return this->enc;
}
/**
 * Função responsável por criar o cabecalho do protocolo, primeiro byte contem o tipo de mensagem e o numero de mensagem
 * segundo byte contem os parametro m(3 bits), n(2 bits), r(3 bits) em ordem
 **/

char *HybridTransmission::criaCabecalho(int cod)
{

    char *cabecalho = new char[3];
    cabecalho[0] = this->count;
    cabecalho[1] = 0;

    // tres primeiros bits do segundo byte sao utilizados para identificar o paramentro "m"
    cabecalho[1] |= this->m << 5;

    // dois bits (4 e 5) do segundo byte sao utilizados para identificar o paramentro "n"
    cabecalho[1] |= cod << 3;

    // ultimos tres bits do segundo byte identificam o parametro "r"
    cabecalho[1] |= this->r;

    return cabecalho;
}
/**
 * Método responsável por formatar a mensagem, juntando o cabeçalho com o payload
 **/
char *HybridTransmission::formataMSG(char str[], char cabecalho[])
{
    char *formatado = new char[this->tam_msg + tamHead];

    formatado[0] = cabecalho[0];
    formatado[1] = cabecalho[1];
    for (int i = 2; i < this->tam_msg + tamHead; i++)
    {
        formatado[i] = str[i - 2];
    }
    return formatado;
}