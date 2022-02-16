/* 
 -------------------------------------------------------------------
  Aulas Online - LSI-TEC: IOT104 Programação física com Arduino
  Programa/hardware: 013 - Aula 7 Servo motor o que é e como usar
  
  Descrição: Controlando o angulo de um servo motor. a cada 2 seg
  desloca 90 graus
  YouTube: https://youtu.be/ATn8xTQGnWI

  Autor: Marcelo Wecchi
  Revisão: 16Fev2022
-------------------------------------------------------------------*/

#include <Servo.h>

Servo meuServo;

void setup() {
  meuServo.attach(9);
}

void loop() {
  for (int angulo = 180; angulo >= 0; angulo -= 30){
    meuServo.write(angulo);
    delay(2000);
  }
}
