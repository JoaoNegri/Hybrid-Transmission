#include "HybridTransmission.h"

// Instâncias dos comunicadores para cada tipo de transmissor
Robocore_communicator *comunicadores[] = {
  new Robocore_communicator(), // HT1
  new Robocore_communicator(), // HT2
  new Robocore_communicator(), // DT
  new Robocore_communicator(), // RT
  new Robocore_communicator()  // CT
};

// Instâncias dos transmissores com documentação:
// HT1 proposto
HybridTransmission HT1(comunicadores[0], 8, 2, 1, 3);
// HT2 proposto
HybridTransmission HT2(comunicadores[1], 8, 1, 1, 2);
// DT: mensagem normal sem o protocolo
HybridTransmission DT(comunicadores[2], 8, 1, 0, 0);
// RT: com 4 repetições (máximo)
HybridTransmission RT(comunicadores[3], 8, 4, 0, 0);
// CT: com a codificação de 3 msgs anteriores
HybridTransmission CT(comunicadores[4], 8, 1, 3, 1);

// Arrays para facilitar o envio das mensagens
HybridTransmission* transmissores[] = { &HT1, &HT2, &DT, &RT, &CT };
const char* prefixos[] = { "HT100", "HT200", "DT100", "RT100", "CT100" };

void event_handler(Event type)
{
  if (type == Event::JOINED)
  {
    Serial.begin(115200);
    Serial.println("conectado!");

    char buffer[8];
    for (int i = 0; i < 255; i++)
    {
      for (int t = 0; t < 5; ++t)
      {
        // Formata o sufixo com zeros à esquerda
        snprintf(buffer, sizeof(buffer), "%s%03d", prefixos[t], i);
        transmissores[t]->send(buffer);
      }
      // delay(20000); // Descomente se necessário
    }
  }
}

void setup()
{
  const char appkey[33] = "00000000000000000000000000000000";
  const char appeui[17] = "0000000000000000";
  int portas[] = {2, 3, 4, 5, 6};

  for (int i = 0; i < 5; ++i)
  {
    comunicadores[i]->inicializa(appkey, appeui, portas[i]);
    comunicadores[i]->configuraEvento(event_handler);
  }

  Serial.begin(115200);
  Serial.print("vai conectar");
  HT1.join();
}

void loop()
{
  comunicadores[0]->ouveEvento();
}
