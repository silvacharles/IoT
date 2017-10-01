
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

#include <LiquidCrystal.h>

const int sensorLuz = A0;
const int sensorTemp = A1;

int valorSensorLuz = 0;
int valorSensorTemp = 0;

// declara os pinos do LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // seta linhas e colunas do display
  lcd.begin(16, 2);

}

void loop() {
  valorSensorLuz = analogRead(sensorLuz);
  valorSensorLuz = map(valorSensorLuz, 0, 1023, 0, 100);
  for ( int i = 1; i <= 8; i++){
    valorSensorTemp = analogRead(sensorTemp);
    valorSensorTemp *= 0.54;
  }

   delay(150);

   lcd.clear();
   lcd.setCursor(8,0);
   lcd.print("Temp:");
   lcd.print(valorSensorTemp);
   // escreve o simbolo grau
   lcd.write(B11011111);
   lcd.print("C");

   lcd.setCursor(9 ,1);
   lcd.print("Luz:");
   lcd.print(valorSensorLuz);
   lcd.print("%");

    if (valorSensorTemp > 30 ){
    Bad();

    } else{
       Happy();
   }
   delay(2000);

}
// smile Feliz
void Happy() {
 byte thumb1[8] = {B00100,B00011,B00100,B00011,B00100,B00011,B00010,B00001};
 byte thumb2[8] = {B00000,B00000,B00000,B00000,B00000,B00000,B00000,B00011};
 byte thumb3[8] = {B00000,B00000,B00000,B00000,B00000,B00000,B00001,B11110};
 byte thumb4[8] = {B00000,B01100,B10010,B10010,B10001,B01000,B11110,B00000};
 byte thumb5[8] = {B00010,B00010,B00010,B00010,B00010,B01110,B10000,B00000};
 byte thumb6[8] = {B00000,B00000,B00000,B00000,B00000,B10000,B01000,B00110};
 lcd.createChar(0, thumb1);
 lcd.createChar(1, thumb2);
 lcd.createChar(2, thumb3);
 lcd.createChar(3, thumb4);
 lcd.createChar(4, thumb5);
 lcd.createChar(5, thumb6);
 lcd.setCursor(0,1);
 lcd.write(byte(0));
 lcd.setCursor(0,0);
 lcd.write(1);
 lcd.setCursor(1,1);
 lcd.write(2);
 lcd.setCursor(1,0);
 lcd.write(3);
 lcd.setCursor(2,1);
 lcd.write(4);
 lcd.setCursor(2,0);
 lcd.write(5);
}
// smile triste
void Bad() {
 byte thumb1[8] = {B00001,B00010,B00011,B00100,B00011,B00100,B00011,B00100};
 byte thumb2[8] = {B00011,B00000,B00000,B00000,B00000,B00000,B00000,B00000};
 byte thumb3[8] = {B11110,B00001,B00000,B00000,B00000,B00000,B00000,B00000};
 byte thumb4[8] = {B00000,B11110,B01000,B10001,B10010,B10010,B01100,B00000};
 byte thumb5[8] = {B00000,B10000,B01110,B00010,B00010,B00010,B00010,B00010};
 byte thumb6[8] = {B00110,B01000,B10000,B00000,B00000,B00000,B00000,B00000};
 lcd.createChar(0, thumb1);
 lcd.createChar(1, thumb2);
 lcd.createChar(2, thumb3);
 lcd.createChar(3, thumb4);
 lcd.createChar(4, thumb5);
 lcd.createChar(5, thumb6);
 lcd.setCursor(0,0);
 lcd.write(byte(0));
 lcd.setCursor(0,1);
 lcd.write(1);
 lcd.setCursor(1,0);
 lcd.write(2);
 lcd.setCursor(1,1);
 lcd.write(3);
 lcd.setCursor(2,0);
 lcd.write(4);
 lcd.setCursor(2,1);
 lcd.write(5);
}
