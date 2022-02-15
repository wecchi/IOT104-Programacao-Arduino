/*-------------------------------------------------------------------
  Aulas Online - LSI-TEC: IOT104 Programação física com Arduino
  Programa/hardware: 010 - Aula 5 - Programando saídas PWM
  
  Descrição: Instalação artística com LEDs coloridos que piscam em 
  intensidades e ritmos diferentes, quando a variação conincidir com 
  com um valor randômico, um certo bip (som) é emitido pelo buzzer.

  Autor: Marcelo Wecchi
  Revisão: 14Fev2022
-------------------------------------------------------------------*/

int crtl =  11;  // Leitura da aleatoriedade do LED
int buzzer = 7;  // Sinal sonoro
int min_val = 290; // mínimo valor para a aleatoriedade
int max_val = 315;  // máximo valor para a aleatoriedade
int val = 0;       // valor de leitura do módulo 7 color flash
int rnd_val = 0;

void setup()   {                
  // Iniciando as portas de comunicação
  pinMode(crtl, INPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);

  // Sorteando um valor aleatório
  rnd_val = random(min_val, max_val);
}

void loop()                     
{
  val = analogRead(crtl);
  Serial.println(val);  
    
  if (val == rnd_val){
    digitalWrite(buzzer, HIGH);
    delay(10);
    digitalWrite(buzzer, LOW);
  }
}
