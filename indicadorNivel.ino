/*-------------------------------------------------------------------
  Aulas Online - LSI-TEC: IOT104 Programação física com Arduino
  Programa/hardware: 008 - Exercício não pontuado: indicador de nível
  
  Descrição: A partir da leitura do valor do potenciômetro...
  1) Todos os LEDs apagados se o valor for 0
  2) Acenda apenas o LED verde se o valor lido for até 255
  3) Acenda os LEDs verde e amarelo se o valor lido for maior ou igual a 255 e menor que 510
  4) Acenda os LEDs verde e amarelo se o valor lido for maior ou igual a 510 e menor que 765
  5) Acenda todos os LEDs se o valor lido for maior ou igual a 765

  Autor: Marcelo Wecchi
  Revisão: 14Fev2022
-------------------------------------------------------------------*/

//Entradas - Porta Analógica
#define POT A0

//Saídas - POrtas Digitais
#define GR 5
#define YL 4
#define RD 3
#define BL 2

// Variáveis de controle
int light = 0;

void setup() {
  // Definição das portas:
  Serial.begin(9600);
  pinMode(POT, INPUT);
  pinMode(GR, OUTPUT);
  pinMode(YL, OUTPUT);
  pinMode(RD, OUTPUT);
  pinMode(BL, OUTPUT);
}

void loop() {
  light = analogRead(POT);
  Serial.println(light);
  if (light == 0){
    digitalWrite(GR, LOW);
    digitalWrite(YL, LOW);
    digitalWrite(RD, LOW);
    digitalWrite(BL, LOW);   
  } else if (light < 255){
    digitalWrite(GR, HIGH);
    digitalWrite(YL, LOW);
    digitalWrite(RD, LOW);
    digitalWrite(BL, LOW);
  } else if (light < 510) {
    digitalWrite(GR, HIGH);
    digitalWrite(YL, HIGH);
    digitalWrite(RD, LOW);
    digitalWrite(BL, LOW);
  } else if (light < 765) {
    digitalWrite(GR, HIGH);
    digitalWrite(YL, HIGH);
    digitalWrite(RD, HIGH);
    digitalWrite(BL, LOW);
  } else {
    digitalWrite(GR, HIGH);
    digitalWrite(YL, HIGH);
    digitalWrite(RD, HIGH);
    digitalWrite(BL, HIGH);    
  }
}
