/* 
    / )|                                   |( \
   / / |            Charles Silva          | \ \
 _( (_ |  _  [www.charlessilva.com.br]  _  | _) )_
(((\ \)|_/ )___________________________( \_|(/ /)))
(\\\\ \_/ /                             \ \_/ ////)
 \       /                               \       /
  \    _/                                 \_    /
  /   /                                     \   \
Autor   : Charles Silva (suporte@charlessilva.com.br)
Linguagen : Arduino (C)
twitter: http://twitter.com/charlessilva_
GitHub: https://github.com/silvacharles
*/


#include <Ethernet2.h> // Shield ethernet
#include <SD.h> // Biblioteca do Cartão de Memória
#include <SPI.h>

byte mac[]= { 0x90, 0xA2, 0xDA, 0x10, 0xC7, 0x5E }; // Endereço MAC do Shield Ethernet
IPAddress ip(192, 168, 150, 2); // Endereço IP do Roteador
IPAddress subnet(255, 255, 255, 248); // Mascara de Sub-rede
IPAddress gateway(192, 168, 150, 1); // Gateway Padrão
IPAddress dnServer(8, 8, 8, 8); // DNS do Google
EthernetServer server(9000); // Porta servidor WEB

File webFile;

#define REQ_BUF_SZ    40
char HTTP_req[REQ_BUF_SZ] = { 0 };
char req_index = 0;

const int LM35 = A0; // Sensor de Temperatura
const int LDR = A1;  // Sensor de Luminosidade

int luz = 0;
int temperatura = 0;
int temp = 0;

const int carga1 = 9;
int flag1 = 0;

const int intervalo = 500;
const int seletor = A3;

//para o LED RGB:
const int Red = 5;

//para o display de 7 segmentos:
const int A = A4;
const int B = A5;
const int C = 7;
const int D = 8;
const int E = 5;
const int F = 3;
const int G = 6;

void setup() {
  pinMode(carga1, OUTPUT);
  analogReference(INTERNAL);

  // Seta todos os pinos os pinos como saídas
  pinMode(3, OUTPUT);pinMode(5, OUTPUT);pinMode(6, OUTPUT);pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);pinMode(A4, OUTPUT);pinMode(A5, OUTPUT);pinMode(A3, OUTPUT);

  // Inicia Servidor Web
  Ethernet.begin(mac, ip, dnServer, gateway, subnet);
  server.begin();

  Serial.begin(9600);

  // Verificar cartão de memória e arquivo index.htm
  Serial.println(F("Inicializando cartao MicroSD..."));
  if (!SD.begin(4)) {
    Serial.println(F("ERRO - iniciallizacao do cartao falhou!"));
    digitalWrite(seletor,LOW);
    delay(intervalo);
    // desenha um E
    digitalWrite(A, HIGH);
    digitalWrite(B, LOW);
    digitalWrite(C, LOW);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
    return;
  }
  Serial.println(F("SUCESSO - cartao MicroSD inicializado."));

  if (!SD.exists("index.htm")) {
    Serial.println(F("ERRO - index.htm nao foi encontrado!"));
    digitalWrite(seletor,LOW);
    delay(intervalo);
    // desenha um E
    digitalWrite(A, HIGH);
    digitalWrite(B, LOW);
    digitalWrite(C, LOW);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
    return;
  }
  Serial.println(F("SUCESSO - Encontrado arquivo index.htm."));

}

void loop() {
  EthernetClient client = server.available();

   //Serial.println(temp);
   //Serial.println(luz);

  if (client) {
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {

        char c = client.read();

        if (req_index < (REQ_BUF_SZ - 1)) {
          HTTP_req[req_index] = c;
          req_index++;
        }

        if (c == '\n' && currentLineIsBlank) {
          client.println(F("HTTP/1.1 200 OK"));
          client.println(F("Content-Type: text/html"));
          client.println(F("Connection: close"));
          client.println();

          if (StrContains(HTTP_req, "ajax_LerDados")) {
            LerDados(client);
          }

          if (StrContains(HTTP_req, "ajax_carga1")){
            SetCarga1();
          }

          else {

            webFile = SD.open("index.htm");
            if (webFile) {
              while (webFile.available()) {
                client.write(webFile.read());
              }
              webFile.close();
            }
          }
          Serial.println(HTTP_req);
          req_index = 0;
          StrClear(HTTP_req, REQ_BUF_SZ);
          break;
        }

        if (c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }

    delay(1);
    client.stop();

  }

}

void LerDados(EthernetClient novoCliente) {
  luz = analogRead(LDR);
  luz = map(luz, 0, 1023, 0, 100);
  novoCliente.print(luz);
  novoCliente.println("%");

  novoCliente.print("|");

  temperatura = analogRead(LM35);
  temperatura = temperatura * 0.1075268817204301;
  temp = (int)temperatura;
  novoCliente.print(temp);
  novoCliente.println("°C");
  novoCliente.print("|");
  novoCliente.print(flag1);
  novoCliente.print("|");
  
}

void StrClear(char *str, char length) {
  for (int i = 0; i < length; i++) {
    str[i] = 0;
  }
}


char StrContains(char *str, char *sfind)
{
  char found = 0;
  char index = 0;
  char len;

  len = strlen(str);

  if (strlen(sfind) > len) {
    return 0;
  }

  while (index < len) {
    if (str[index] == sfind[found]) {
      found++;
      if (strlen(sfind) == found) {
        return 1;
      }
    }
    else {
      found = 0;
    }
    index++;
  }
  return 0;
}



void SetCarga1(){
  if(flag1 == 0){
    digitalWrite(carga1, HIGH);
    digitalWrite(seletor,HIGH);
    digitalWrite(Red,HIGH);
    flag1 = 1;
  }
  else{
    digitalWrite(carga1, LOW);
    digitalWrite(seletor,HIGH);
    digitalWrite(Red,LOW);
    flag1 = 0;
  }
}
