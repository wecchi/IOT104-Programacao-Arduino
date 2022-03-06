/*
 *  Aulas Online - LSI-TEC: IOT104 Programação física com Arduino
 *  Programa/hardware: 020 - Aula 12 - Projeto Final: Monitoramento da Chácara
 *  
 *  Descrição: Registrar e monitorar comportamento de pequena plantação
 *             - Sensor de humidade do solo
 *             - Sensor de queimada
 *             - Sensor de temperatura externa
 *             - Atuador: LED RGB para reportar a humidade do solo
 *             - Atuador: LED AMARELO para sinalizar temperatura acima de 35ºC
 *             - Atuador: Buzzer quando detectado queimada
 *             - Atuador: Módulo de comunicação ESP01-8266 para envio de dados à Internet
 *  
 *  Autor:            Marcelo Wecchi
 *  Data/Atualização: 2022.03.06
 */

 
// -----> Bibliotecas auxiliares
#include <OneWire.h>
#include <DallasTemperature.h>
#include <stdlib.h>
#include <SoftwareSerial.h>


// -----> Definição das portas
#define humidadePin A0
#define temperaturaPin A5
#define queimadaPin 10
#define rxPin 11
#define txPin 12
#define buzzerPin 9
#define ledAmPin 8
#define rgbVmPin 2
#define rgbVdPin 3
#define rgbAzPin 4


// -----> Definição das constantes
String strSSID = "NOME_DA_REDE";               // Nome da rede WiFi
String strPSW = "SENHA_CONEXAO_DA_REDE";       // Senha da rede WiFi
const float tempLimite = 35.00;                // Temperatura crítica (acima de)
const int delayGeral = 30000;                  // 30 segundos
const bool debug = false;                      // Imprime dados no Serial
const String chaveDeEscrita = "WRITE_API_KEY"; // Chave para gravação de dados no canal do https://thingspeak.com/


// -----> Setup dos objetos auxiliares
OneWire oneWire(temperaturaPin);
DallasTemperature sensor18B20(&oneWire);
SoftwareSerial esp8266(rxPin, txPin); // Inicializa comunicacao com WiFi


// -----> Funções auxiliares
String floatToString(float f) {
  char buf[16];
  return dtostrf(f, 4, 1, buf);
}


String alertaHumidade(int humidade){
  /*  The sensor value description:
   *      0  ~300     dry soil
   *      300~700     humid soil
   *      700~950     in water
  */
  if (debug){Serial.print("Humidade: "); Serial.println(humidade);}

  if(humidade>=0 && humidade <=300){
      // solo seco
      digitalWrite(rgbVmPin, HIGH);
      digitalWrite(rgbVdPin, LOW);
      digitalWrite(rgbAzPin, LOW);
  } else if (humidade > 300 && humidade <=700) {
      // solo húmido
      digitalWrite(rgbVmPin, LOW);
      digitalWrite(rgbVdPin, HIGH);
      digitalWrite(rgbAzPin, LOW);
  } else if (humidade > 700){
      // solo enxarcado
      digitalWrite(rgbVmPin, LOW);
      digitalWrite(rgbVdPin, LOW);
      digitalWrite(rgbAzPin, HIGH);    
  }

   // converte leituras para String
  return floatToString(humidade);
}


String alertaTemperatura(float temperatura, float limite){
  // Pisca LED Amarelo quando temperatura maior que limite
  if (debug) {Serial.print("Temperatura: "); Serial.println(temperatura);}
  digitalWrite(ledAmPin, temperatura > limite ? HIGH: LOW);
  return floatToString(temperatura);
}


String alertaQueimada(int fogo){
  // Liga/Desliga Buzzer
  if (debug) {Serial.print("Queimada: "); Serial.println(fogo);}
  digitalWrite(buzzerPin, fogo);
  return floatToString(fogo);
}


String sendData(String command, const int timeout = 2000, const int val_delay = 0)
{
  // Envio dos comandos AT para o modulo
  String response = "";
  esp8266.print(command);
  delay(val_delay);
  if (debug) Serial.println("\r\n---------------------");
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

  if (debug) Serial.print(response);
  
  return response;
}

void init_ESP8266(){   
    if (debug) Serial.println("---------------------\r\n> Iniciando ESP8266 <\r\n---------------------");
    
    //Reseta as configurações anteriores - se houver
    sendData("AT+RST\r\n", 500, 2000);

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
    sendData("AT+CWMODE=3\r\n", 1000, 0);

    // Set transfer mode (0: normal mode)
    sendData("AT+CIPMODE=0\r\n", 1000, 0);    
    
    // Obtêm o endereco IP atribuído pelo roteador
    // sendData("AT+CIFSR\r\n", 1000, 0);
    
    // Configura para uma única conexão
    sendData("AT+CIPMUX=0\r\n", 1000, 0);

    // Configura para uma única conexão
    sendData("AT+CIPSTATUS\r\n");    
    
    if (debug) Serial.println("---------------------\r\n>  ESP8266 ativado  <\r\n---------------------");
}

void piscaLED(){
  // Sinaliza ciclo com LED externo (Branco)
  digitalWrite(LED_BUILTIN, HIGH);
  delayMicroseconds(delayGeral);
  digitalWrite(LED_BUILTIN, LOW);
}

void enviaDadosThingSpeak(String strH, String strT, String strQ){

  // Conexao com TCP com Thingspeak -> GET https://api.thingspeak.com/update?api_key=
  String cmd = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n";
  sendData(cmd, 500, 50);

  if(esp8266.find("ERROR")){
    Serial.println("===> AT+CIPSTART=TCP error");
    return;
  }

  // preparacao da string GET
  String getStr = "GET /update?api_key=" + chaveDeEscrita;
  getStr += "&field1=" + strH;
  getStr += "&field2=" + strT;
  getStr += "&field3=" + strQ;
  getStr += "\r\n";

  // envia o tamanho dos dados
  cmd = "AT+CIPSEND=";
  cmd += getStr.length();
  cmd += "\r\n";
  sendData(cmd, 500, 50);

  sendData(getStr, 500, 500);
  if (debug) {Serial.println(getStr);}

  sendData("AT+CIPCLOSE\r\n");
}


void setup() {
  // -----> Modo de operação das portas
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(humidadePin, INPUT);
  pinMode(temperaturaPin, INPUT);
  pinMode(queimadaPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledAmPin, OUTPUT);
  pinMode(rgbVmPin, OUTPUT);
  pinMode(rgbVdPin, OUTPUT);
  pinMode(rgbAzPin, OUTPUT);

  // -----> Inicialização dos objetos
  if (debug) {Serial.begin(9600);}
  esp8266.begin(115200); // Comunicacao com Modulo WiFi
  sensor18B20.begin();   // Comunicacao com Modulo de Temperatura
  if(esp8266.isListening()){
    init_ESP8266();    
  } else {
    Serial.println("Setup para ESP01 com problemas!!");
  }
}


void loop() {
  piscaLED();

  // Verifica sensores: Humidade, Temperatura e Queimada 
  String h = alertaHumidade(analogRead(humidadePin));
  sensor18B20.requestTemperatures();
  String t = alertaTemperatura(sensor18B20.getTempCByIndex(0), tempLimite);
  String q = alertaQueimada(digitalRead(queimadaPin));

  enviaDadosThingSpeak(h, t, q);
  delay(delayGeral);
}
