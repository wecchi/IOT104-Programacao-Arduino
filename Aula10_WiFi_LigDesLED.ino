/*
 *  Aulas Online - LSI-TEC: IOT104 Programação física com Arduino
 *  Programa/hardware: 015 - Aula 10 - Conectar o ESP01 (WiFi)
 *  
 *  Descrição: Configura um servidor na ESP-01S; a ideia é, pelo navegador, permitir:
 *     - controlar um atuador (liga ou desliga LED da Arduino);
 *     - ler um sensor (abre uma página com valor de leitura de conversão AD).
 * 
 *  The circuit:
 *     RX is digital rxPin (connect to TX of other device)
 *     TX is digital txPin (connect to RX of other device)
 * 
 *  Autor: Marcelo Wecchi
 *  Revisão: 01Mar2022
*/

// =============================================================================================================
// --- Bibliotecas ---
#include "SoftwareSerial.h"

// --- Portas utilizadas da Arduino UNO ---
#define RD 13     // Porta para ativação do LED vermelho RGB
#define GR 12     // Porta para ativação do LED verde RGB
#define BL 11     // Porta para ativação do LED azul RGB
#define LDR A0    // Porta para leitura do LDR
#define rxPin 2   // Porta para recebimento dos dados da ESP8266
#define txPin 3   // Porta para envio dos dados da ESP8266

// --- Constantes --- 
#define TIMEOUT 5000                      // miliSegundos para timeout
String strSSID = "@homehorana";           // Nome da rede WiFi
String strPSW = "2222Amor&Respeito2222";  // Senha da rede WiFi

// --- Variáveis --- 
int LDR_value = 0;          // Valor lido do LRD
int LED_value = -1;         // Valor mapeado para o LED RGB
String IncomingString="";   // String solicitada pelo Browser

// --- Criação de objetos --- 
SoftwareSerial esp8266(rxPin, txPin); // RX, TX


// =============================================================================================================
// --- Assinatura das Funções Auxiliares ---
void piscaLED(int LED);
void exibePaginaWeb(int connectionId, String textoH2);
String sendData(String command, const int timeout, const int val_delay, const boolean debug);
void init_ESP8266();


// =============================================================================================================
// --- Configurações Iniciais ---
void setup(){
  Serial.begin(9600);
  esp8266.begin(115200);
    
  // Define pin modes for LEDs and LDR
  pinMode(RD,OUTPUT);
  pinMode(GR, OUTPUT);
  pinMode(BL, OUTPUT);
  pinMode(LDR, INPUT);

  if(esp8266.isListening()){
    init_ESP8266();    
  } else {
    Serial.println("Setup has error and NOT complete!!");
  }
}


// =============================================================================================================
// --- Loop Infinito ---
void loop(){
  LDR_value = analogRead(LDR);
  LED_value = map(LDR_value, 50, 600, BL, RD);
    
  if (esp8266.available()){
    if (esp8266.find("+IPD,")){
      delay(300);

      // Obtêm o ID de conexão com o Cliente
      int connectionId = esp8266.read() - 48;

      // Lê caracteres do buffer serial e os move para uma String. A função termina se ocorre time-out 
      IncomingString = esp8266.readString();
      String response = "";
              
      if (IncomingString.indexOf("/LED=ON") != -1) { 
        response = "LED Aceso";
        exibePaginaWeb(connectionId, response);
        piscaLED(LED_value);
      } else if (IncomingString.indexOf("/LED=OFF") != -1) {
        response = "LED Apagado";
        exibePaginaWeb(connectionId, response);
        piscaLED(0);
      } else if(IncomingString.indexOf("/LDR") != -1)    {
        char valueStr[4];
        sprintf(valueStr,"%04d",LDR_value);
        response = "Valor do Resistor Dependente de Luz: ";
        response += valueStr;
        exibePaginaWeb(connectionId, response);        
      }   
   }
  }
}

// =============================================================================================================
// --- Funções Auxiliares ---
void piscaLED(int LED){
  for (int i = 1; i<4; i++){
    digitalWrite(i, LOW);
    delay(50);
  }
  if (LED>0) digitalWrite(LED, HIGH);
}

void exibePaginaWeb(int connectionId, String textoH2){
  String webpage = "<head><meta http-equiv=""refresh"" content=""2"">";
  webpage += "</head><h1><u>Aula 10 - Conectar o ESP01 (WiFi)</u></h1><h2>";
  webpage += textoH2;
  webpage += "</h2>";
  
  String cipSend = "AT+CIPSEND=";    
  cipSend += connectionId;       
  cipSend += ",";
  cipSend += webpage.length();
  cipSend += "\r\n";
  
  sendData(cipSend, 500, 50, true);
  sendData(webpage, 500, 50, true);
  
  String closeCommand = "AT+CIPCLOSE=";
  closeCommand += connectionId; // append connection id
  closeCommand += "\r\n";
  sendData(closeCommand, 1000, 50, true); 
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
    sendData("AT+RST\r\n");
    
    // Conecta na rede wireless **OBRIGATÓRIO**
    String cwjap = "AT+CWJAP=\"#SSID#\",\"#PSW#\"\r\n";
    cwjap.replace("#SSID#", strSSID);
    cwjap.replace("#PSW#", strPSW);
    sendData(cwjap, 2000, 3000);
    
    // Define a velocidade de comunicacao do modulo ESP8266 (9600, 19200, 38400, 57600, etc)
    sendData("AT+CIOBAUD=19200\r\n");

    // Modo de coneção da ESP - Station mode (client)
    sendData("AT+CWMODE=1\r\n", 1000, 0, true);
    
    // Obtêm o endereco IP atribuído pelo roteador
    sendData("AT+CIFSR\r\n", 1000, 0, true);
    
    // Configura para multiplas conexoes
    sendData("AT+CIPMUX=1\r\n", 1000, 0, true);
    
    // Inicia o web server na porta 80
    sendData("AT+CIPSERVER=1,80\r\n", 1000, 500, true);

    // Sets the TCP Server Timeout
    sendData("AT+CIPSTO=10\r\n", 1000, 500, true);

    Serial.println("---------------------\r\n>  ESP8266 ativado  <\r\n---------------------");
}
