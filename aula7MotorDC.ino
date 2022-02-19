/* 
 -------------------------------------------------------------------
  Aulas Online - LSI-TEC: IOT104 Programação física com Arduino
  Programa/hardware: 013 - Aula 7 - Controlando motores (DC e servo) 
  
  Descrição: Ao pressionar o botão o motor liga
  YouTube https://youtu.be/P6AaVUF7zeI

  Autor: Marcelo Wecchi
  Revisão: 16Fev2022
-------------------------------------------------------------------*/

int pinMotor = 11;
int pinBotao = 8;
int estado = LOW;


void setup() {
  pinMode(pinMotor, OUTPUT);
  pinMode(pinBotao, INPUT_PULLUP);
  digitalWrite(pinMotor, LOW);
}

void loop() {
  estado = digitalRead(pinBotao);
  if (estado == HIGH){
    digitalWrite(pinMotor, LOW);
  } else {
    digitalWrite(pinMotor, HIGH);
  }

}
