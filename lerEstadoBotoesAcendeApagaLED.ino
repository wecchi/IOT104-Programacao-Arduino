/*-------------------------------------------------------------------

//  Aulas Online - LSI-TEC: IOT104 Programação física com Arduino
//  Programa/hardware: 003 - Ligando/Desligando o LED com 2 botões
//  Autor: Marcelo Wecchi
//  Revisão: 10Fev2021
-------------------------------------------------------------------*/

//Entradas - Portas Digitais
#define botaoLiga 2
#define botaoDesliga 4

//Saídas - Porta Digital
#define LED_01 7

//Definindo variáveis
bool x = false; //variável auxiliar

void setup() {
  // Definição das portas:
  Serial.begin(9600);
  pinMode(botaoLiga, INPUT);
  pinMode(botaoDesliga, INPUT);
  pinMode(LED_01, OUTPUT);
}

void loop() {
  // Verifica quando o botão Liga foi pressionado
  x = (digitalRead(botaoLiga) == HIGH); // Ocorre quando Liga pressionado

  // Nesta condição entra no loop enquanto o botão Desliga não for pressionado
  if (x){
    while (x) {
      digitalWrite(LED_01, HIGH); // Liga o LED
      delay(50);                  // Delay para estabilizar aas leituras     
      Serial.println("LIGADO");   // Printa status

      if (digitalRead(botaoDesliga) == HIGH) // Ocorre quando Desliga é pressionado
      {
        x = false;
      }
      
    }
  } 

  // Nesta condição o LED permanece apagado
  if (x == false){
    digitalWrite(LED_01, LOW);
    Serial.println("desligado");
  }  
}
