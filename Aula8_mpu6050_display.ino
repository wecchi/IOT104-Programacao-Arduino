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
int acX, acY, acZ, gyX, gyY, gyZ, tmp;

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
  lcd.noBacklight();
  lcd.clear();
  lcd.backlight();   

  lcd.print("--  MPU 6050  --");
  delay(2000); 
  lcd.setCursor(0, 1); 
  lcd.print("... setup OK ...");
  delay(2000);
  lcd.clear();
}

void loop() {

  ler();
  exibir();
    
  // Aguarda 5 segundos e reinicia o processo
  delay(5000);

}

void ler(){
  // Configuração do módulo para leitura
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  // Solicitação dos dados do sensor
  Wire.requestFrom(MPU, 14, true);

  // Armazena o valor dos sensores nas variaveis correspondentes
  acX=Wire.read()<<8|Wire.read(); //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  acY=Wire.read()<<8|Wire.read(); //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  acZ=Wire.read()<<8|Wire.read(); //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  tmp=Wire.read()<<8|Wire.read(); //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  gyX=Wire.read()<<8|Wire.read(); //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  gyY=Wire.read()<<8|Wire.read(); //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  gyZ=Wire.read()<<8|Wire.read(); //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}

void exibir(){  
  // Mostra os valores no monitor serial
  Serial.print("Acel. X = "); Serial.print(acX);
  Serial.print(" | Y = "); Serial.print(acY);
  Serial.print(" | Z = "); Serial.print(acZ);
  Serial.print(" | Gir. X = "); Serial.print(gyX);
  Serial.print(" | Y = "); Serial.print(gyY);
  Serial.print(" | Z = "); Serial.print(gyZ);
  Serial.print(" | Temp = "); Serial.println(tmp/340.00+36.53);

  // Exibe os valores no display
  lcd.clear();
  lcd.print("Ac("); lcd.print(acX/100);
  lcd.print(","); lcd.print(acY/100);
  lcd.print(","); lcd.print(acZ/100); lcd.print(")");
  lcd.setCursor(0, 1);
  lcd.print("Gy("); lcd.print(gyX/100);
  lcd.print(","); lcd.print(gyY/100);
  lcd.print(","); lcd.print(gyZ/100); lcd.print(")");
  lcd.print(" T:"); lcd.print(tmp/340.00+36.53);
}
