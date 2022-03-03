/*
 *  Aulas Online - LSI-TEC: IOT104 Programação física com Arduino
 *  Programa/hardware: 017 - Aula 11 - Conectar o ESP01 (WiFi) no Thingspeak
 *  
 *  Descrição: Configura um servidor na ESP-01 para:
 *     - controlar um atuador - indicação de leitura(liga ou desliga LED da Arduino);
 *     - ler um sensor LDR (Luminosidade);
 *     - ler um sensot DHT11 (Humidade e temperatura);
 *     - enviar dados para o Thingspeak
 * 
 *  The circuit:
 *     RX is digital rxPin (connect to TX of other device)
 *     TX is digital txPin (connect to RX of other device)
 * 
 *  Autor: Marcelo Wecchi
 *  Revisão: 02Mar2022
*/

#include "DHT.h"
#include <stdlib.h>
#include <SoftwareSerial.h>

// --- Constantes --- 
String strSSID = "@homehorana";           // Nome da rede WiFi
String strPSW = "2020Amor&Respeito2020";  // Senha da rede WiFi
int pinoLED = 13;
int pinoLDR = A0;
int pinoDHT = 8;
int tipoDHT =  DHT11;
int rxPin = 6;
int txPin = 7;

// --- Criação de objetos --- 
DHT dht(pinoDHT, tipoDHT);            // Inicializa o sensor
SoftwareSerial esp8266(rxPin, txPin); // Inicializa comunicacao com WiFi

// substitua pela chave de leitura do seu canal no Thingspeak
String chaveDeEscrita = "BX8YGTRQCV8ZWCWU";

// =============================================================================================================
// --- Assinatura das Funções Auxiliares ---
String sendData(String command, const int timeout, const int val_delay, const boolean debug);
void init_ESP8266();
void piscaLED();
void enviaDadosThingSpeak(String strLuminosidade, String strUmidade, String strTemperatura);

void setup() {                
  pinMode(pinoLED, OUTPUT);
  Serial.begin(9600); // Comunicacao com Monitor Serial
  esp8266.begin(115200); // Comunicacao com Modulo WiFi
  dht.begin();
  if(esp8266.isListening()){
    init_ESP8266();    
  } else {
    Serial.println("Setup has error and NOT complete!!");
  }
}

void loop() {
  // valores baixos: luminosidade baixa
  float luminosidade = 1024.0f - analogRead(pinoLDR);

  // Coleta Umidade e Temperatura
  float umidade = dht.readHumidity();
  float temperatura = dht.readTemperature();

  // converte leituras para String
  String strLuminosidade = floatToString(luminosidade);
  String strUmidade = floatToString(umidade);
  String strTemperatura = floatToString(temperatura);
  
  enviaDadosThingSpeak(strLuminosidade, strUmidade, strTemperatura);
  piscaLED();

  // o thingspeak precisa de pelo menos 16 segundos entre atualizacoes
  delay(180000);  // 3 minutos
}

String floatToString(float f) {
  char buf[16];
  return dtostrf(f, 4, 1, buf);
}

String sendData(String command, const int timeout = 2000, const int val_delay = 0, const boolean is_display = false)
{
  // Envio dos comandos AT para o modulo
  String response = "";
  esp8266.print(command);
  delay(val_delay);
  if (is_display) Serial.println("\r\n---------------------");
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }

  if (is_display) Serial.print(response);
  
  return response;
}

void init_ESP8266(){   
    Serial.println("---------------------\r\n> Iniciando ESP8266 <\r\n---------------------");
    
    //Reseta as configurações anteriores - se houver
    sendData("AT+RST\r\n", 500, 2000, false);

    //Reseta as configurações anteriores - se houver
    sendData("AT+GMR\r\n");
         
    // Conecta na rede wireless **OBRIGATÓRIO**
    String cwjap = "AT+CWJAP=\"#SSID#\",\"#PSW#\"\r\n";
    cwjap.replace("#SSID#", strSSID);
    cwjap.replace("#PSW#", strPSW);
    sendData(cwjap, 500, 3000);
 
    // Define a velocidade de comunicacao do modulo ESP8266 (9600, 19200, 38400, 57600, etc)
    sendData("AT+CIOBAUD=9600\r\n");     

    // Modo de coneção da ESP - Station mode (client) and AP mode (host)
    sendData("AT+CWMODE=3\r\n", 1000, 0, true);

    // Set transfer mode (0: normal mode)
    sendData("AT+CIPMODE=0\r\n", 1000, 0, true);    
    
    // Obtêm o endereco IP atribuído pelo roteador
    // sendData("AT+CIFSR\r\n", 1000, 0, true);
    
    // Configura para uma única conexão
    sendData("AT+CIPMUX=0\r\n", 1000, 0, true);

    // Configura para uma única conexão
    sendData("AT+CIPSTATUS\r\n");    
    
    Serial.println("---------------------\r\n>  ESP8266 ativado  <\r\n---------------------");
}

void piscaLED(){
  // pisca o LED a cada envio
  digitalWrite(pinoLED, HIGH);   
  delay(200);               
  digitalWrite(pinoLED, LOW);
}

void enviaDadosThingSpeak(String strLuminosidade, String strUmidade, String strTemperatura){

 
  // Conexao com TCP com Thingspeak -> GET https://api.thingspeak.com/update?api_key=
  String cmd = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n";
  sendData(cmd, 500, 50, true);

  if(esp8266.find("ERROR")){
    Serial.println("===> AT+CIPSTART=TCP error");
    return;
  }

  // preparacao da string GET
  String getStr = "GET /update?api_key=" + chaveDeEscrita;
  getStr += "&field1=" + strLuminosidade;
  getStr += "&field2=" + strUmidade;
  getStr += "&field3=" + strTemperatura;
  getStr += "\r\n";

  // envia o tamanho dos dados
  cmd = "AT+CIPSEND=";
  cmd += getStr.length();
  cmd += "\r\n";
  sendData(cmd, 500, 50, true);

  // envia comando de GET
  //if(esp8266.find(">")){
    sendData(getStr, 500, 500, false);
    //Serial.println("\r\n===> Comando enviado ao Thingspeak:");
    Serial.println(getStr);    
  //} else {
    // Close TCP or UDP connection
    sendData("AT+CIPCLOSE\r\n");
    // alerta usuario
    //Serial.println("\r\n===> Conexão fechada!!");
  //} 

}
