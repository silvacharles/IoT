// inclui o SoftwareSerial na biblioteca
#include "SoftwareSerial.h"

// Cria um novo software  serial
SoftwareSerial bluetooth(2, 3); //RX, TX

void setup() {
  // Inicializa o hardware serial
  Serial.begin(9600);
  Serial.println(F("Digite o comandos AT: "));

  //Inicializa o software serial
  bluetooth.begin(9600);
}

void loop() {
  // verificar recebeu um byte de hardware serial
  if (Serial.available()) {
    char r = Serial.read(); // Leia e salve o byte
    bluetooth.print(r);  // Enviar o byte para bluetooth por software serial
    Serial.print(r);  // Printa na tela
  }
 // Verificação recebeu um byte do bluetooth por software serial
  if (bluetooth.available()) {
    char r = bluetooth.read(); // Leia e salve o byte
    Serial.print(r); //Printa o byte do hardware serial
  }
}
