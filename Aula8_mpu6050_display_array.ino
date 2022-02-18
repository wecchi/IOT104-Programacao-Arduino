/*
  Aulas Online - LSI-TEC: IOT104 Programação física com Arduino
  Programa/hardware: 013 - Aula 8 - Exercicio não pontuado
  
  Descrição: Conectando o módulo MPU6050 (acelerômetro e giroscópio) nas portas SDA e SCL
  Leitura de diversos sensores por meio do protocolo de comunicação I2C.
  Exibição dos valore lidos no display

  Autor: Marcelo Wecchi
  Revisão: 17Fev2022
*/

// Biblioteca requerida para comunicação com o módulo
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Endereço físico do acelerômetro
const int MPU=0x68;

// Variáveis para armazar valores dos sensores presentes no módulo
int reads[7];

// Configuração do Display 16 x 2
#define endereco  0x3F // Endereços comuns: 0x27, 0x3F
#define colunas   16
#define linhas    2
LiquidCrystal_I2C lcd(endereco, colunas, linhas);

void setup() {
  // inicialização do monitor serial
  Serial.begin(9600);

  // iniciando a comunicação com o Slave-MPU6050
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  // iniciando a comunicação com o display
  lcd.init();
  lcd.clear();  
  lcd.noBacklight();
  
  lcd.print("--  MPU 6050  --");
  lcd.backlight();
  delay(2000); 
  lcd.setCursor(0, 1);
  lcd.print(char(210)); lcd.print(" sw_ver 1.19 "); lcd.print(char(210));
  delay(2000);
  lcd.clear();
  lcd.noBacklight();
  delay(1000);
  lcd.backlight();
}

void loop() {

  ler();
  exibir();
    
  // Aguarda 5 segundos e reinicia o processo
  delay(3000);

}

void ler(){
  // Configuração do módulo para leitura
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  // Solicitação dos dados do sensor
  Wire.requestFrom(MPU, 14, true);

  // Armazena o valor dos sensores nas variaveis correspondentes
  for (int i = 0; i < 7; i++){
    reads[i] = Wire.read()<<8|Wire.read();
  }
}

void exibir(){

  float temp = reads[3]/340.00+36.53;
  
  // Imprime os valores no monitor serial
  Serial.print("Acel. X = "); Serial.print(reads[0]);
  Serial.print(" | Y = "); Serial.print(reads[1]);
  Serial.print(" | Z = "); Serial.print(reads[2]);
  Serial.print(" | Gir. X = "); Serial.print(reads[4]);
  Serial.print(" | Y = "); Serial.print(reads[5]);
  Serial.print(" | Z = "); Serial.print(reads[6]);
  Serial.print(" | Temp. = "); Serial.println(temp);

  // Exibe os valores no display
  lcd.clear();
  lcd.print("Ac("); lcd.print(reads[0]/100);
  lcd.print(","); lcd.print(reads[1]/100);
  lcd.print(","); lcd.print(reads[2]/100); lcd.print(")");
  lcd.setCursor(0, 1);
  lcd.print("Gy("); lcd.print(reads[4]/100);
  lcd.print(","); lcd.print(reads[5]/100);
  lcd.print(","); lcd.print(reads[6]/100); lcd.print(")");
  delay(3000);
  lcd.clear();
  lcd.print("Temp.: "); lcd.print(temp); lcd.print(char(223)); lcd.print("C");
  lcd.setCursor(0, 1);
  if (temp <= 10){
    lcd.print("** muito frio **");
  } else if (temp <= 19){
    lcd.print("**   frio   **");
  } else if (temp <= 30) {
    lcd.print("** agradavel **");
  } else if (temp <= 36){
    lcd.print("**   quente  **");
  } else {
    lcd.print("**muito quente**");
  }
}
