/*
 *  Aulas Online - LSI-TEC: IOT104 Programação física com Arduino
 *  Programa/hardware: 013 - Aula 9 - Bluetooth: Controlando um LED com o monitor serial
 *  Descrição: 1º estágio do desenvolvimento é ler um valor de uma entrada serial para 
 *  ligar ou desligar um LED.
 *  
 *  Autor: Marcelo Wecchi
 *  Revisão: 17Fev2022
 */

#define LED_AZUL 7

char valorSerial = "";
const char msg[] = "Digite as letras 1, 'l' ou 'L' para ligar o LED\nDigite as letras 0, 'd' ou 'D' para desligar o LED";

void setup() {
  Serial.begin(9600);
  pinMode(LED_AZUL, OUTPUT);
  for (int i = 1; i < 4; i++){
    digitalWrite(LED_AZUL, HIGH);
    delay(200);
    digitalWrite(LED_AZUL, LOW);
    delay(200);
  }
  Serial.println(msg);
}

void loop() {
  if (Serial.available()>0){
    valorSerial = Serial.read();
    delay(100);
    if (valorSerial == '1' | valorSerial == 'l' | valorSerial == 'L'){
      digitalWrite(LED_AZUL, HIGH);
      Serial.println("\nLED ligado");
      valorSerial = "";
    } else if (valorSerial == '0' | valorSerial == 'd' | valorSerial == 'D'){
      digitalWrite(LED_AZUL, LOW);
      Serial.println("\nLED desligado");
      valorSerial = "";
    } else if (valorSerial == 'h' | valorSerial == 'H'){
      Serial.println(msg);
      valorSerial = "";
    }
    delay(2000);
  }

}
