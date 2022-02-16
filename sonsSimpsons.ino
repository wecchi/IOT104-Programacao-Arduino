/* 
 -------------------------------------------------------------------
  Aulas Online - LSI-TEC: IOT104 Programação física com Arduino
  Programa/hardware: 012 - Aula 6 - Exercício não Pontuado
  
  Descrição: Criar uma música a sua escolha
  Melody https://bitmidi.com/simpsons-3-mid

  Autor: Marcelo Wecchi
  Revisão: 16Fev2022
-------------------------------------------------------------------*/


#include "frequencias.h"

// notas da melodia
int melodia[] = {
  C6, E6, FS6, A6, G6, E6, C6, A5, G5, FS5, FS5, FS5, G5, PAUSA, FS5, FS5, FS5, G5, AS5, C6, C6, C6, C6, PAUSA
};

// duração das notas: 4 = semínima (ou 1/4 seg), 8 = colcheia (ou 1/8 seg), etc.:
int duracaoDasNotas[] = {
  6, 4, 4, 2, 6, 4, 4, 2, 2, 2, 2, 8, 4, 2, 2, 2, 2, 6, 2, 2, 2, 4, 6
};

// pino no qual está conectado o alto-falante/buzzer
const int pinoSom = 11;

void setup() {
  pinMode(pinoSom, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // a melodia se repete a cada 5 segundos
  toca();
  delay(5);
}

void toca(){
  // Calcula o tamanho da melodia, tomando o tamanho do vetor que armazena
  // os n números inteiros equivalentes à melodia e dividindo pelo tamanho
  // que um número inteiro ocupa na memória
  int tamanhoDaMelodia = sizeof(melodia)/sizeof(int);

  // itera pelas notas da melodia
  for (int essaNota = 0; essaNota < tamanhoDaMelodia; essaNota++) {
    // para calcular a duração da nota, divida 1 segundo (ou 1000 ms) pelo tipo da nota
    // por ex.: semínima = 1000 / 4, colcheia = 1000/8, etc.
    int duracaoDessaNota = 1000 / duracaoDasNotas[essaNota];
    long delayValue = 1000000 / duracaoDessaNota / 2; // calculate the delay value between transitions

    digitalWrite(LED_BUILTIN, HIGH);
    tone(pinoSom, melodia[essaNota], duracaoDessaNota);

    // Conjuga LED com o som
    delayMicroseconds(delayValue);
    digitalWrite(LED_BUILTIN, LOW);
    
    // para distinguir as notas, defina um intervalo mínimo entre elas
    // a duração da nota + 10% funciona bem (ou 1,1 vezes a nota):
    delay(duracaoDessaNota*1.1);

    // parar de tocar a nota
    noTone(pinoSom);
    
  }
}
