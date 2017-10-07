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


#include <SPI.h>
#include <Ethernet2.h>
#include <PubSubClient.h>

unsigned long readTime;
const int LM35 = A0; // Sensor de Temperatura
const int LDR = A1;  // Sensor de Luminosidade
const int pino = 9;

const int seletor = A3;
const int intervalo = 500;

//para o display de 7 segmentos:
const int A = A4;
const int B = A5;
const int C = 7;
const int D = 8;
const int E = 5;
const int F = 3;
const int G = 6;

//para o LED RGB:
const int Red = 5;
const int Green = 3;
const int Blue = 6;

//para a barreira de 4 LEDs:
const int Bar_G = 8;
const int Bar_Y1 = 7;
const int Bar_Y2 = A5;
const int Bar_R = A4;


int luz = 0;
int temperatura = 0;
int temp = 0;


byte mac[]= { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA }; // Endereço MAC do Shield Ethernet
IPAddress ip(192, 168, 150, 2); // Endereço IP do Roteador
IPAddress subnet(255, 255, 255, 248); // Mascara de Sub-rede
IPAddress gateway(192, 168, 150, 1); // Gateway Padrão
IPAddress dnServer(8, 8, 8, 8); // DNS do Google
IPAddress server(192, 168, 150, 3);


EthernetClient ethClient;
PubSubClient client(server, 1883, 0, ethClient);

//Função que será chamada ao receber mensagem do servidor MQTT
void retorno(char* topico, byte* mensagem, unsigned int tamanho) {
  
  //Convertendo a mensagem recebida para string
  mensagem[tamanho] = '\0';
  String strMensagem = String((char*)mensagem);
  strMensagem.toLowerCase();
      if(strMensagem == "liga"){
    Serial.println("Colocando o pino em estado ALTO...");
    digitalWrite(pino, HIGH);
    digitalWrite(seletor,HIGH);
    digitalWrite(Red,HIGH);
    digitalWrite(Green,LOW);
    digitalWrite(Blue,LOW);
  }else if(strMensagem == "desliga"){
    Serial.println("Colocando o pino em estado BAIXO...");
    digitalWrite(pino, LOW);
    digitalWrite(seletor,HIGH);
    digitalWrite(Red,LOW);
    digitalWrite(Green,LOW);
    digitalWrite(Blue,LOW);
  }else{
    Serial.println("Trocando o estado do pino...");
    digitalWrite(pino, !digitalRead(pino));
  }
  
}

void setup()
{
  Ethernet.begin(mac, ip);
  Serial.begin(9600);
  analogReference(INTERNAL);
  pinMode(pino, OUTPUT);
  pinMode(3, OUTPUT);pinMode(5, OUTPUT);pinMode(6, OUTPUT);pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);pinMode(A4, OUTPUT);pinMode(A5, OUTPUT);pinMode(A3, OUTPUT);

  client.setCallback(retorno);
    
   if (client.connect("arduino","usuario","senha")) {
    client.publish("outTopic","Conectado!");
    client.subscribe("blackboard/relequarto",0); //QoS 0
  }
}

//Função que reconecta ao servidor MQTT
void reconectar() {
   //Repete até conectar
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    digitalWrite(seletor, HIGH);
    //acende barreira de LEDs na ordem:
  digitalWrite(Bar_R, HIGH);
  delay(intervalo);
  digitalWrite(Bar_Y2, HIGH);
  delay(intervalo);
  digitalWrite(Bar_Y1, HIGH);
  delay(intervalo);
  digitalWrite(Bar_G, HIGH);
  delay(intervalo);
     
    if (client.connect("arduino","usuario","senha")) {
      Serial.println("Conectado!");
        digitalWrite(Bar_R, LOW);
        digitalWrite(Bar_Y2, LOW);
        digitalWrite(Bar_Y1, LOW);
        digitalWrite(Bar_G, LOW);
        // publica topico de conectada
      client.publish("outTopic","blackboard conectada!");
      // Subscreve para monitorar os comandos recebidos 
      client.subscribe("blackboard/relequarto",0); //QoS 0
    } else {
      Serial.print("falhou rc=");
      Serial.print(client.state());
      Serial.println("  tente novamente em 5 segundos");
        digitalWrite(Bar_R, LOW);
        digitalWrite(Bar_Y2, LOW);
        digitalWrite(Bar_Y1, LOW);
        digitalWrite(Bar_G, LOW);
      delay(5000);
    }
  }
}
// Lê sensores de luminosidade e temperatura
void sensorRead(){
  // recebe a variavel tempo
  readTime = millis();
  // sensor LDR
  luz = analogRead(LDR);
  luz = map(luz, 0, 1023, 0, 100);
  // converte caracteres
  char buffer[10];
  dtostrf(luz,0, 0, buffer);
  // faz publicação via MQTT
  client.publish("blackboard/ldr",buffer);
  // sensor de temperatura LM35
  temperatura = analogRead(LM35);
  temperatura = temperatura * 0.1075268817204301;
  temp = (int)temperatura;
  dtostrf(temp,0, 0, buffer);
  // faz publicação via MQTT
  client.publish("blackboard/lm35",buffer);

  
}

void loop()
{
  if (!client.connected()) {
    // tenta reconectar
    reconectar();
  }
    client.loop();
     // lê sensores após 1 minuto
     if(millis() > readTime+60000){
    sensorRead();
  }
}
