import base64
from ctypes import util
import json

## Classe receptora das mensagens Hybrid Transmission
#
class HybridTransmissionReceiver:
    def __init__(self):
        self.maxrep = 0
        self.conta = 0
        self.devices = dict()
        self.fila = []


    ## Recebe a mensagem da TTN e retorna a string do payload decodificada
    def recebemensagem(self,ttnmessage):
        my_json = ttnmessage.payload.decode('UTF-8')
        data = json.loads(my_json)
        payload = base64.b64decode(data["uplink_message"]["frm_payload"])
        print("fport: ")
        print(data["uplink_message"]["f_port"])
        # print(payload)
        try:
            self.devices[data["end_device_ids"]["dev_addr"] + ":" + str(data["uplink_message"]["f_port"])]
        except:
            # devices é um dicionario que tem duas listas a lista[0] contém as msg de informacao (com cabecalho)
            # a lista[0] contém as msgs de codificacao (com cabecalho)
            self.devices[data["end_device_ids"]["dev_addr"] + ":" + str(data["uplink_message"]["f_port"])] = []
            self.devices[data["end_device_ids"]["dev_addr"] + ":" + str(data["uplink_message"]["f_port"])].append(list())
            self.devices[data["end_device_ids"]["dev_addr"] + ":" + str(data["uplink_message"]["f_port"])].append(list())

        print(int(payload[0]))
        print(int((payload[1] & 0b00011000) >>3))

        # transforma em string
        payloadstring = [payload, data["end_device_ids"]["dev_addr"] + ":" + str(data["uplink_message"]["f_port"])]
        

        file = open("informacao_recebida/"+data["end_device_ids"]["dev_addr"] + ":" + str(data["uplink_message"]["f_port"])+" tudo",'a')
        
        file.write(str(payload) + "\n")
        file.close()
        return payloadstring

    ## faz a filtragem de mensagens repetidas
    # mensagens filtradas devem ser publicadas em uma fila de mensagens
    def filtramensagem(self, mensagem):
                
        print("mensagem: " + str(mensagem[0][2:]))

        print(int(mensagem[0][1]))

        if ((int(mensagem[0][1]) & 0b00011000)>>3 == 0):
            # info = str(numeroMsg) + mensagem[0][1:].decode('ASCII')

            print("devices: ")
            print (self.devices[mensagem[1]][0])
            print("mensagem: ")
            print(mensagem[0])
            if (self.devices[mensagem[1]][1].count(mensagem[0]) != 0):
                # mensagem igual, deve apenas ser descartada
                print("mensagem repetida será descartada")
                
            else:
                #mensagem nova, deve ser salva!
                file = open("informacao_recebida/"+mensagem[1] + " recebida",'a')
                file.write(str(mensagem[0]) + "\n")
                file.close()

                #adiciona a msg de informacao a lista de msg de informacao
                self.devices[mensagem[1]][1].append(mensagem[0])

        else:

            if (self.devices[mensagem[1]][0].count(mensagem[0]) != 0):
                # mensagem igual, deve apenas ser descartada
                print("mensagem repetida será descartada")
                
            else:
                #mensagem nova, deve ser salva!

                #adiciona a msg de codificacao a lista de msg de codificacao
                self.devices[mensagem[1]][0].append(mensagem[0])


    def decodifica(self,msg1, msg2,msg2Existe):
        msg = []
        if msg2Existe:
            for a in range(len(msg1)):
                msg.append(msg1[a] ^ msg2[a])
            return [msg, True]
        return ["", False]


    def verificaPerdidas(self):
        for dev in self.devices:
            # self.teste(dev)
            print(self.devices[dev])
            info_list = self.devices[dev][1]
            if len(info_list) != 0:
                msgPerdida = self.teste(dev)
                if (len(msgPerdida) != 0):
                    print(self.devices[dev][0])
                    for cod in self.devices[dev][0]:
                        # msg de codificacao da msg perdida
                        for perdidas in msgPerdida:
                            if (int(cod[0]) == perdidas):
                                if((int(cod[1]) & 0b00011000)>>3 == 1):
                                    # xor com msg anterior
                                    infoXor = self.obtemInfo(perdidas-1,dev)
                                    if (infoXor[1]):
                                        msgDecodificada = self.decodifica(cod[2:],infoXor[0][2:],infoXor[1]) 
                                        print ("xor com msg anterior")
                                        self.addLista(dev,perdidas,msgDecodificada)
                                        return
                                elif((int(cod[1]) & 0b00011000)>>3 == 2):
                                    #xor com a msg 2 antes
                                    infoXor = self.obtemInfo(perdidas-2,dev)
                                    if (infoXor[1]):
                                        msgDecodificada = self.decodifica(cod[2:],infoXor[0][2:],infoXor[1]) 
                                        print ("xor com a msg 2 antes")
                                        self.addLista(dev,perdidas,msgDecodificada)
                                        return
                                elif((int(cod[1]) & 0b00011000)>>3 == 3):
                                    #xor com a msg 3 antes
                                    infoXor = self.obtemInfo(perdidas-3,dev)
                                    if (infoXor[1]):
                                        msgDecodificada = self.decodifica(cod[2:],infoXor[0][2:],infoXor[1]) 
                                        print ("xor com a msg 3 antes")
                                        self.addLista(dev,perdidas,msgDecodificada)
                                        return
                            elif (int(cod[0]) == perdidas + 1):
                                if((int(cod[1]) & 0b00011000)>>3 == 1):
                                    infoXor = self.obtemInfo(cod[0],dev)
                                    if (infoXor[1]):
                                        msgDecodificada = self.decodifica(cod[2:],infoXor[0][2:],infoXor[1]) 
                                        print ("xor com msg de cod igual a cod[0]")
                                        self.addLista(dev,perdidas,msgDecodificada)
                                        return
                            elif (int(cod[0]) == perdidas + 2):
                                if((int(cod[1]) & 0b00011000)>>3 == 2):
                                    infoXor = self.obtemInfo(cod[0],dev)
                                    if (infoXor[1]):
                                        msgDecodificada = self.decodifica(cod[2:],infoXor[0][2:],infoXor[1]) 
                                        print ("xor com msg de cod igual a cod[0]")
                                        self.addLista(dev,perdidas,msgDecodificada)
                                        return
                            elif (int(cod[0]) == perdidas + 3):
                                if((int(cod[1]) & 0b00011000)>>3 == 3):
                                    infoXor = self.obtemInfo(cod[0],dev)
                                    if (infoXor[1]):
                                        msgDecodificada = self.decodifica(cod[2:],infoXor[0][2:],infoXor[1]) 
                                        print ("xor com msg de cod igual a cod[0]")
                                        self.addLista(dev,perdidas,msgDecodificada)
                                        return
        for msg in self.devices:
            arquivo = open("informacao_recebida/"+msg+" info",'w')
            arquivo.write(str(self.devices[msg][1]))
            arquivo.close()



    def obtemInfo(self, num, dev):
        for info in self.devices[dev][1]:
            if (int(info[0]) == num):
                return [info, True]
        return ["", False]


    def teste(self, dev):
        lst = []
        for info in self.devices[dev][1]:
            lst.append(info[0])

        start = lst[0]
        end = lst[-1]
        print(sorted(set(range(start, end + 1)).difference(lst)))
        return sorted(set(range(start, end + 1)).difference(lst))
        # return [0,"0", False]
        
    def addLista(self,dev,num,msg):
        i =0
        if (msg[1]):
            for info in self.devices[dev][1]:
                if (int(info[0]) >= num):
                    msg[0].insert(0,info[1])
                    msg[0].insert(0,num)
                    inserir = bytes(msg[0])

                    self.devices[dev][1].insert(i,inserir)
                    return
                aux = info[1]
                i = i+1
            for info in self.devices[dev][1]:
                if (int(info[0]) < num):
                    msg[0].insert(0,info[1])
                    msg[0].insert(0,num)
                    inserir = bytes(msg[0])

                    self.devices[dev][1].insert(i+1,inserir)
                    return
                aux = info[1]
                i = i+1
            msg[0].insert(0,num)
            msg[0].insert(1,aux)
            inserir = bytes(msg[0])

            self.devices[dev][1].insert(0,inserir)