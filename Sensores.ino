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

const int LM35 = A0; // Sensor de Temperatura
const int LDR = A1;  // Sensor de Luminosidade

int luz = 0;
int temperatura = 0;
int temp = 0;

void setup() {

  analogReference(INTERNAL);
  Serial.begin(9600);
  
}

void loop() {

  luz = analogRead(LDR);
  luz = map(luz, 0, 1023, 0, 100);
 
  temperatura = analogRead(LM35);
  temperatura = temperatura * 0.1075268817204301;
  temp = (int)temperatura;

   Serial.println(temp);
   Serial.println(luz);

}
